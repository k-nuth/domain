// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_SCRIPT_HPP
#define KTH_CHAIN_SCRIPT_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <type_traits>

#include <kth/domain/chain/script_basis.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/machine/operation.hpp>
#include <kth/domain/machine/rule_fork.hpp>
#include <kth/infrastructure/error.hpp>
#include <kth/infrastructure/machine/script_pattern.hpp>
#include <kth/infrastructure/machine/script_version.hpp>
#include <kth/infrastructure/math/elliptic_curve.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/common.hpp>
#include <kth/domain/concepts.hpp>

namespace kth {
namespace chain {

class transaction;
class witness;

class BC_API script : public script_basis {
public:
    using operation = machine::operation;
    using rule_fork = machine::rule_fork;
    using script_pattern = machine::script_pattern;
    using script_version = machine::script_version;

    // Constructors.
    //-------------------------------------------------------------------------

    script() = default;

    script(operation::list const& ops);
    script(operation::list&& ops);
    script(data_chunk const& encoded, bool prefix);
    script(data_chunk&& encoded, bool prefix);


    script(script const& x);
    script(script&& x) noexcept;
    /// This class is move assignable and copy assignable.
    script& operator=(script const& x);
    script& operator=(script&& x) noexcept;

    // Operators.
    //-------------------------------------------------------------------------

    // bool operator==(script const& x) const;
    // bool operator!=(script const& x) const;


    // Deserialization.
    //-------------------------------------------------------------------------

    static script factory_from_data(data_chunk const& encoded, bool prefix);
    static script factory_from_data(std::istream& stream, bool prefix);

    template <Reader R, KTH_IS_READER(R)>
    static script factory_from_data(R& source, bool prefix) {
        script instance;
        instance.from_data(source, prefix);
        return instance;
    }

    /// Deserialization invalidates the iterator.
    void from_operations(operation::list&& ops);
    void from_operations(operation::list const& ops);
    bool from_string(std::string const& mnemonic);

    /// Script operations is valid if all push ops have the predicated size.
    bool is_valid_operations() const;

    // Serialization.
    //-------------------------------------------------------------------------

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

    // size_t serialized_size(bool prefix) const;
    using script_basis::serialized_size;
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
    // static hash_digest to_outputs(transaction const& tx);
    // static hash_digest to_inpoints(transaction const& tx);
    // static hash_digest to_sequences(transaction const& tx);

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
    static operation::list to_pay_multisig_pattern(uint8_t signatures, point_list const& points);
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

//     static code verify(transaction const& tx, uint32_t input, uint32_t forks);


//     // TODO(legacy): move back to private.
// #ifdef KTH_CURRENCY_BCH
//     static code verify(transaction const& tx, uint32_t input_index, uint32_t forks, script const& input_script, script const& prevout_script, uint64_t value);
// #else
//     static code verify(transaction const& tx, uint32_t input_index, uint32_t forks, script const& input_script, witness const& input_witness, script const& prevout_script, uint64_t value);
// #endif

// protected:
//     // So that input and output may call reset from their own.
//     friend class input;
//     friend class output;

    void reset();
    bool is_pay_to_witness(uint32_t forks) const;
    bool is_pay_to_script_hash(uint32_t forks) const;

private:
    static size_t serialized_size(operation::list const& ops);
    static data_chunk operations_to_data(operation::list const& ops);
    static hash_digest generate_unversioned_signature_hash(transaction const& tx, uint32_t input_index, script const& script_code, uint8_t sighash_type);

    static hash_digest generate_version_0_signature_hash(transaction const& tx,
                                                         uint32_t input_index,
                                                         script const& script_code,
                                                         uint64_t value,
                                                         uint8_t sighash_type);

    void find_and_delete_(data_chunk const& endorsement);

    // These are protected by mutex.
    mutable bool cached_{false};
    mutable operation::list operations_;
    mutable upgrade_mutex mutex_;
};

}  // namespace chain
}  // namespace kth

//#include <kth/domain/concepts_undef.hpp>

#endif
