# Discrete Distribution Sampler

## Samplers

### [origin::DiscreteDistributionSampler](include/origin/discrete_distributor.hpp)

Оригинальный семплер (для референса)

### [improved::DiscreteDistributionSampler\<T\>](include/improved/discrete_distributor.hpp)

Задание #1 Шаблонный семплер, с незначительными улучшениями интерфейса

### [add_elem_0_n::DiscreteDistributionSampler\<T\>](include/add_elem_o_n/discrete_distributor.hpp)

Заданиe #2 Развитие задания #1. Семплер с возможностью добавления элемента (см. `AddObject`). Добавление за О(N)- пересчет значений в массиве с вероятностями.

### [add_elem_o_1::DiscreteDistributionSampler\<T\>](include/add_elem_o_1/discrete_distributor.hpp)

Задание #2. Развитие задания #1. Семплер с возможностью добавления значения за амортизированное О(1), при этом первая выдача объекта после добавления занимает O(N).
