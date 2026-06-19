#pragma once
#include <atomic>
#include <cstddef>
#include <regex.h>
#include <utility>

namespace {
class ControlBlock final {
public:
  constexpr ControlBlock() noexcept = default;
  std::size_t count() const noexcept { return _count.load(std::memory_order_relaxed); }
  void increase() noexcept { _count.fetch_add(1, std::memory_order_relaxed); }
  bool decrease() noexcept { return _count.fetch_sub(1, std::memory_order_acq_rel) == 1; }
private:
  std::atomic<std::size_t> _count{1};
};
}

namespace not_std {

template <class T>
class SharedPtr final {
public:
  ~SharedPtr() noexcept { release(); }
  constexpr SharedPtr() noexcept = default;
  SharedPtr(T* obj) noexcept : _ptr(obj), _cb(obj ? new ControlBlock() : nullptr) {}

  SharedPtr(const SharedPtr& other) noexcept
  :  _ptr(other._ptr), _cb(other._cb) {
    if (_cb) _cb->increase();
  }

  SharedPtr& operator=(const SharedPtr& other) noexcept {
    if (this == &other) return *this;
    release();
    _cb = other._cb;
    _ptr = other._ptr;
    if (_cb) _cb->increase();
    return *this;
  }

  SharedPtr(SharedPtr&& other) noexcept
  : _ptr(std::exchange(other._ptr, nullptr)), _cb(std::exchange(other._cb, nullptr)) {}

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (this == &other) return *this;
    release();
    _cb = std::exchange(other._cb, nullptr);
    _ptr = std::exchange(other._ptr, nullptr);
    return *this;
  }

  T* get() const noexcept { return _ptr; }
  T& operator*() const noexcept { return *_ptr; }
  T* operator->() const noexcept { return _ptr; }
  void reset() noexcept {
    release();
    _ptr = nullptr;
    _cb = nullptr;
  }
  std::size_t use_count() const noexcept { return _cb ? _cb->count() : 0; }
  explicit operator bool() const noexcept { return _ptr != nullptr; }
  void swap(SharedPtr& other) noexcept {
    auto temp = _ptr;
    _ptr = other._ptr;
    other._ptr = temp;

    auto temp_cb = _cb;
    _cb = other._cb;
    other._cb = temp_cb;
  }
  friend void swap(SharedPtr& a, SharedPtr& b) noexcept {
    a.swap(b);
  }
  friend bool operator==(const SharedPtr& lhs, const SharedPtr& rhs) noexcept {
    return lhs._ptr == rhs._ptr;
  }
private:
  T* _ptr{};
  ControlBlock* _cb{};

  void release() noexcept {
    if (_cb && _cb->decrease()) {
      delete _ptr;
      delete _cb;
    }
  }
};

template <class T, class...Args>
SharedPtr<T> MakeShared(Args&&...args) noexcept {
  return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

// TODO: ARRAY IMPLEMENTATION
}
