// Copyright (c) 2016-2024 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_OUTPUT_BASIS_HPP
#define KTH_DOMAIN_CHAIN_OUTPUT_BASIS_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <kth/domain/chain/script.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/multi_crypto_settings.hpp>
#include <kth/domain/chain/token_data.hpp>

#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::chain {

struct KD_API output_basis {
    using list = std::vector<output_basis>;


    // This is a sentinel used in .value to indicate not found in store.
    // This is a sentinel used in cache.value to indicate not populated.
    // This is a consensus value used in script::generate_signature_hash.
    // This is a consensus critical value that must be set on reset.
    static constexpr uint64_t const not_found = sighash_null_value;


    // Constructors.
    //-------------------------------------------------------------------------

    output_basis() = default;
    output_basis(uint64_t value, chain::script const& script, token_data_opt const& token_data);
    output_basis(uint64_t value, chain::script&& script, token_data_opt&& token_data);

    // Operators.
    //-------------------------------------------------------------------------
    // bool operator==(output_basis const& x) const;
    // bool operator!=(output_basis const& x) const;
    friend
    auto operator<=>(output_basis const&, output_basis const&) = default;

    // Deserialization.
    //-------------------------------------------------------------------------

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, bool /*wire*/ = true, bool /*witness*/ = false) {
        reset();

        value_ = source.read_8_bytes_little_endian();

        auto script_size = source.read_size_little_endian();
        if ( ! source) {
            reset();
            return source;
        }

        auto const has_token_data = source.peek_byte() == chain::encoding::PREFIX_BYTE;
        if ( ! source) {
            reset();
            return source;
        }

        if (has_token_data) {
            source.skip(1); // skip prefix byte
            chain::encoding::from_data(source, token_data_);
            script_size -= chain::encoding::serialized_size(token_data_);
            script_size -= 1; // prefix byte
        }

        script_.from_data_with_size(source, script_size);

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

        if ( ! token_data_.has_value()) {
            script_.to_data(sink, true);
            return;
        }

        auto const size = chain::encoding::serialized_size(token_data_) + script_.serialized_size(false) + 1;
        sink.write_variable_little_endian(size);

        sink.write_byte(chain::encoding::PREFIX_BYTE);
        chain::encoding::to_data(sink, token_data_.value());
        script_.to_data(sink, false);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    [[nodiscard]]
    size_t serialized_size(bool wire = true) const;

    [[nodiscard]]
    uint64_t value() const;

    void set_value(uint64_t value);

    // [[deprecated]] // unsafe
    chain::script& script();

    [[nodiscard]]
    chain::script const& script() const;

    void set_script(chain::script const& value);
    void set_script(chain::script&& value);


    [[nodiscard]]
    token_data_opt const& token_data() const;

    void set_token_data(token_data_opt const& value);
    void set_token_data(token_data_opt&& value);

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
    token_data_opt token_data_;
};

} // namespace kth::domain::chain

#endif // KTH_DOMAIN_CHAIN_OUTPUT_BASIS_HPP