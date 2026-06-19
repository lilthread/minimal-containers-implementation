#pragma once
#include <cassert>
#include <cstddef>
#include <cstring>
#include <assert.h>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace not_std {
// IMPORTANT:
    template <class T> requires std::is_trivially_copyable_v<T>
class Vector final {
  using iterator = T*;
  using const_iterator = T const *;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
public:
  ~Vector() noexcept { delete[] _ptr; }
  constexpr Vector() = default;
  Vector(Vector&& other) noexcept {
    _ptr = std::exchange(other._ptr, nullptr);
    _capacity = std::exchange(other._capacity, 0);
    _length = std::exchange(other._length, 0);
  }
  Vector(const Vector& other) noexcept {
    _ptr = new T[other._capacity];
    std::copy(other._ptr, other._ptr + other._length, _ptr);
    _capacity = other._capacity;
    _length = other._length;
  }
  Vector &operator=(Vector&& other) noexcept {
    if (this == &other)
      return *this;
    _ptr = std::exchange(other._ptr, nullptr);
    _capacity = std::exchange(other._capacity, 0);
    _length = std::exchange(other._length, 0);
    return *this;
  }
  Vector &operator=(const Vector& other) noexcept {
    if (this == &other)
      return *this;
    _ptr = new T[other._capacity];
    std::copy(other._ptr, other._ptr + other._length, _ptr);
    _capacity = other._capacity;
    _length = other._length;
    return *this;
  }
  // Element access 
  T& front() { return _ptr[0]; } // UB if empty
  T const& front() const { return _ptr[0]; } // UB if empty
  T& back() { return _ptr[_length - 1]; }
  T const& back() const { return _ptr[_length]; }

  T& operator[](std::size_t idx) { return _ptr[idx]; }
  T const& operator[](std::size_t idx) const { return _ptr[idx]; }

  T& at(std::size_t idx) {
    if (idx >= _length)
      throw std::out_of_range("");
    return _ptr[idx];
  }
  T const& at(std::size_t idx) const {
    if (idx >= _length)
      throw std::out_of_range("");
    return _ptr[idx];
  }
  T* data() { return _ptr; }

  // Modifiers 
  void clear() {
    _length = 0;
  }
  iterator insert(iterator pos, const T& value) {
    std::size_t index = pos - _ptr;

    if (invalid_capacity()) {
      reallocate(_capacity * 3);
      pos = _ptr + index;
    }

    for (auto i = _length; i > index; i--)
      _ptr[i] = std::move(_ptr[i - 1]);

    _ptr[index] = value;
    _length++;

    return _ptr + index;
  }
  void erase(iterator it) {
    for (; it != end(); it++) {
      auto it_temp = it + 1;
      *it = std::move(*it_temp);
    }
    _length--;
  }
  void push_back(T obj) {
    if (invalid_capacity())
      reallocate(_capacity * 3);
    _ptr[_length++] = obj;
  }

  // Iterators 
  iterator begin() noexcept { return _ptr; }
  const_iterator cbegin() const noexcept { return _ptr; }
  iterator end() noexcept { return _ptr + _length; }
  const_iterator cend() const noexcept { return _ptr + _length; }
  reverse_iterator rbegin() { return reverse_iterator(_ptr + _length); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator(_ptr + _length); }
  reverse_iterator rend() { return reverse_iterator(_ptr); }
  const_reverse_iterator crend() const { return const_reverse_iterator(_ptr); }
  // Capacity
  bool empty() const noexcept { return _length == 0; }
  std::size_t size() const noexcept { return _length; }
  void reserve() {}
  std::size_t capacity() const noexcept { return _capacity; }
  friend bool operator==(const Vector& lhs, const Vector& rhs) {
    if (lhs.size() != rhs.size())
      return false;

    for (std::size_t i = 0; i < lhs.size(); i++)
      if (!(lhs[i] == rhs[i]))
        return false;

    return true;
  }
  explicit operator bool() const noexcept { return _length != 0; }
private:
  T* _ptr{};
  std::size_t _capacity{};
  std::size_t _length{};

  void reallocate(size_t new_cap) {
    if (new_cap == 0)
      new_cap = 1;

    T* old = _ptr;

    T* new_buf = new T[new_cap * 3];

    std::memcpy(new_buf, old, _length * sizeof(T));

    _ptr = new_buf;
    _capacity = new_cap;

    delete[] old;
  }
  bool invalid_capacity() { return _length >= _capacity; }
};

}
