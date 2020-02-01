// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_INPUT_HPP
#define KTH_CHAIN_INPUT_HPP

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

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

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

    static input factory_from_data(data_chunk const& data, bool wire = true, bool witness = false);
    static input factory_from_data(std::istream& stream, bool wire = true, bool witness = false);

    template <Reader R, KTH_IS_READER(R)>
    static input factory_from_data(R& source, bool wire = true, bool witness = false) {
        input instance;
        instance.from_data(source, wire, witness_val(witness));
        return instance;
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    void set_script(chain::script const& value);
    void set_script(chain::script&& value);

#ifndef KTH_CURRENCY_BCH
    void set_witness(chain::witness const& value);
    void set_witness(chain::witness&& value);
#endif // KTH_CURRENCY_BCH

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
}  // namespace kth

//#include <knuth/concepts_undef.hpp>

#endif