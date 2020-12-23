#pragma once

#include <random>

template<typename T>
using range_distribution = typename std::conditional<std::is_integral<T>::value, std::uniform_int_distribution<T>, std::uniform_real_distribution<T>>::type;

template<typename T, typename RNG = std::mt19937_64>
class RandomNumber {
  private:
    static std::random_device random_dev;
    RNG rng;
    range_distribution<T> distribution;

  public:
    RandomNumber(T a, T b, unsigned int seed) : rng(seed), distribution(a, b) {}
    RandomNumber(T a, T b) : RandomNumber(a, b, random_dev()) {}
    RandomNumber() : RandomNumber(0, 1, random_dev()) {}
    T draw() { return distribution(rng); }
    T operator()() { return draw(); }
};
