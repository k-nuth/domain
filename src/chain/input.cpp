// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/input.hpp>

#include <algorithm>
#include <sstream>

#include <kth/domain/chain/script.hpp>

#if defined(KTH_SEGWIT_ENABLED)
#include <kth/domain/chain/witness.hpp>
#endif

#include <kth/domain/constants.hpp>
#include <kth/domain/wallet/payment_address.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::domain::chain {

using namespace kth::domain::wallet;
using namespace kth::domain::machine;

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

// #if ! defined(KTH_SEGWIT_ENABLED)
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

// #if ! defined(KTH_SEGWIT_ENABLED)
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
    input_basis::operator=(std::move(static_cast<input_basis&&>(x)));
    addresses_ = x.addresses_cache();
    return *this;
}

// bool input::operator==(input const& x) const {
//     return (sequence_ == x.sequence_) 
//         && (previous_output_ == x.previous_output_) 
//         && (script_ == x.script_) 
// #if defined(KTH_SEGWIT_ENABLED)
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

#if defined(KTH_SEGWIT_ENABLED)
void input::set_witness(chain::witness const& value) {
    input_basis::set_witness(value);
    invalidate_cache();
}

void input::set_witness(chain::witness&& value) {
    input_basis::set_witness(std::move(value));
    invalidate_cache();
}
#endif // KTH_CURRENCY_BCH

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

        // TODO(legacy): expand to include segregated witness address extraction.
        addresses_ = std::make_shared<payment_address::list>(payment_address::extract_input(script()));
        mutex_.unlock_and_lock_upgrade();
        //---------------------------------------------------------------------
    }

    auto addresses = *addresses_;
    mutex_.unlock_upgrade();
    ///////////////////////////////////////////////////////////////////////////

    return addresses;
}

} // namespace kth::domain::chain
