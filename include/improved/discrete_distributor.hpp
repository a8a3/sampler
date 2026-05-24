#pragma once

#include <cassert>
#include <random>
#include <vector>

namespace improved {

template<typename T, typename URNG = std::mt19937>
class DiscreteDistributionSampler
{
public:
    explicit DiscreteDistributionSampler(
        std::vector<std::pair<T, float>> objectsWithWeights
       ,URNG rng = URNG{std::random_device{}()}
    ) : urng_(std::move(rng))
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
        auto target = std::uniform_real_distribution<float>{0.f, totalWeight_}(urng_);
        auto it = std::lower_bound(cumulativeProbabilities_.begin(), cumulativeProbabilities_.end(), target);
        assert(it != cumulativeProbabilities_.end());
        return objects_[it - cumulativeProbabilities_.begin()];
    }

private:
    std::vector<T> objects_;
    std::vector<float> cumulativeProbabilities_;
    float totalWeight_{0.f};
    mutable URNG urng_;
};

} // improved
