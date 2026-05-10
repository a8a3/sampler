#pragma once

#include <cassert>
#include <cstdlib>
#include <vector>

namespace add_elem_o_1{

template<typename T>
class DiscreteDistributionSampler
{
public:
    explicit DiscreteDistributionSampler(const std::vector<std::pair<T, float>>& objectsWithWeights)
    {
        objects_.reserve(objectsWithWeights.size());
        weights_.reserve(objectsWithWeights.size());

        for (const auto& [object, weight]: objectsWithWeights) {
            assert(weight > 0.f);
            weights_.push_back(weight);
            totalWeight_ += weight;
            objects_.push_back(object);
        }
    }

    void AddObject(const T& obj, float weight) {
        assert(weight > 0.f);
        weights_.push_back(weight);
        totalWeight_ += weight;
        objects_.push_back(obj);
        dirty_ = true;
    }
    
    const T& Sample()
    {
        if (dirty_) {
            RebuildProbabilities();
            dirty_ = false;
        }
        auto target = static_cast<float>(std::rand()) / RAND_MAX;

        auto it = std::lower_bound(cumulativeProbabilities_.begin(), cumulativeProbabilities_.end(), target);
        assert(it != cumulativeProbabilities_.end());

        return objects_[it - cumulativeProbabilities_.begin()];
    }

private:
    void RebuildProbabilities() {
        float currentWeight{0.f};
        cumulativeProbabilities_.clear();
        cumulativeProbabilities_.reserve(weights_.size());

        for (auto weight: weights_) {
            currentWeight += weight;
            cumulativeProbabilities_.push_back(currentWeight / totalWeight_);
        }
    }

private:
    std::vector<T> objects_;
    std::vector<float> weights_;
    std::vector<float> cumulativeProbabilities_;
    float totalWeight_{0.f};
    bool dirty_{true};
};

} // namespace add_elem_o_1
