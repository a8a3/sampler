#include <gtest/gtest.h>

#include <set>
#include <vector>

#include "improved/discrete_distributor.hpp"

struct ImprovedTraits
{
    using ValueType = int;
    static improved::DiscreteDistributionSampler<int> Make(const std::vector<std::pair<int, float>>& items)
    {
        return improved::DiscreteDistributionSampler<int>(items);
    }
};

template<typename Traits>
class AddElemCommonTest : public ::testing::Test {};

using AddElemSamplerTypes = ::testing::Types<ImprovedTraits>;
TYPED_TEST_SUITE(AddElemCommonTest, AddElemSamplerTypes);

TYPED_TEST(AddElemCommonTest, AddedOutcomeIsReachable)
{
    auto sampler = TypeParam::Make({{1, 1.0f}});
    ASSERT_EQ(sampler.SampleByValue(.9f), 1);

    sampler.AddObject(2, 1.0f);
    ASSERT_EQ(sampler.SampleByValue(1.1f), 2);
}

TYPED_TEST(AddElemCommonTest, OriginalOutcomeStillReachableAfterAdd)
{
    auto sampler = TypeParam::Make({{1, 1.0f}});
    ASSERT_EQ(sampler.SampleByValue(.9f), 1);

    sampler.AddObject(2, 1.0f);
    ASSERT_EQ(sampler.SampleByValue(.9f), 1);
}

TYPED_TEST(AddElemCommonTest, AllOutcomesReachableAfterMultipleAdds)
{
    auto sampler = TypeParam::Make({{1, 1.0f}});
    sampler.AddObject(2, 1.0f);
    sampler.AddObject(3, 1.0f);

    ASSERT_EQ(sampler.SampleByValue(0.9f), 1);
    ASSERT_EQ(sampler.SampleByValue(1.9f), 2);
    ASSERT_EQ(sampler.SampleByValue(2.9f), 3);
}
