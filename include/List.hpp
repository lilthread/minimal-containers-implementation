#pragma once
#include <cassert>
#include <cstddef>
#include <utility>
namespace not_std {
// TODO: INSERT(), ERASE() AND IMPLEMENT ITERATORS

template <class T>
class List final {
  struct Node {
    T data{};
    Node* next{};
    Node* prev{};
  };
public:
  ~List() noexcept { clear(); }
  constexpr explicit List() noexcept = default;
  constexpr void push_back(const T& value) noexcept {
    auto* node = new Node{.data=value, .next=nullptr, .prev=nullptr};
    if (!_tail) {
      _head = _tail = node;
    } else {
      node->prev = _tail;
      _tail->next= node;
      _tail = node;
    }
    _length++;
  }
  constexpr void push_front(const T& value) noexcept {
    auto* node = new Node{.data=value, .next=nullptr, .prev=nullptr};
    if (!_head) {
      _head = _tail = node;
    } else {
      node->next = _head;
      _head->prev = node;
      _head = node;
    }
    _length++;
  }
  constexpr void pop_front() noexcept {
    if (empty()) return;
    auto* temp = _head;
    _head = _head->next;
    if (_head)
      _head->next = nullptr;
    else
      _head = nullptr;
    delete temp;
    _length--;
  }
  constexpr void pop_back() noexcept {
    if (empty()) return;
    auto* temp = _tail;
    _tail = _tail->prev;
    if (_tail)
      _tail->next = nullptr;
    else
      _head = nullptr;
    delete  temp;
    _length--;
  }
  constexpr void clear() noexcept {
    while (_head) {
      Node* next = _head->next;
      delete _head;
      _head = next;
    }
    _tail = nullptr;
    _length = 0;
  }
  constexpr T& front() const noexcept { return _head->data; }
  constexpr T& back() const noexcept { return _tail->data; }
  constexpr std::size_t size() const noexcept { return _length; }
  constexpr bool empty() const noexcept { return 0 == _length; }
  constexpr void swap(List& list) noexcept {
    std::swap(_head, list._head);
    std::swap(_tail, list._tail);
    std::swap(_length, list._length);
  }
  friend constexpr void swap(List& lhs, List& rhs) noexcept { lhs.swap(rhs); }
private:
  Node* _head{};
  Node* _tail{};
  std::size_t _length{};
};
}
