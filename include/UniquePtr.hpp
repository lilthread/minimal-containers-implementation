#pragma once
#include <cstddef>
#include <utility>

namespace not_std {
template <class T>
class UniquePtr final {
public:
  ~UniquePtr() { delete _ptr; }
  constexpr UniquePtr() noexcept = default;
  UniquePtr(T* ptr) noexcept : _ptr(ptr) {}

  UniquePtr(UniquePtr&& other) noexcept : _ptr(other._ptr) { other._ptr = nullptr; }
  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (this == &other)
      return *this;
    delete _ptr;

    _ptr = std::exchange(other._ptr, nullptr);
    return *this;
  }
  UniquePtr(const UniquePtr&) = delete;
  void operator=(const UniquePtr&) = delete;

  explicit operator bool() const noexcept { return _ptr != nullptr; }

  T* get() const noexcept { return _ptr; }
  T& operator*() const noexcept { return *_ptr; }
  T* operator->() const noexcept { return _ptr; }
  T* release() noexcept {
    auto* ptr = _ptr;
    _ptr = nullptr;
    return ptr;
  } 
  void reset(T* new_ptr = nullptr) noexcept {
    delete _ptr;
    _ptr = new_ptr;
  }
  void swap(UniquePtr& other) noexcept {
    auto temp = _ptr;
    _ptr = other._ptr;
    other._ptr = temp;
  }
  friend void swap(UniquePtr& a, UniquePtr& b) noexcept {
    a.swap(b);
  }
  friend bool operator==(const UniquePtr& lhs, const UniquePtr& rhs) noexcept {
    return lhs._ptr == rhs._ptr;
  }
private:
  T* _ptr{};
};

template <class T, class ...Args>
UniquePtr<T> MakeUnique(Args&& ...args) {
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
UniquePtr<T[]> MakeUniqueArray(std::size_t n) {
  return UniquePtr<T[]>(new T[n]());
}

template <class T>
class UniquePtr<T[]> final {
public:
  ~UniquePtr() { delete[] _ptr; }
  constexpr UniquePtr() noexcept = default;
  UniquePtr(T* ptr) noexcept : _ptr(ptr) {}

  UniquePtr(UniquePtr&& other) noexcept : _ptr(other._ptr) { other._ptr = nullptr; }
  UniquePtr& operator=(UniquePtr&& other) noexcept {
    if (this == &other)
      return *this;
    delete[] _ptr;
    _ptr = other._ptr;
    other._ptr = nullptr;
    return *this;
  }
  UniquePtr(const UniquePtr&) = delete;
  void operator=(const UniquePtr&) = delete;

  explicit operator bool() const noexcept { return _ptr != nullptr; }

  T* get() const noexcept { return _ptr; }
  T& operator[](std::size_t idx) const noexcept { return _ptr[idx]; }
  T* release() noexcept {
    auto* ptr = _ptr;
    _ptr = nullptr;
    return ptr;
  } 
  void reset(T* new_ptr = nullptr) noexcept {
    delete[] _ptr;
    _ptr = new_ptr;
  }
  void swap(UniquePtr& other) noexcept {
    auto temp = _ptr;
    _ptr = other._ptr;
    other._ptr = temp;
  }
  friend void swap(UniquePtr& a, UniquePtr& b) noexcept {
    a.swap(b);
  }
  friend bool operator==(const UniquePtr& lhs, const UniquePtr& rhs) noexcept {
    return lhs._ptr == rhs._ptr;
  }
private:
  T* _ptr{};
};

}

