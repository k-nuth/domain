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
#include <bitcoin/bitcoin/message/not_found.hpp>

#include <initializer_list>

#include <bitcoin/bitcoin/message/inventory.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>

namespace libbitcoin {
namespace message {

std::string const not_found::command = "notfound";
uint32_t const not_found::version_minimum = version::level::bip37;
uint32_t const not_found::version_maximum = version::level::maximum;

not_found not_found::factory_from_data(uint32_t version, data_chunk const& data) {
    not_found instance;
    instance.from_data(version, data);
    return instance;
}

not_found not_found::factory_from_data(uint32_t version, data_source& stream) {
    not_found instance;
    instance.from_data(version, stream);
    return instance;
}

not_found::not_found()
    : inventory() {
}

not_found::not_found(inventory_vector::list const& values)
    : inventory(values) {
}

not_found::not_found(inventory_vector::list&& values)
    : inventory(values) {
}

not_found::not_found(hash_list const& hashes, inventory::type_id type)
    : inventory(hashes, type) {
}

not_found::not_found(std::initializer_list<inventory_vector> const& values)
    : inventory(values) {
}

// not_found::not_found(not_found const& x)
//     : inventory(x) {
// }

// not_found::not_found(not_found&& x) noexcept
//     : inventory(x) 
// {}

// not_found& not_found::operator=(not_found&& x) noexcept {
//     set_inventories(x.inventories());
//     return *this;
// }

bool not_found::operator==(not_found const& x) const {
    return (static_cast<inventory>(*this) == static_cast<inventory>(x));
}

bool not_found::operator!=(not_found const& x) const {
    return (static_cast<inventory>(*this) != static_cast<inventory>(x));
}
bool not_found::from_data(uint32_t version, data_chunk const& data) {
    return inventory::from_data(version, data);
}

bool not_found::from_data(uint32_t version, data_source& stream) {
    return inventory::from_data(version, stream);
}

}  // namespace message
}  // namespace libbitcoin
