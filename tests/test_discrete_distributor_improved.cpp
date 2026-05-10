#include <gtest/gtest.h>

#include <set>
#include <string>

#include "improved/discrete_distributor.hpp"

TEST(ImprovedDiscreteDistributionSampler, SupportsStringType)
{
    auto sampler = improved::DiscreteDistributionSampler<std::string>(
        {{"hello", 1.0f},
        {"world", 1.0f}}
    );
    const auto& result = sampler.Sample();
    ASSERT_TRUE(result == "hello" || result == "world");
}

TEST(ImprovedDiscreteDistributionSampler, SingleStringElementAlwaysReturned)
{
    auto sampler = improved::DiscreteDistributionSampler<std::string>(
        {{"only", 1.0f}}
    );
    ASSERT_EQ(sampler.Sample(), "only");
}

TEST(ImprovedDiscreteDistributionSampler, AllStringElementsAreReachable)
{
    auto sampler = improved::DiscreteDistributionSampler<std::string>(
        {{"a", 1.0f}, {"b", 1.0f}, {"c", 1.0f}}
    );
    std::set<std::string> seen;
    for (int i = 0; i < 1000; ++i) {
        seen.insert(sampler.Sample());
    }
    ASSERT_EQ(seen.size(), 3u);
}
