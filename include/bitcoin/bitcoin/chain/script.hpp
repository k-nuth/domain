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
#ifndef LIBBITCOIN_CHAIN_SCRIPT_HPP
#define LIBBITCOIN_CHAIN_SCRIPT_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include <type_traits>

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/machine/operation.hpp>
#include <bitcoin/infrastructure/error.hpp>
#include <bitcoin/infrastructure/machine/rule_fork.hpp>
#include <bitcoin/infrastructure/machine/script_pattern.hpp>
#include <bitcoin/infrastructure/machine/script_version.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/thread.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace chain {

class transaction;
class witness;

class BC_API script {
public:
    typedef machine::operation operation;
    typedef machine::rule_fork rule_fork;
    typedef machine::script_pattern script_pattern;
    typedef machine::script_version script_version;

    // Constructors.
    //-------------------------------------------------------------------------

    script();

    script(script const& x);
    script(script&& x);

    script(operation::list const& ops);
    script(operation::list&& ops);

    script(data_chunk const& encoded, bool prefix);
    script(data_chunk&& encoded, bool prefix);

    // Operators.
    //-------------------------------------------------------------------------

    /// This class is move assignable and copy assignable.
    script& operator=(script const& x);
    script& operator=(script&& x);

    bool operator==(script const& x) const;
    bool operator!=(script const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static script factory_from_data(data_chunk const& encoded, bool prefix);
    static script factory_from_data(data_source& stream, bool prefix);

    template <Reader R, BITPRIM_IS_READER(R)>
    static script factory_from_data(R& source, bool prefix) {
        script instance;
        instance.from_data(source, prefix);
        return instance;
    }

    //static script factory_from_data(reader& source, bool prefix);

    /// Deserialization invalidates the iterator.
    bool from_data(data_chunk const& encoded, bool prefix);
    bool from_data(data_source& stream, bool prefix);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(R& source, bool prefix) {
        static_assert(!std::is_same<R, data_chunk>::value, "");
        // static_assert(   std::is_same<R, data_chunk>::value, "");

        reset();
        valid_ = true;

        if (prefix) {
            auto const size = source.read_size_little_endian();

            // The max_script_size constant limits evaluation, but not all scripts
            // evaluate, so use max_block_size to guard memory allocation here.
            if (size > get_max_block_size())
                source.invalidate();
            else
                bytes_ = source.read_bytes(size);
        } else {
            bytes_ = source.read_bytes();
        }

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(reader& source, bool prefix);

    /// Deserialization invalidates the iterator.
    void from_operations(operation::list&& ops);
    void from_operations(operation::list const& ops);
    bool from_string(std::string const& mnemonic);

    /// A script object is valid if the byte count matches the prefix.
    bool is_valid() const;

    /// Script operations is valid if all push ops have the predicated size.
    bool is_valid_operations() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool prefix) const;
    void to_data(data_sink& stream, bool prefix) const;

    template <Writer W>
    void to_data(W& sink, bool prefix) const {
        // TODO(libbitcoin): optimize by always storing the prefixed serialization.
        if (prefix) {
            sink.write_variable_little_endian(serialized_size(false));
        }

        sink.write_bytes(bytes_);
    }

    //void to_data(writer& sink, bool prefix) const;

    std::string to_string(uint32_t active_forks) const;

    // Iteration.
    //-------------------------------------------------------------------------

    void clear();
    bool empty() const;
    size_t size() const;
    operation const& front() const;
    operation const& back() const;
    operation::iterator begin() const;
    operation::iterator end() const;
    operation const& operator[](size_t index) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool prefix) const;
    operation::list const& operations() const;

    // Signing.
    //-------------------------------------------------------------------------

    static hash_digest generate_signature_hash(transaction const& tx,
                                               uint32_t input_index,
                                               script const& script_code,
                                               uint8_t sighash_type,
                                               script_version version = script_version::unversioned,
                                               uint64_t value = max_uint64);

    static bool check_signature(ec_signature const& signature,
                                uint8_t sighash_type,
                                data_chunk const& public_key,
                                script const& script_code,
                                transaction const& tx,
                                uint32_t input_index,
                                script_version version = script_version::unversioned,
                                uint64_t value = max_uint64);

