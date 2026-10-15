#pragma once

#include <cstdint>
#include <cstring>
#include <cassert>
#include <type_traits>

constexpr uint64_t med(uint64_t min, uint64_t max) {
  return (min+max)/2;
}

constexpr bool has_factor(uint64_t n, uint64_t min, uint64_t max) {
  return min*min > n ? false :
    min + 1 >= max ? n % min == 0 :
    has_factor(n, min, med(min,max)) || has_factor(n, med(min,max), max);
}

constexpr uint64_t next_prime(uint64_t n) {
  return has_factor(n, 2, n) ? next_prime(n+1) : n;
}

template<unsigned int key_size, unsigned int value_size, unsigned int log_size>
class TranspositionTable {
private:
  static_assert(key_size <= 64);
  static_assert(value_size <= 64);
  static_assert(log_size < 31);
  
  template<int S> using uint_t =
    typename std::conditional<S <= 8, uint_least8_t,
    typename std::conditional<S <= 16, uint_least16_t,
    typename std::conditional<S <= 32, uint_least32_t,
    uint_least64_t>::type>::type>::type;

  using key_t = uint_t<key_size - log_size>;
  using value_t = uint_t<value_size>;

  static constexpr size_t size = next_prime(uint64_t{1} << log_size);

  key_t* K;
  value_t* V;

  size_t index(uint64_t key) const {
      return key % size;
  }

public:
  TranspositionTable(): K(new key_t[size]{}), V(new value_t[size]{}){}

  ~TranspositionTable() {
    delete[] K;
    delete[] V;
  }

  TranspositionTable(const TranspositionTable&) = delete;
  TranspositionTable& operator=(const TranspositionTable&) = delete;

  void reset() {
    std::memset(K, 0, size * sizeof(key_t));
    std::memset(V, 0, size * sizeof(value_t));
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