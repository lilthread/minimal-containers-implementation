#pragma once
#include <array>
#include <cstddef>
#include <sys/types.h>
#include <type_traits>

namespace not_std {
template <class event, std::size_t capacity>
class RingBuffer final {
public:
  constexpr RingBuffer() noexcept = default;
  void push(event e) noexcept {
    _events[_head]  = e;
    _head = (_head + 1) % capacity;
    if (_size < capacity)
      _size++;
    else {
      _tail = (_tail + 1) % capacity;
    }
  }
  std::array<event, capacity> snapshot() noexcept { 
    std::array<event, capacity> temp;
    for (auto i{0uz}; i < capacity; i++) {
      temp[i] = _events[_tail];
      _tail = (_tail + 1) % capacity;
    }
    return temp;
  }
private:
  std::array<event, capacity> _events{};
  std::size_t _head{0};
  std::size_t _size{0};
  std::size_t _tail{0};
};
}