    static bool create_endorsement(endorsement& out, ec_secret const& secret, script const& prevout_script, transaction const& tx, uint32_t input_index, uint8_t sighash_type, script_version version = script_version::unversioned, uint64_t value = max_uint64);

    // Utilities (static).
    //-------------------------------------------------------------------------

    /// Transaction helpers.
    static hash_digest to_outputs(transaction const& tx);
    static hash_digest to_inpoints(transaction const& tx);
    static hash_digest to_sequences(transaction const& tx);

    /// Determine if the fork is enabled in the active forks set.
    static bool is_enabled(uint32_t active_forks, rule_fork fork) {
        return (fork & active_forks) != 0;
    }

    /// Consensus patterns.
    static bool is_push_only(operation::list const& ops);
    static bool is_relaxed_push(operation::list const& ops);
    static bool is_coinbase_pattern(operation::list const& ops, size_t height);
    static bool is_commitment_pattern(operation::list const& ops);
    static bool is_witness_program_pattern(operation::list const& ops);

    /// Common output patterns (psh and pwsh are also consensus).
    static bool is_null_data_pattern(operation::list const& ops);
    static bool is_pay_multisig_pattern(operation::list const& ops);
    static bool is_pay_public_key_pattern(operation::list const& ops);
    static bool is_pay_key_hash_pattern(operation::list const& ops);
    static bool is_pay_script_hash_pattern(operation::list const& ops);
    static bool is_pay_witness_script_hash_pattern(operation::list const& ops);

    /// Common input patterns (skh is also consensus).
    static bool is_sign_multisig_pattern(operation::list const& ops);
    static bool is_sign_public_key_pattern(operation::list const& ops);
    static bool is_sign_key_hash_pattern(operation::list const& ops);
    static bool is_sign_script_hash_pattern(operation::list const& ops);

    /// Stack factories.
    static operation::list to_null_data_pattern(data_slice data);
    static operation::list to_pay_public_key_pattern(data_slice point);
    static operation::list to_pay_key_hash_pattern(short_hash const& hash);
    static operation::list to_pay_script_hash_pattern(short_hash const& hash);
    static operation::list to_pay_multisig_pattern(uint8_t signatures, const point_list& points);
    static operation::list to_pay_multisig_pattern(uint8_t signatures, data_stack const& points);

    // Utilities (non-static).
    //-------------------------------------------------------------------------

    /// Common pattern detection.
    data_chunk witness_program() const;
    script_version version() const;
    script_pattern pattern() const;
    script_pattern output_pattern() const;
    script_pattern input_pattern() const;

    /// Consensus computations.
    size_t sigops(bool accurate) const;
    void find_and_delete(data_stack const& endorsements);
    bool is_unspendable() const;

    // Validation.
    //-------------------------------------------------------------------------

    static code verify(transaction const& tx, uint32_t input, uint32_t forks);

    // TODO(libbitcoin): move back to private.
    static code verify(transaction const& tx, uint32_t input_index, uint32_t forks, script const& input_script, const witness& input_witness, script const& prevout_script, uint64_t value);

protected:
    // So that input and output may call reset from their own.
    friend class input;
    friend class output;

    void reset();
    bool is_pay_to_witness(uint32_t forks) const;
    bool is_pay_to_script_hash(uint32_t forks) const;

private:
    static size_t serialized_size(operation::list const& ops);
    static data_chunk operations_to_data(operation::list const& ops);
    static hash_digest generate_unversioned_signature_hash(
        transaction const& tx,
        uint32_t input_index,
        script const& script_code,
        uint8_t sighash_type);

    static hash_digest generate_version_0_signature_hash(transaction const& tx,
                                                         uint32_t input_index,
                                                         script const& script_code,
                                                         uint64_t value,
                                                         uint8_t sighash_type);

    void find_and_delete_(data_chunk const& endorsement);

    data_chunk bytes_;
    bool valid_;

    // These are protected by mutex.
    mutable bool cached_;
    mutable operation::list operations_;
    mutable upgrade_mutex mutex_;
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif
