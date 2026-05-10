#pragma once

#include <cassert>
#include <cstdlib>
#include <vector>

namespace origin {

class DiscreteDistributionSampler
{
public:
    DiscreteDistributionSampler(std::vector<std::pair<int, float>> objectsWithWeights)
    {
        float totalWeight = 0.0;
        for (auto [object, weight] : objectsWithWeights) {
            objects_.push_back(object);
            assert(weight > 0.0);
            totalWeight += weight;
        }
        
        float currentWeight = 0.0;
        cumulativeProbabilities_.push_back(0.0);
        for (auto [_, weight] : objectsWithWeights) {
            currentWeight += weight;
            cumulativeProbabilities_.push_back(currentWeight / totalWeight);
        }
    }

    int Sample()
    {
        auto r = std::rand();
        auto target = static_cast<float>(r) / RAND_MAX;
        auto it = std::lower_bound(cumulativeProbabilities_.begin(), cumulativeProbabilities_.end(), target);
        if (it == cumulativeProbabilities_.begin()) {
            ++it;
        }
        assert(it != cumulativeProbabilities_.end());

        return objects_[it - cumulativeProbabilities_.begin() - 1];
    }

private:
    std::vector<int> objects_;
    std::vector<float> cumulativeProbabilities_;
};

} // namespace origin
