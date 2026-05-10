#include <gtest/gtest.h>

#include <set>
#include <vector>

#include "origin/discrete_distributor.hpp"
#include "improved/discrete_distributor.hpp"
#include "add_elem_o_n/discrete_distributor.hpp"
#include "add_elem_o_1/discrete_distributor.hpp"

struct OriginSamplerTraits
{
    using ValueType = int;
    static origin::DiscreteDistributionSampler Make(std::vector<std::pair<int, float>> items)
    {
        return origin::DiscreteDistributionSampler(items);
    }
};

struct ImprovedSamplerTraits
{
    using ValueType = int;
    static improved::DiscreteDistributionSampler<int> Make(std::vector<std::pair<int, float>> items)
    {
        return improved::DiscreteDistributionSampler<int>(items);
    }
};

template<typename Traits>
class DiscreteDistributionSamplerCommonTest : public ::testing::Test {};

struct AddElemONSamplerTraits
{
    using ValueType = int;
    static add_elem_0_n::DiscreteDistributionSampler<int> Make(const std::vector<std::pair<int, float>>& items)
    {
        return add_elem_0_n::DiscreteDistributionSampler<int>(items);
    }
};

struct AddElemO1SamplerTraits
{
    using ValueType = int;
    static add_elem_o_1::DiscreteDistributionSampler<int> Make(const std::vector<std::pair<int, float>>& items)
    {
        return add_elem_o_1::DiscreteDistributionSampler<int>(items);
    }
};

using SamplerTypes = ::testing::Types<OriginSamplerTraits, ImprovedSamplerTraits, AddElemONSamplerTraits, AddElemO1SamplerTraits>;
TYPED_TEST_SUITE(DiscreteDistributionSamplerCommonTest, SamplerTypes);

// Common tests
TYPED_TEST(DiscreteDistributionSamplerCommonTest, SingleElementAlwaysReturned)
{
    auto sampler = TypeParam::Make({{42, 1.0f}});
    ASSERT_EQ(sampler.Sample(), 42);
}

TYPED_TEST(DiscreteDistributionSamplerCommonTest, SampleIsAlwaysFromInputSet)
{
    auto sampler = TypeParam::Make({{10, 1.0f}, {20, 2.0f}, {30, 3.0f}});
    for (int i = 0; i < 100; ++i) {
        const auto result = sampler.Sample();
        ASSERT_TRUE(result == 10 || result == 20 || result == 30);
    }
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
