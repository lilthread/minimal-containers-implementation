#pragma once
#include <any>
#include <array>
#include <cstddef>
#include <memory>
#include <new>

// NOT DONE
namespace not_std {
// FORWARD DECLARE 
template<class> class Any;
template <class U> inline U any_cast(const Any<U>&);
template <class T>
class Any final {
  using DataPtr = std::unique_ptr<void, void(*)(void*)>;
  static constexpr std::size_t SOO_SIZE = 64;
  static constexpr bool IS_SMALL_OBJ = sizeof(T) <= SOO_SIZE; 
public:
  ~Any() noexcept = default;
  constexpr Any() noexcept = default;
  constexpr Any(T obj) noexcept {
    if constexpr (IS_SMALL_OBJ) {
      T* ptr = std::construct_at(reinterpret_cast<T*>(_sooBuffer.data()), obj);
      _data = DataPtr(static_cast<void*>(ptr) , [](void* p) -> void {
        std::launder(static_cast<T*>(p))->~T();
      });
    } else {
      T* ptr = new T(obj);
      _data = DataPtr(static_cast<void*>(ptr), [](void* p) -> void {
        delete static_cast<T*>(p);
      });
    }
    _type = &typeid(T);
  }
  void reset() noexcept {
    _data.reset();
    _type = &typeid(void);
  }
  bool has_value() const noexcept { return _data != nullptr; }
  constexpr const std::type_info& type() const noexcept { return *_type; }
  template<class U>
  friend inline U any_cast(const Any<U>& op);
  template <class Value>
  Any& operator=(Value&& rhs) noexcept {
    _data.reset();
    _data = DataPtr{static_cast<void*>(rhs), [](void*){}};
    _type = &typeid(Value);
    return *this;
  }
  // TODO:
  Any& operator=(Any&& other) = delete;
  Any& operator=(const Any& other) = delete;
  Any(const Any& other) = delete;
  Any(Any&& other) = delete;
private:
  DataPtr _data{nullptr, [](void*){}};
  const std::type_info* _type = &typeid(void);
  alignas(std::max_align_t) std::array<std::byte, SOO_SIZE> _sooBuffer{};
};

template<class U>
inline U any_cast(const Any<U>& op) {
  if (op.type() != typeid(U))
    throw std::bad_any_cast{};
  return *static_cast<const U*>(op._data.get());
}
}

