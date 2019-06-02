/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/message/get_data.hpp>

#include <algorithm>
#include <initializer_list>

#include <bitcoin/bitcoin/message/inventory.hpp>
#include <bitcoin/bitcoin/message/inventory_vector.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>

namespace libbitcoin {
namespace message {

std::string const get_data::command = "getdata";
uint32_t const get_data::version_minimum = version::level::minimum;
uint32_t const get_data::version_maximum = version::level::maximum;

get_data get_data::factory_from_data(uint32_t version, data_chunk const& data) {
    get_data instance;
    instance.from_data(version, data);
    return instance;
}

get_data get_data::factory_from_data(uint32_t version, std::istream& stream) {
    get_data instance;
    instance.from_data(version, stream);
    return instance;
}

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

// bool get_data::from_data(uint32_t version, data_chunk const& data) {
//     return inventory::from_data(version, data);
// }

// bool get_data::from_data(uint32_t version, std::istream& stream) {
//     return inventory::from_data(version, stream);
// }

bool get_data::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool get_data::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

#ifndef KNUTH_CURRENCY_BCH
void get_data::to_witness() {
    auto const convert = [](inventory_vector& element) {
        element.to_witness();
    };

    std::for_each(inventories().begin(), inventories().end(), convert);
}
#endif

}  // namespace message
}  // namespace libbitcoin
