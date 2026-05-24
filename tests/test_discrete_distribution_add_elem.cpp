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
