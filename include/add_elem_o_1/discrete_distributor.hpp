#pragma once

#include <cassert>
#include <cstdlib>
#include <vector>

namespace add_elem_o_1 {

template<typename T>
class DiscreteDistributionSampler
{
public:
    explicit DiscreteDistributionSampler(std::vector<std::pair<T, float>> objectsWithWeights)
    {
        objects_.reserve(objectsWithWeights.size());
        cumulativeProbabilities_.reserve(objectsWithWeights.size());

        for (auto& [object, weight]: objectsWithWeights) {
            assert(weight > 0.f);
            objects_.push_back(std::move(object));
            totalWeight_ += weight;
            cumulativeProbabilities_.push_back(totalWeight_);
        }
    }

    void AddObject(T obj, float weight) {
        assert(weight > 0.f);
        totalWeight_ += weight;
        cumulativeProbabilities_.push_back(totalWeight_);
        objects_.push_back(std::move(obj));
    }

    const T& Sample() const
    {
        const auto r = std::rand();
        auto target = (static_cast<float>(r) / RAND_MAX) * totalWeight_;
        auto it = std::lower_bound(cumulativeProbabilities_.begin(), cumulativeProbabilities_.end(), target);
        assert(it != cumulativeProbabilities_.end());
        
        return objects_[it - cumulativeProbabilities_.begin()];
    }

private:
    std::vector<T> objects_;
    std::vector<float> cumulativeProbabilities_;
    float totalWeight_{0.f};
};

} // namespace add_elem_o_1
