#pragma once

#include <cassert>
#include <random>
#include <shared_mutex>
#include <vector>

namespace improved::rwlock {

template<typename T, typename URNG = std::mt19937>
class DiscreteDistributionSampler
{
public:
    explicit DiscreteDistributionSampler(
        std::vector<std::pair<T, float>> objectsWithWeights
    )
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

    void AddObject(T obj, float weight) 
    {
        assert(weight > 0.f);

        std::lock_guard guard{mtx_};
        totalWeight_ += weight;
        cumulativeProbabilities_.push_back(totalWeight_);
        objects_.push_back(std::move(obj));
    }

    T Sample() const
    {
        thread_local URNG rng = URNG{std::random_device{}()};

        std::shared_lock lock{mtx_};
        assert(totalWeight_ > 0.f);
        auto target = std::uniform_real_distribution<float>{0.f, totalWeight_}(rng);
        return DoSample(target);
    }

    T SampleByValue(float val) const
    {
        std::shared_lock lock{mtx_};
        assert(val >= 0.f && val < totalWeight_);
        return DoSample(val);
    }

private:
    T DoSample(float target) const
    {
        auto it = std::lower_bound(cumulativeProbabilities_.begin(), cumulativeProbabilities_.end(), target);
        assert(it != cumulativeProbabilities_.end());
        return objects_[it - cumulativeProbabilities_.begin()];
    }

    std::vector<T> objects_;
    std::vector<float> cumulativeProbabilities_;
    float totalWeight_{0.f};
    std::shared_mutex mtx_;
};

} // improved::rwlock
