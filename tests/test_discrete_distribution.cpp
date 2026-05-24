#include <gtest/gtest.h>

#include <random>
#include <set>
#include <vector>

#include "improved/discrete_distributor.hpp"

struct ImprovedSamplerTraits
{
    using ValueType = int;
    static improved::DiscreteDistributionSampler<int> Make(std::vector<std::pair<int, float>> items) {
        return improved::DiscreteDistributionSampler<int>(items, std::mt19937{42});
    }
};

template<typename Traits>
class DiscreteDistributionSamplerCommonTest : public ::testing::Test {};

using SamplerTypes = ::testing::Types<ImprovedSamplerTraits>;
TYPED_TEST_SUITE(DiscreteDistributionSamplerCommonTest, SamplerTypes);

// Common tests
TYPED_TEST(DiscreteDistributionSamplerCommonTest, SingleElementAlwaysReturned)
{
    auto sampler = TypeParam::Make({{42, 1.0f}});
    ASSERT_EQ(sampler.Sample(), 42);
}

TYPED_TEST(DiscreteDistributionSamplerCommonTest, AllElementsAreReachable)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {2, 1.0f}, {3, 1.0f}});
    std::set<int> seen;
    for (int i = 0; i < 1'000; ++i) {
        seen.insert(sampler.Sample());
    }
    ASSERT_EQ(seen.size(), 3u);
}

TYPED_TEST(DiscreteDistributionSamplerCommonTest, HighWeightElementSampledMoreOften)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {99, 10.0f}});
    int heavyCount = 0;
    constexpr int kTrials = 1'000;
    for (int i = 0; i < kTrials; ++i) {
        if (sampler.Sample() == 99) {
            ++heavyCount;
        }
    }
    ASSERT_GT(heavyCount, 800);
}
