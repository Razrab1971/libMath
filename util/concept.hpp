#pragma once

#include <concepts>
#include <ranges>

//Концепт типа числа
template<typename NumberType>
concept numberRConcept = std::integral<NumberType> || std::floating_point<NumberType>;

//Концепт типа функции
template<typename NumberType, typename Func>
concept funcNumberRC = numberRConcept<NumberType> && requires(NumberType x, Func f) {
    { f(x) } -> std::convertible_to<NumberType>;
};

//Концепт типа vector, со вставкой в конец
template<typename VecPoints>
concept vecPointsC = std::ranges::contiguous_range<VecPoints> &&  requires(VecPoints vec, typename VecPoints::value_type val)
{
    vec.insert(vec.end(), val);
};

