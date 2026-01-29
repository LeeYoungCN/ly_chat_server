#ifndef COMMON_CONTAINER_BLOCKING_QUEUE_HPP
#define COMMON_CONTAINER_BLOCKING_QUEUE_HPP

#include <cstddef>
#include <format>
#include <stdexcept>
#include <utility>
#include <vector>
#include "common/debug/debug_log.h"

namespace common::container {
template <typename T>
class BlockingQueue {
public:
    BlockingQueue() : _container(_capacity_it) {}

    explicit BlockingQueue(size_t capacity) : _capacity(capacity), _capacity_it(capacity + 1), _container(_capacity_it)
    {
        if (capacity == 0) {
            throw std::invalid_argument("capacity is 0.");
        }
    }

    bool enqueue(const T& item)
    {
        if (full()) {
            return false;
        }

        _container[_tail] = item;
        _tail = (_tail + 1) % _capacity_it;
        return true;
    }

    bool enqueue(T&& item)
    {
        if (full()) {
            return false;
        }

        _container[_tail] = std::move(item);
        _tail = (_tail + 1) % _capacity_it;
        return true;
    }

    void enqueue_overrun(const T& item)
    {
        _container[_tail] = item;
        _tail = (_tail + 1) % _capacity_it;

        if (_head == _tail) {
            _head = (_head + 1) % _capacity_it;
            ++_overrun_counter;
        }
    }

    void enqueue_overrun(T&& item)
    {
        _container[_tail] = std::move(item);
        _tail = (_tail + 1) % _capacity_it;

        if (_head == _tail) {
            _head = (_head + 1) % _capacity_it;
            ++_overrun_counter;
        }
    }

    bool dequeue(T& item)
    {
        if (empty()) {
            return false;
        }
        item = std::move(_container[_head]);
        _head = (_head + 1) % _capacity_it;
        return true;
    }

    bool dequeue()
    {
        if (empty()) {
            return false;
        }

        _head = (_head + 1) % _capacity_it;
        return true;
    }

    bool front(T& item)
    {
        if (empty()) {
            return false;
        }
        item = _container[_head];

        return true;
    }

    const T& operator[](size_t idx) const
    {
        throw_if_out_of_range(idx);
        return _container.at((_head + idx) % _capacity_it);
    }

    const T& at(size_t idx) const
    {
        throw_if_out_of_range(idx);
        return _container[(_head + idx) % _capacity_it];
    }

    void clear()
    {
        _head = 0;
        _tail = 0;
        _overrun_counter = 0;
    }

    void throw_if_out_of_range(size_t idx) const
    {
        if (idx >= size()) {
            std::string errmsg = std::format("Out of range. idx: {}, queue size: {}.", idx, size());
            DEBUG_LOG_ERR(errmsg.c_str());
            throw std::out_of_range(errmsg);
        }
    }

    [[nodiscard]] size_t size() const { return _tail >= _head ? _tail - _head : _capacity_it - (_head - _tail); }
    [[nodiscard]] size_t capacity() const { return _capacity; }

    [[nodiscard]] bool empty() const { return _head == _tail; }

    [[nodiscard]] bool full() const { return ((_tail + 1) % _capacity_it) == _head; }

    [[nodiscard]] size_t overrun_counter() const { return _overrun_counter; }

private:
    const size_t _capacity = 1024;
    const size_t _capacity_it = _capacity + 1;
    std::vector<T> _container;
    size_t _head = 0;
    size_t _tail = 0;
    size_t _overrun_counter = 0;
};
}  // namespace common::container

#endif  // COMMON_CONTAINER_CIRCULAR_QUEUE_HPP
