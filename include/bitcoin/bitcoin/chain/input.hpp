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
#ifndef LIBBITCOIN_CHAIN_INPUT_HPP
#define LIBBITCOIN_CHAIN_INPUT_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <vector>

#include <bitcoin/bitcoin/chain/input_basis.hpp>
#include <bitcoin/bitcoin/chain/output_point.hpp>
#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/chain/witness.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/wallet/payment_address.hpp>
#include <bitcoin/infrastructure/math/hash.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/thread.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace chain {

class BC_API input : public input_basis {
public:
    using list = std::vector<input>;

    // Constructors.
    //-------------------------------------------------------------------------

    input() = default;
    // input(output_point const& previous_output, chain::script const& script, uint32_t sequence);
    // input(output_point&& previous_output, chain::script&& script, uint32_t sequence);
    // input(output_point const& previous_output, chain::script const& script, chain::witness const& witness, uint32_t sequence);
    // input(output_point&& previous_output, chain::script&& script, chain::witness&& witness, uint32_t sequence);

    using input_basis::input_basis; // inherit constructors from input_basis

    input(input const& x);
    input(input&& x) noexcept;
    input& operator=(input&& x) noexcept;
    input& operator=(input const& x);

    // Operators.
    //-------------------------------------------------------------------------

    // bool operator==(input const& x) const;
    // bool operator!=(input const& x) const;


    bool is_valid() const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    void set_script(chain::script const& value);
    void set_script(chain::script&& value);

#ifndef BITPRIM_CURRENCY_BCH
    void set_witness(chain::witness const& value);
    void set_witness(chain::witness&& value);
#endif // BITPRIM_CURRENCY_BCH

    /// The first payment address extracted (may be invalid).
    wallet::payment_address address() const;

    /// The payment addresses extracted from this input as a standard script.
    wallet::payment_address::list addresses() const;

    // Utilities.
    //-------------------------------------------------------------------------

    // Validation.
    //-------------------------------------------------------------------------

    // bool is_final() const;
    // bool is_segregated() const;
    // bool is_locked(size_t block_height, uint32_t median_time_past) const;
    // size_t signature_operations(bool bip16, bool bip141) const;
    // bool extract_reserved_hash(hash_digest& out) const;
    // bool extract_embedded_script(chain::script& out) const;
    // bool extract_witness_script(chain::script& out, chain::script const& prevout) const;

// protected:
    void reset();
protected:
    void invalidate_cache() const;

private:
    using addresses_ptr = std::shared_ptr<wallet::payment_address::list>;
    addresses_ptr addresses_cache() const;
    mutable upgrade_mutex mutex_;
    mutable addresses_ptr addresses_;
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif