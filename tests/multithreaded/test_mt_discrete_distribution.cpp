#include <gtest/gtest.h>

#include <atomic>
#include <thread>
#include <vector>

#include "improved/rwlock/discrete_distributor.hpp"

TEST(TsanSmokeTest, DISABLED_DetectsDataRace)
{
    int shared = 0;
    std::thread t1([&]() { shared = 1; });
    std::thread t2([&]() { shared = 2; });
    t1.join();
    t2.join();
}

struct RwlockSamplerTraits
{
    using SamplerType = improved::rwlock::DiscreteDistributionSampler<int>;

    static SamplerType Make(std::vector<std::pair<int, float>> items)
    {
        return SamplerType(std::move(items));
    }
};

template<typename Traits>
class MtDiscreteDistributionTest : public ::testing::Test {};

using MtSamplerTypes = ::testing::Types<RwlockSamplerTraits>;
TYPED_TEST_SUITE(MtDiscreteDistributionTest, MtSamplerTypes);

TYPED_TEST(MtDiscreteDistributionTest, SampleByValueReturnsCorrectElement)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {2, 1.0f}, {3, 1.0f}});
    EXPECT_EQ(sampler.SampleByValue(0.5f), 1);
    EXPECT_EQ(sampler.SampleByValue(1.5f), 2);
    EXPECT_EQ(sampler.SampleByValue(2.5f), 3);
}

TYPED_TEST(MtDiscreteDistributionTest, SampleByValueRespectsWeights)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {99, 10.0f}});
    EXPECT_EQ(sampler.SampleByValue(0.5f),  1);
    EXPECT_EQ(sampler.SampleByValue(5.0f),  99);
    EXPECT_EQ(sampler.SampleByValue(10.9f), 99);
}

TYPED_TEST(MtDiscreteDistributionTest, ConcurrentSamplesProduceValidValues)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {2, 2.0f}, {3, 3.0f}});

    constexpr int kThreads    = 8;
    constexpr int kIterations = 10'000;

    std::vector<std::thread> threads;
    threads.reserve(kThreads);

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&sampler]() {
            for (int i = 0; i < kIterations; ++i) {
                int val = sampler.Sample();
                EXPECT_TRUE(val == 1 || val == 2 || val == 3);
            }
        });
    }

    for (auto& t : threads) t.join();
}

TYPED_TEST(MtDiscreteDistributionTest, ConcurrentSampleByValueProducesCorrectValues)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {2, 1.0f}, {3, 1.0f}});

    constexpr int kThreads    = 8;
    constexpr int kIterations = 10'000;

    const float targets[]  = {0.5f, 1.5f, 2.5f};
    const int   expected[] = {1,    2,    3};

    std::vector<std::thread> threads;
    threads.reserve(kThreads);

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            const float target = targets[t % 3];
            const int   exp    = expected[t % 3];
            for (int i = 0; i < kIterations; ++i) {
                int val = sampler.SampleByValue(target);
                EXPECT_EQ(val, exp);
            }
        });
    }

    for (auto& t : threads) t.join();
}

TYPED_TEST(MtDiscreteDistributionTest, ConcurrentAddAndSampleDoNotCrash)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {2, 1.0f}});

    constexpr int kReaders        = 6;
    constexpr int kWriterAdds     = 1'000;
    constexpr int kReaderIter     = 10'000;

    std::thread writer([&]() {
        for (int i = 0; i < kWriterAdds; ++i) {
            sampler.AddObject(3, 1.0f);
        }
    });

    std::vector<std::thread> readers;
    readers.reserve(kReaders);
    for (int t = 0; t < kReaders; ++t) {
        readers.emplace_back([&]() {
            for (int i = 0; i < kReaderIter; ++i) {
                int val = sampler.Sample();
                EXPECT_TRUE(val == 1 || val == 2 || val == 3);
            }
        });
    }

    writer.join();
    for (auto& r : readers) r.join();
}
