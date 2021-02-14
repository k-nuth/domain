// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/get_data.hpp>

#include <algorithm>
#include <initializer_list>

#include <kth/domain/message/inventory.hpp>
#include <kth/domain/message/inventory_vector.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>

namespace kth::domain::message {

std::string const get_data::command = "getdata";
uint32_t const get_data::version_minimum = version::level::minimum;
uint32_t const get_data::version_maximum = version::level::maximum;

get_data::get_data(inventory_vector::list const& values)
    : inventory(values) {
}

get_data::get_data(inventory_vector::list&& values)
    : inventory(values) {
}

get_data::get_data(hash_list const& hashes, inventory::type_id type)
    : inventory(hashes, type) {
}

get_data::get_data(std::initializer_list<inventory_vector> const& values)
    : inventory(values) {
}

// get_data::get_data(get_data const& x)
//     : inventory(x) {
// }

// get_data::get_data(get_data&& x) noexcept
//     : inventory(x) 
// {}

// get_data& get_data::operator=(get_data&& x) noexcept {
//     set_inventories(x.inventories());
//     return *this;
// }

bool get_data::operator==(get_data const& x) const {
    return (static_cast<inventory>(*this) == static_cast<inventory>(x));
}

bool get_data::operator!=(get_data const& x) const {
    return (static_cast<inventory>(*this) != static_cast<inventory>(x));
}

#if defined(KTH_SEGWIT_ENABLED)
void get_data::to_witness() {
    auto const convert = [](inventory_vector& element) {
        element.to_witness();
    };

    std::for_each(inventories().begin(), inventories().end(), convert);
}
#endif

} // namespace kth::domain::message
