// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/output_point.hpp>

#include <cstddef>
#include <cstdint>
#include <utility>

#include <kth/domain/chain/point.hpp>
#include <kth/domain/constants.hpp>

namespace kth {
namespace chain {

// Constructors.
//-----------------------------------------------------------------------------

output_point::output_point()
    : validation{} 
{}

output_point::output_point(hash_digest const& hash, uint32_t index)
    : point(hash, index), validation{} 
{}

output_point::output_point(point const& x)
    : point(x), validation{} 
{}

// Operators.
//-----------------------------------------------------------------------------

output_point& output_point::operator=(point const& x) {
    reset();
    point::operator=(x);
    return *this;
}

bool operator==(output_point const& x, point const& y) {
    return static_cast<point const&>(x) == y;
}

bool operator!=(output_point const& x, point const& y) {
    return !(x == y);
}

bool operator==(point const& x, output_point const& y) {
    return x == static_cast<point const&>(y);
}

bool operator!=(point const& x, output_point const& y) {
    return !(x == y);
}

bool operator==(output_point const& x, output_point const& y) {
    return static_cast<point const&>(x) == static_cast<point const&>(y);
}

bool operator!=(output_point const& x, output_point const& y) {
    return !(x == y);
}

// Deserialization.
//-----------------------------------------------------------------------------

output_point output_point::factory_from_data(data_chunk const& data, bool wire) {
    output_point instance;
    instance.from_data(data, wire);
    return instance;
}

output_point output_point::factory_from_data(std::istream& stream, bool wire) {
    output_point instance;
    instance.from_data(stream, wire);
    return instance;
}

// Validation.
//-----------------------------------------------------------------------------

// For tx pool validation height is that of the candidate block.
bool output_point::is_mature(size_t height) const {
    // Coinbase (null) inputs and those with non-coinbase prevouts are mature.
    if ( ! validation.coinbase || is_null()) {
        return true;
    }

    // The (non-coinbase) input refers to a coinbase output, so validate depth.
    return floor_subtract(height, validation.height) >= coinbase_maturity;
}

}  // namespace chain
}  // namespace kth
