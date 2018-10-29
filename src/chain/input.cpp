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
#include <bitcoin/bitcoin/chain/input.hpp>

#include <algorithm>
#include <sstream>

#include <bitcoin/bitcoin/chain/script.hpp>

#ifndef BITPRIM_CURRENCY_BCH
#include <bitcoin/bitcoin/chain/witness.hpp>
#endif

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace chain {

using namespace bc::wallet;
using namespace bc::machine;

// Constructors.
//-----------------------------------------------------------------------------

// input::input(output_point&& previous_output, chain::script&& script, uint32_t sequence)
//     : previous_output_(std::move(previous_output)),
//       script_(std::move(script)),
//       sequence_(sequence) {}

// input::input(output_point const& previous_output, chain::script const& script, uint32_t sequence)
//     : previous_output_(previous_output),
//       script_(script),
//       sequence_(sequence) {}

// Private cache access for copy/move construction.
input::addresses_ptr input::addresses_cache() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    shared_lock lock(mutex_);

    return addresses_;
    ///////////////////////////////////////////////////////////////////////////
}

// #ifdef BITPRIM_CURRENCY_BCH
// input::input(output_point const& previous_output, chain::script const& script, chain::witness const& /*witness*/, uint32_t sequence)
//     : previous_output_(previous_output)
//     , script_(script)
// #else
// input::input(output_point const& previous_output, chain::script const& script, chain::witness const& witness, uint32_t sequence)
//     : previous_output_(previous_output)
//     , script_(script)
//     , witness_(witness)
// #endif
//     , sequence_(sequence) 
// {}

// #ifdef BITPRIM_CURRENCY_BCH
// input::input(output_point&& previous_output, chain::script&& script, chain::witness&& /*witness*/, uint32_t sequence)
//     : previous_output_(std::move(previous_output))
//     , script_(std::move(script))
// #else
// input::input(output_point&& previous_output, chain::script&& script, chain::witness&& witness, uint32_t sequence)
//     : previous_output_(std::move(previous_output))
//     , script_(std::move(script))
//     , witness_(std::move(witness))
// #endif
//     , sequence_(sequence) 
// {}


input::input(input const& x)
    : input_basis(x)
    , addresses_(x.addresses_cache())
{}

input::input(input&& x) noexcept
    : input_basis(std::move(x))
    , addresses_(x.addresses_cache())
{}

// Operators.
//-----------------------------------------------------------------------------

input& input::operator=(input const& x) {
    input_basis::operator=(x);
    addresses_ = x.addresses_cache();
    return *this;
}

input& input::operator=(input&& x) noexcept {
    input_basis::operator=(std::move(x));
    addresses_ = x.addresses_cache();
    return *this;
}

// bool input::operator==(input const& x) const {
//     return (sequence_ == x.sequence_) 
//         && (previous_output_ == x.previous_output_) 
//         && (script_ == x.script_) 
// #ifndef BITPRIM_CURRENCY_BCH
//         && (witness_ == x.witness_)
// #endif
//         ;
// }

// bool input::operator!=(input const& x) const {
//     return !(*this == x);
// }

// Accessors.
//-----------------------------------------------------------------------------

void input::set_script(chain::script const& value) {
    input_basis::set_script(value);
    invalidate_cache();
}

void input::set_script(chain::script&& value) {
    input_basis::set_script(std::move(value));
    invalidate_cache();
}

#ifndef BITPRIM_CURRENCY_BCH
void input::set_witness(chain::witness const& value) {
    input_basis::set_witness(value);
    invalidate_cache();
}

void input::set_witness(chain::witness&& value) {
    input_basis::set_witness(std::move(value));
    invalidate_cache();
}
#endif // BITPRIM_CURRENCY_BCH

// protected
void input::invalidate_cache() const {
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

payment_address input::address() const {
    auto const value = addresses();
    return value.empty() ? payment_address{} : value.front();
}

payment_address::list input::addresses() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    mutex_.lock_upgrade();

    if ( ! addresses_) {
        //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        mutex_.unlock_upgrade_and_lock();

        // TODO(libbitcoin): expand to include segregated witness address extraction.
        addresses_ = std::make_shared<payment_address::list>(payment_address::extract_input(script()));
        mutex_.unlock_and_lock_upgrade();
        //---------------------------------------------------------------------
    }

    auto const addresses = *addresses_;
    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return addresses;
}

}  // namespace chain
}  // namespace libbitcoin
