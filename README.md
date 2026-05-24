# Discrete Distribution Sampler

## Samplers

### [origin::DiscreteDistributionSampler](include/origin/discrete_distributor.hpp)

Оригинальный семплер (для референса)

### [improved::DiscreteDistributionSampler\<T\>](include/improved/discrete_distributor.hpp)

Шаблонный семплер, с улучшениями в интерфейсе

- добавлениe элемента за амортизированное O(1) (см. `AddObject`);

- возможность параметризовать семплер генератором случайных значений;