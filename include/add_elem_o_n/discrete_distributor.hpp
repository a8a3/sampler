#pragma once

#include <cassert>
#include <cstdlib>
#include <vector>

namespace add_elem_0_n {

template<typename T>
class DiscreteDistributionSampler
{
public:
    explicit DiscreteDistributionSampler(std::vector<std::pair<T, float>> objectsWithWeights)
    {
        for (const auto& [_, weight]: objectsWithWeights) {
            assert(weight > 0.f);
            totalWeight_ += weight;
        }
        
        float currentWeight = 0.f;
        objects_.reserve(objectsWithWeights.size());
        cumulativeProbabilities_.reserve(objectsWithWeights.size());

        for (auto& [object, weight]: objectsWithWeights) {
            objects_.push_back(std::move(object));
            currentWeight += weight;
            cumulativeProbabilities_.push_back(currentWeight / totalWeight_);
        }
    }

    void AddObject(T obj, float weight) {
        assert(weight > 0.f);

        for (auto& probability: cumulativeProbabilities_) {
            probability *= totalWeight_ / (totalWeight_ + weight);
        }
        cumulativeProbabilities_.push_back(1.f);
        objects_.push_back(std::move(obj));
        totalWeight_ += weight;
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
    float totalWeight_{0.f};
};

} // namespace add_elem_o_n
