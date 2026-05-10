#include <gtest/gtest.h>

#include <set>
#include <vector>

#include "add_elem_o_n/discrete_distributor.hpp"
#include "add_elem_o_1/discrete_distributor.hpp"

struct AddElemONTraits
{
    using ValueType = int;
    static add_elem_0_n::DiscreteDistributionSampler<int> Make(const std::vector<std::pair<int, float>>& items)
    {
        return add_elem_0_n::DiscreteDistributionSampler<int>(items);
    }
};

struct AddElemO1Traits
{
    using ValueType = int;
    static add_elem_o_1::DiscreteDistributionSampler<int> Make(const std::vector<std::pair<int, float>>& items)
    {
        return add_elem_o_1::DiscreteDistributionSampler<int>(items);
    }
};


template<typename Traits>
class AddElemCommonTest : public ::testing::Test {};

using AddElemSamplerTypes = ::testing::Types<AddElemONTraits, AddElemO1Traits>;
TYPED_TEST_SUITE(AddElemCommonTest, AddElemSamplerTypes);


TYPED_TEST(AddElemCommonTest, AddedOutcomeIsReachable)
{
    auto sampler = TypeParam::Make({{1, 1.0f}});
    sampler.AddObject(2, 1.0f);

    std::set<int> seen;
    for (int i = 0; i < 1'000; ++i) {
        seen.insert(sampler.Sample());
    }
    ASSERT_TRUE(seen.count(2));
}

TYPED_TEST(AddElemCommonTest, OriginalOutcomeStillReachableAfterAdd)
{
    auto sampler = TypeParam::Make({{1, 1.0f}});
    sampler.AddObject(2, 1.0f);

    std::set<int> seen;
    for (int i = 0; i < 1'000; ++i) {
        seen.insert(sampler.Sample());
    }
    ASSERT_TRUE(seen.count(1));
}

TYPED_TEST(AddElemCommonTest, AllOutcomesReachableAfterMultipleAdds)
{
    auto sampler = TypeParam::Make({{1, 1.0f}});
    sampler.AddObject(2, 1.0f);
    sampler.AddObject(3, 1.0f);

    std::set<int> seen;
    for (int i = 0; i < 1'000; ++i) {
        seen.insert(sampler.Sample());
    }
    ASSERT_EQ(seen.size(), 3u);
}

TYPED_TEST(AddElemCommonTest, HighWeightAddedOutcomeSampledMoreOften)
{
    auto sampler = TypeParam::Make({{1, 1.0f}});
    sampler.AddObject(99, 10.0f);

    int heavyCount = 0;
    constexpr int kTrials = 1'000;
    for (int i = 0; i < kTrials; ++i) {
        if (sampler.Sample() == 99) {
            ++heavyCount;
        }
    }
    ASSERT_GT(heavyCount, 800);
}

TYPED_TEST(AddElemCommonTest, SampleOnlyFromKnownOutcomesAfterAdd)
{
    auto sampler = TypeParam::Make({{10, 1.0f}, {20, 1.0f}});
    sampler.AddObject(30, 1.0f);

    for (int i = 0; i < 500; ++i) {
        const auto result = sampler.Sample();
        ASSERT_TRUE(result == 10 || result == 20 || result == 30);
    }
}
