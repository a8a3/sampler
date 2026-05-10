#pragma once

#include <cassert>
#include <cstdlib>
#include <vector>

namespace improved {

template<typename T>
class DiscreteDistributionSampler
{
public:
    explicit DiscreteDistributionSampler(const std::vector<std::pair<T, float>>& objectsWithWeights)
    {
        float totalWeight = 0.0;
        objects_.reserve(objectsWithWeights.size());

        for (const auto& [object, weight]: objectsWithWeights) {
            objects_.push_back(object);
            assert(weight > 0.0);
            totalWeight += weight;
        }
        
        float currentWeight = 0.0;
        cumulativeProbabilities_.reserve(objectsWithWeights.size());

        for (const auto& [_, weight]: objectsWithWeights) {
            currentWeight += weight;
            cumulativeProbabilities_.push_back(currentWeight / totalWeight);
        }
    }

    const T& Sample() const
    {
        const auto r = std::rand();
        auto target = static_cast<float>(r) / RAND_MAX;
        auto it = std::lower_bound(cumulativeProbabilities_.begin(), cumulativeProbabilities_.end(), target);
        assert(it != cumulativeProbabilities_.end());
        
        return objects_[it - cumulativeProbabilities_.begin()];
    }

private:
    std::vector<T> objects_;
    std::vector<float> cumulativeProbabilities_;
};

} // namespace improved
