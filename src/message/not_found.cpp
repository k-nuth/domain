// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/bitcoin/message/not_found.hpp>

#include <initializer_list>

#include <bitcoin/bitcoin/message/inventory.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>

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

not_found not_found::factory_from_data(uint32_t version, std::istream& stream) {
    not_found instance;
    instance.from_data(version, stream);
    return instance;
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

// bool not_found::from_data(uint32_t version, data_chunk const& data) {
//     return inventory::from_data(version, data);
// }

// bool not_found::from_data(uint32_t version, std::istream& stream) {
//     return inventory::from_data(version, stream);
// }


bool not_found::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool not_found::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

}  // namespace message
}  // namespace kth
