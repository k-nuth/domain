// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/output.hpp>

#include <algorithm>
#include <cstdint>
#include <sstream>

#include <kth/domain/constants.hpp>
#include <kth/domain/wallet/payment_address.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::chain {

using namespace bc::wallet;

// This is a consensus critical value that must be set on reset.
uint64_t const output::not_found = sighash_null_value;

// This is a non-consensus sentinel used to indicate an output is unspent.
uint32_t const output::validation::not_spent = max_uint32;

// Constructors.
//-----------------------------------------------------------------------------

// output::output()
//     : validation{} 
// {}

output::output(output const& x)
    : output_basis(x)
    , addresses_(x.addresses_cache())
    , validation(x.validation) 
{}

output::output(output&& x) noexcept
    : output_basis(std::move(x))
    , addresses_(x.addresses_cache())
    , validation(x.validation) 
{}

output& output::operator=(output const& x) {
    output_basis::operator=(x);
    addresses_ = x.addresses_cache();
    validation = x.validation;
    return *this;
}

output& output::operator=(output&& x) noexcept {
    output_basis::operator=(std::move(static_cast<output_basis&&>(x)));
    addresses_ = x.addresses_cache();
    validation = x.validation;
    return *this;
}

// output::output(uint64_t value, chain::script&& script)
//     : value_(value)
//     , script_(std::move(script))
//     , validation{} 
// {}

// output::output(uint64_t value, chain::script const& script)
//     : value_(value)
//     , script_(script)
//     , validation{} 
// {}

// Private cache access for copy/move construction.
output::addresses_ptr output::addresses_cache() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    shared_lock lock(mutex_);

    return addresses_;
    ///////////////////////////////////////////////////////////////////////////
}

// Operators.
//-----------------------------------------------------------------------------


// bool output::operator==(output const& x) const {
//     return (value_ == x.value_) && (script_ == x.script_);
// }

// bool output::operator!=(output const& x) const {
//     return !(*this == x);
// }

// Deserialization.
//-----------------------------------------------------------------------------

output output::factory_from_data(data_chunk const& data, bool wire) {
    output instance;
    instance.from_data(data, wire);
    return instance;
}

output output::factory_from_data(std::istream& stream, bool wire) {
    output instance;
    instance.from_data(stream, wire);
    return instance;
}

bool output::from_data(data_chunk const& data, bool wire) {
    data_source istream(data);
    return from_data(istream, wire);
}

bool output::from_data(std::istream& stream, bool wire) {
    istream_reader stream_r(stream);
    return from_data(stream_r, wire);
}

// // protected
// void output::reset() {
//     value_ = output::not_found;
//     script_.reset();
// }

// Empty scripts are valid, validation relies on not_found only.
// bool output::is_valid() const {
//     return value_ != output::not_found;
// }

// Serialization.
//-----------------------------------------------------------------------------

data_chunk output::to_data(bool wire) const {
    data_chunk data;
    auto const size = serialized_size(wire);
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream, wire);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void output::to_data(data_sink& stream, bool wire) const {
    ostream_writer sink_w(stream);
    to_data(sink_w, wire);
}

// Size.
//-----------------------------------------------------------------------------

size_t output::serialized_size(bool wire) const {
    // validation.spender_height is size_t stored as uint32_t.
    return (wire ? 0 : sizeof(uint32_t)) 
            + output_basis::serialized_size(wire);
}

// Accessors.
//-----------------------------------------------------------------------------

void output::set_script(chain::script const& value) {
    output_basis::set_script(value);
    invalidate_cache();
}

void output::set_script(chain::script&& value) {
    output_basis::set_script(std::move(value));
    invalidate_cache();
}

// protected
void output::invalidate_cache() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if (addresses_) {
        mutex_.unlock_upgrade_and_lock();
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        addresses_.reset();
        //---------------------------------------------------------------------
        mutex_.unlock_and_lock_upgrade();
    }

    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////
}
payment_address output::address(bool testnet /*= false*/) const {
    if (testnet) {
        return address(wallet::payment_address::testnet_p2kh, wallet::payment_address::testnet_p2sh);
    }
    return address(wallet::payment_address::mainnet_p2kh, wallet::payment_address::mainnet_p2sh);
}

payment_address output::address(uint8_t p2kh_version, uint8_t p2sh_version) const {
    auto const value = addresses(p2kh_version, p2sh_version);
    return value.empty() ? payment_address{} : value.front();
}

payment_address::list output::addresses(uint8_t p2kh_version, uint8_t p2sh_version) const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if ( ! addresses_) {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        mutex_.unlock_upgrade_and_lock();
        addresses_ = std::make_shared<payment_address::list>(payment_address::extract_output(script(), p2kh_version, p2sh_version));
        mutex_.unlock_and_lock_upgrade();
        //---------------------------------------------------------------------
    }

    auto const addresses = *addresses_;
    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return addresses;
}

}  // namespace kth
