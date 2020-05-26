// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_OUTPUT_HPP
#define KTH_DOMAIN_CHAIN_OUTPUT_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <kth/domain/chain/output_basis.hpp>
#include <kth/domain/chain/script.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/wallet/payment_address.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::chain {

class KD_API output : public output_basis {
public:
    using list = std::vector<output>;

    /// This is a sentinel used in .value to indicate not found in store.
    /// This is a sentinel used in cache.value to indicate not populated.
    /// This is a consensus value used in script::generate_signature_hash.
    static
    uint64_t const not_found;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation {
        /// This is a non-consensus sentinel indicating output is unspent.
        static
        uint32_t const not_spent;

        size_t spender_height = validation::not_spent;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    output() = default;
    // output(uint64_t value, chain::script const& script);
    // output(uint64_t value, chain::script&& script);

    using output_basis::output_basis;   //inherit constructors from output_basis
    
    output(output const& x);
    output(output&& x) noexcept;
    output& operator=(output const& x);
    output& operator=(output&& x) noexcept;

    // Operators.
    //-------------------------------------------------------------------------
    // bool operator==(output const& x) const;
    // bool operator!=(output const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static
    output factory_from_data(data_chunk const& data, bool wire = true);
    
    static
    output factory_from_data(std::istream& stream, bool wire = true);

    template <typename R, KTH_IS_READER(R)>
    static
    output factory_from_data(R& source, bool wire = true) {
        output instance;
        instance.from_data(source, wire);
        return instance;
    }

    bool from_data(data_chunk const& data, bool wire = true);
    bool from_data(std::istream& stream, bool wire = true);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, bool wire = true, bool witness = false) {
        if ( ! wire) {
            validation.spender_height = source.read_4_bytes_little_endian();
        }

        output_basis::from_data(source, wire, witness);
        return source;
    }

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool wire = true) const;
    void to_data(data_sink& stream, bool wire = true) const;

    template <typename W>
    void to_data(W& sink, bool wire = true, bool witness = false) const {
        if ( ! wire) {
            auto height32 = safe_unsigned<uint32_t>(validation.spender_height);
            sink.write_4_bytes_little_endian(height32);
        }

        output_basis::to_data(sink, wire, witness);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool wire = true) const;

    void set_script(chain::script const& value);
    void set_script(chain::script&& value);

    /// The payment address extracted from this output as a standard script.
    wallet::payment_address address(bool testnet = false) const;

    /// The first payment address extracted (may be invalid).
    wallet::payment_address address(uint8_t p2kh_version, uint8_t p2sh_version) const;

    /// The payment addresses extracted from this output as a standard script.
    wallet::payment_address::list addresses(
        uint8_t p2kh_version = wallet::payment_address::mainnet_p2kh,
        uint8_t p2sh_version = wallet::payment_address::mainnet_p2sh) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation validation{};

// protected:
    // void reset();
protected:
    void invalidate_cache() const;

private:
    using addresses_ptr = std::shared_ptr<wallet::payment_address::list>;
    addresses_ptr addresses_cache() const;
    mutable upgrade_mutex mutex_;
    mutable addresses_ptr addresses_;
};

}  // namespace kth::chain

//#include <kth/domain/concepts_undef.hpp>

#endif