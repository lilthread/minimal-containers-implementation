#pragma once
#include <typeinfo>
#include <utility>

namespace not_std {
class Any final {
public:
  constexpr Any() = default;

  template<class T>
  Any(T value) {
    emplace<T>(std::move(value));
  }

  Any(const Any& other) {
    if (other._ops)
      other._ops->copy(other._ptr, *this);
  }

  Any(Any&& other) noexcept {
    if (other._ops)
      other._ops->move(other._ptr, *this);
  }

  ~Any() { reset(); }

  Any& operator=(Any other) noexcept {
    if (&other == this)
      return *this;
    swap(other);
    return *this;
  }

  template<class T>
  void emplace(T value) noexcept {
    reset();
    _ptr = new T(std::move(value));

    static constexpr Ops table {
      [](void* p) noexcept { delete static_cast<T*>(p); },

      [](const void* p, Any& dst) noexcept { dst.emplace(*static_cast<const T*>(p)); },

      [](void* p, Any& dst) noexcept {
        dst._ptr = std::exchange(p, nullptr);
        dst._ops = &table;
      },

      [] noexcept -> const std::type_info& { return typeid(T); }
    };
    _ops = &table;
  }

  void reset() noexcept {
    if (_ops)
      _ops->destroy(_ptr);
    _ptr = nullptr;
    _ops = nullptr;
  }
  constexpr bool has_value() const noexcept { return _ptr != nullptr; }

  constexpr const std::type_info& type() const noexcept {
    return _ops ? _ops->type() : typeid(void);
  }

  void swap(Any& other) noexcept {
    std::swap(_ptr, other._ptr);
    std::swap(_ops, other._ops);
  }
  friend void swap(Any& lhs, Any& rhs) noexcept { lhs.swap(rhs); }

  template<class T>
  friend T& any_cast(Any&);

  template<class T>
  friend const T& any_cast(const Any&);
private:
  struct Ops final {
    void (*destroy)(void*) noexcept;
    void (*copy)(const void*, Any&) noexcept;
    void (*move)(void*, Any&) noexcept;
    const std::type_info& (*type)() noexcept;
  };
  void* _ptr{};
  const Ops* _ops{};
};

template<class T>
T& any_cast(Any& a) {
  if (a.type() != typeid(T))
    throw std::bad_cast{};
  return *static_cast<T*>(a._ptr);
}

template<class T>
const T& any_cast(const Any& a) {
  if (a.type() != typeid(T))
    throw std::bad_cast{};
  return *static_cast<const T*>(a._ptr);
}
}
