// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_OUTPUT_BASIS_HPP
#define KTH_DOMAIN_CHAIN_OUTPUT_BASIS_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <kth/domain/chain/script.hpp>
#include <kth/domain/define.hpp>
// #include <kth/domain/wallet/payment_address.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::chain {

class KD_API output_basis {
public:
    using list = std::vector<output_basis>;

    /// This is a sentinel used in .value to indicate not found in store.
    /// This is a sentinel used in cache.value to indicate not populated.
    /// This is a consensus value used in script::generate_signature_hash.
    static
    uint64_t const not_found;

    // Constructors.
    //-------------------------------------------------------------------------

    output_basis() = default;
    output_basis(uint64_t value, chain::script const& script);
    output_basis(uint64_t value, chain::script&& script);

    // Operators.
    //-------------------------------------------------------------------------
    bool operator==(output_basis const& x) const;
    bool operator!=(output_basis const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    // bool from_data(data_chunk const& data, bool wire = true);
    // bool from_data(std::istream& stream, bool wire = true);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, bool /*wire*/ = true, bool /*witness*/ = false) {
        reset();

        value_ = source.read_8_bytes_little_endian();
        script_.from_data(source, true);

        if ( ! source) {
            reset();
        }

        return source;
    }

    [[nodiscard]]
    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    [[nodiscard]]
    data_chunk to_data(bool wire = true) const;
    
    void to_data(data_sink& stream, bool wire = true) const;

    template <typename W>
    void to_data(W& sink, bool /*wire*/ = true, bool /*witness*/ = false) const {
        sink.write_8_bytes_little_endian(value_);
        script_.to_data(sink, true);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    [[nodiscard]]
    size_t serialized_size(bool wire = true) const;

    [[nodiscard]]
    uint64_t value() const;
    
    void set_value(uint64_t value);

    // Deprecated (unsafe).
    chain::script& script();
    
    [[nodiscard]]
    chain::script const& script() const;
    
    void set_script(chain::script const& value);
    void set_script(chain::script&& value);

    // Validation.
    //-------------------------------------------------------------------------

    [[nodiscard]]
    size_t signature_operations(bool bip141) const;
    
    [[nodiscard]]
    bool is_dust(uint64_t minimum_output_value) const;

#if defined(KTH_SEGWIT_ENABLED)    
    bool extract_committed_hash(hash_digest& out) const;
#endif

// protected:
    void reset();

private:
    uint64_t value_{not_found};
    chain::script script_;
};

}  // namespace kth::chain

//#include <kth/domain/concepts_undef.hpp>

#endif // KTH_CHAIN_OUTPUT_BASIS_HPP_