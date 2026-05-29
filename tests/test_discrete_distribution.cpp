#include <gtest/gtest.h>

#include "improved/discrete_distributor.hpp"

struct ImprovedSamplerTraits
{
    using ValueType = int;
    static improved::DiscreteDistributionSampler<int> Make(std::vector<std::pair<int, float>> items) {
        return improved::DiscreteDistributionSampler<int>(items);
    }
};

template<typename Traits>
class DiscreteDistributionSamplerCommonTest : public ::testing::Test {};

using SamplerTypes = ::testing::Types<ImprovedSamplerTraits>;
TYPED_TEST_SUITE(DiscreteDistributionSamplerCommonTest, SamplerTypes);

TYPED_TEST(DiscreteDistributionSamplerCommonTest, SingleElementAlwaysReturned)
{
    auto sampler = TypeParam::Make({{42, 1.0f}});
    ASSERT_EQ(sampler.SampleByValue(.1f), 42);
    ASSERT_EQ(sampler.SampleByValue(.5f), 42);
    ASSERT_EQ(sampler.SampleByValue(.9f), 42);
}

TYPED_TEST(DiscreteDistributionSamplerCommonTest, AllElementsAreReachable)
{
    auto sampler = TypeParam::Make({{1, 1.0f}, {2, 1.0f}, {3, 1.0f}});
    ASSERT_EQ(sampler.SampleByValue(0.5f), 1);
    ASSERT_EQ(sampler.SampleByValue(1.5f), 2);
    ASSERT_EQ(sampler.SampleByValue(2.5f), 3);
}
