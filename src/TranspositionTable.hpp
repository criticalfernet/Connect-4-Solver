#pragma once

#include <cstdint>
#include <algorithm>
#include <cassert>
#include <type_traits>

constexpr uint64_t med(uint64_t min, uint64_t max) {
  return (min+max)/2;
}

constexpr bool has_factor(uint64_t n, uint64_t min, uint64_t max) {
  if (min*min > n) return false;
  if(min + 1 >= max) return (n % min == 0);

  return has_factor(n, min, med(min,max)) || has_factor(n, med(min,max), max);
}

constexpr uint64_t next_prime(uint64_t n) {
  if (has_factor(n, 2, n))
    return next_prime(n + 1);
  return n;
}

template<unsigned int key_size, unsigned int value_size, unsigned int log_size>
class TranspositionTable {
private:
  static_assert(key_size <= 64);
  static_assert(value_size <= 64);
  static_assert(log_size < 31);
  
  template<int S> 
  using uint_t =
    std::conditional_t<S <= 8, uint_least8_t,
    std::conditional_t<S <= 16, uint_least16_t,
    std::conditional_t<S <= 32, uint_least32_t,
    uint_least64_t>>>;

  using key_t = uint_t<key_size - log_size>;
  using value_t = uint_t<value_size>;

  static constexpr size_t size = next_prime(uint64_t{1} << log_size);

  key_t* K;
  value_t* V;

  size_t index(uint64_t key) const {
      return key % size;
  }

public:
  TranspositionTable() {
    K = new key_t[size];
    V = new value_t[size];
    reset();
  }

  ~TranspositionTable() {
    delete[] K;
    delete[] V;
  }

  void reset() {
    std::fill_n(K, size, key_t{});
    std::fill_n(V, size, value_t{});
  }

  void put(uint64_t key, value_t value) {
    assert((key >> key_size) == 0);
    assert((value >> value_size) == 0);

    size_t pos = index(key);
    K[pos] = static_cast<key_t>(key);
    V[pos] = value;
  }

  value_t get(uint64_t key) const {
    assert((key >> key_size) == 0);

    size_t pos = index(key);
    if (K[pos] == static_cast<key_t>(key))
        return V[pos];
    return 0;
  }
};