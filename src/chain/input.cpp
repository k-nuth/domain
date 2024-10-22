// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/input.hpp>

#include <algorithm>
#include <sstream>

#include <kth/domain/chain/script.hpp>
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

input::input(input_basis const& x)
    : input_basis(x)
{}

input::input(input_basis&& x) noexcept
    : input_basis(std::move(x))
{}

// Private cache access for copy/move construction.
input::addresses_ptr input::addresses_cache() const {
    ///////////////////////////////////////////////////////////////////////////
    // Critical Section
    shared_lock lock(mutex_);

    return addresses_;
    ///////////////////////////////////////////////////////////////////////////
}

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

void input::reset() {
    input_basis::reset();
    addresses_.reset();
}

// Deserialization.
//-----------------------------------------------------------------------------

// bool input::from_data_old(istream_reader& source, bool wire) {
//     reset();

//     if ( ! previous_output_.from_data_old(source, wire)) {
//         return false;
//     }

//     script_.from_data_old(source, true);
//     sequence_ = source.read_4_bytes_little_endian();

//     if ( ! source) {
//         reset();
//     }

//     return source;
// }

// // static
// expect<input> input::from_data(byte_reader& reader, bool wire) {
//     // return input_basis::from_data(reader, wire);


//    input old_object;
//     {
//         auto new_reader = reader;
//         auto tmp = new_reader.read_remaining_bytes();
//         if ( ! tmp) {
//             fmt::print("****** INPUT read_remaining_bytes ERROR *******\n");
//             std::terminate();
//         }
//         auto spn_bytes = *tmp;
//         data_chunk data(spn_bytes.begin(), spn_bytes.end());
//         data_source istream(data);
//         istream_reader source(istream);
//         old_object.from_data_old(source, wire);
//     }

//     auto basis = input_basis::from_data(reader, wire);
//     if ( ! basis) {
//         return make_unexpected(basis.error());
//     }

//     auto res = input(std::move(*basis));

//     std::string old_hex = encode_base16(old_object.to_data(wire));
//     std::string new_hex = encode_base16(res.to_data(wire));
//     if (old_hex != new_hex) {
//         fmt::print("****** INPUT MISMATCH *******\n");
//         fmt::print("old_hex: {}\n", old_hex);
//         fmt::print("new_hex: {}\n", new_hex);
//         std::terminate();
//     }

//     return res;
// }


// static
expect<input> input::from_data(byte_reader& reader, bool wire) {
    // return input_basis::from_data(reader, wire);
    auto basis = input_basis::from_data(reader, wire);
    if ( ! basis) {
        return make_unexpected(basis.error());
    }
    return input(std::move(*basis));
}


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

// protected
void input::invalidate_cache() const {
#if ! defined(__EMSCRIPTEN__)
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

#else
    {
        std::shared_lock lock(mutex_);
        if ( ! addresses_) {
            return;
        }
    }

    std::unique_lock lock(mutex_);
    addresses_.reset();
#endif
}

payment_address input::address() const {
    auto const value = addresses();
    return value.empty() ? payment_address{} : value.front();
}

payment_address::list input::addresses() const {
#if ! defined(__EMSCRIPTEN__)
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
#else
    {
        std::shared_lock lock(mutex_);

        if (addresses_) {
            return *addresses_;
        }
    }

    std::unique_lock lock(mutex_);
    if ( ! addresses_) {
        addresses_ = std::make_shared<payment_address::list>(payment_address::extract_input(script()));
    }

    auto addresses = *addresses_;
#endif

    return addresses;
}

} // namespace kth::domain::chain
