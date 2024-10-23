// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_INPUT_BASIS_HPP
#define KTH_DOMAIN_CHAIN_INPUT_BASIS_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <vector>

#include <kth/domain/chain/output_point.hpp>
#include <kth/domain/chain/script.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/multi_crypto_settings.hpp>

#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::chain {

class KD_API input_basis {
public:
    using list = std::vector<input_basis>;

    // Constructors.
    //-------------------------------------------------------------------------

    input_basis() = default;
    input_basis(output_point const& previous_output, chain::script const& script, uint32_t sequence);
    input_basis(output_point&& previous_output, chain::script&& script, uint32_t sequence);

    // Operators.
    //-------------------------------------------------------------------------

    // friend
    // auto operator<=>(input_basis const&, input_basis const&) = default;

    friend
    bool operator==(input_basis const&, input_basis const&) = default;

    friend
    bool operator!=(input_basis const&, input_basis const&) = default;

    // Deserialization.
    //-------------------------------------------------------------------------

    static
    expect<input_basis> from_data(byte_reader& reader, bool wire);

    // template <typename R, KTH_IS_READER(R)>
    // bool from_data(R& source, bool wire = true) {
    //     reset();

    //     if ( ! previous_output_.from_data(source, wire)) {
    //         return false;
    //     }

    //     script_.from_data(source, true);
    //     sequence_ = source.read_4_bytes_little_endian();

    //     if ( ! source) {
    //         reset();
    //     }

    //     return source;
    // }

    [[nodiscard]]
    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    [[nodiscard]]
    data_chunk to_data(bool wire = true) const;

    void to_data(data_sink& stream, bool wire = true) const;

    template <typename W>
    void to_data(W& sink, bool wire = true) const {
        previous_output_.to_data(sink, wire);
        script_.to_data(sink, true);
        sink.write_4_bytes_little_endian(sequence_);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    /// This accounts for wire, but does not read or write it.
    [[nodiscard]]
    size_t serialized_size(bool wire = true) const;


    output_point& previous_output();

    [[nodiscard]]
    output_point const& previous_output() const;

    void set_previous_output(output_point const& value);
    void set_previous_output(output_point&& value);

    // [[deprecated]] // unsafe
    chain::script& script();

    [[nodiscard]]
    chain::script const& script() const;

    void set_script(chain::script const& value);
    void set_script(chain::script&& value);

    [[nodiscard]]
    uint32_t sequence() const;

    void set_sequence(uint32_t value);


    // Validation.
    //-------------------------------------------------------------------------

    [[nodiscard]]
    bool is_final() const;

    [[nodiscard]]
    bool is_locked(size_t block_height, uint32_t median_time_past) const;

    [[nodiscard]]
    size_t signature_operations(bool bip16, bool bip141) const;

    bool extract_reserved_hash(hash_digest& out) const;
    // bool extract_embedded_script(chain::script& out) const;
    expect<chain::script> extract_embedded_script() const;

// protected:
    void reset();

private:
    output_point previous_output_;
    chain::script script_;
    uint32_t sequence_{0};
};

} // namespace kth::domain::chain

#endif // KTH_DOMAIN_CHAIN_INPUT_BASIS_HPP