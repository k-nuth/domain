// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_POINT_ITERATOR_HPP
#define KTH_CHAIN_POINT_ITERATOR_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>

#include <kth/domain/define.hpp>

namespace kth {
namespace chain {

class point;

/// A point iterator for store serialization (does not support wire).
class BC_API point_iterator {
public:
    using pointer = uint8_t;
    using reference = uint8_t;
    using value_type = uint8_t;
    using difference_type = ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    using iterator = point_iterator;
    using const_iterator = point_iterator;

    // Constructors.
    //-------------------------------------------------------------------------

    point_iterator(point const& value, unsigned index = 0);

    // point_iterator(point_iterator const& x) = default;
    // /// The iterator may only be assigned to another of the same point.
    // point_iterator& operator=(point_iterator const& x) = default;

    // Operators.
    //-------------------------------------------------------------------------

    operator bool() const;
    pointer operator->() const;
    reference operator*() const;
    point_iterator& operator++();
    point_iterator operator++(int);
    point_iterator& operator--();
    point_iterator operator--(int);
    point_iterator operator+(int value) const;
    point_iterator operator-(int value) const;
    bool operator==(point_iterator const& x) const;
    bool operator!=(point_iterator const& x) const;


protected:
    void increment();
    void decrement();
    point_iterator increase(unsigned value) const;
    point_iterator decrease(unsigned value) const;

private:
    uint8_t current() const;

    point const* point_;
    unsigned current_;
};

}  // namespace chain
}  // namespace kth

#endif
