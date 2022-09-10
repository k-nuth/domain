// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/chain/script_basis.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <numeric>
#include <ranges>
#include <sstream>
#include <utility>

// #include <string>
// #include <functional>

#include <boost/range/adaptor/reversed.hpp>


#include <kth/domain/chain/transaction.hpp>
#if defined(KTH_SEGWIT_ENABLED)
#include <kth/domain/chain/witness.hpp>
#endif

#include <kth/domain/common.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/machine/interpreter.hpp>
#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/machine/program.hpp>
#include <kth/domain/machine/rule_fork.hpp>
#include <kth/domain/multi_crypto_support.hpp>
#include <kth/infrastructure/error.hpp>
#include <kth/infrastructure/formats/base_16.hpp>
#include <kth/infrastructure/machine/script_pattern.hpp>
#include <kth/infrastructure/machine/script_version.hpp>
#include <kth/infrastructure/machine/sighash_algorithm.hpp>
#include <kth/infrastructure/math/elliptic_curve.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/assert.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>
#include <kth/infrastructure/utility/string.hpp>

namespace ranges = std::ranges;
namespace views = std::views;

using namespace kth::domain::machine;
using namespace kth::infrastructure::machine;
using namespace boost::adaptors;

namespace kth::domain::chain {

// bit.ly/2cPazSa
static
auto const one_hash = hash_literal("0000000000000000000000000000000000000000000000000000000000000001"); //NOLINT

// Constructors.
//-----------------------------------------------------------------------------

script_basis::script_basis(data_chunk&& encoded, bool prefix) {
    if (prefix) {
        valid_ = entity_from_data(*this, encoded, prefix);
        return;
    }

    // This is an optimization that avoids streaming the encoded bytes.
    bytes_ = std::move(encoded);
    valid_ = true;
}

script_basis::script_basis(data_chunk const& encoded, bool prefix) {
    valid_ = entity_from_data(*this, encoded, prefix);
}

// Operators.
//-----------------------------------------------------------------------------

bool script_basis::operator==(script_basis const& x) const {
    return bytes_ == x.bytes_;
}

bool script_basis::operator!=(script_basis const& x) const {
    return !(*this == x);
}

// Deserialization.
//-----------------------------------------------------------------------------

// Concurrent read/write is not supported, so no critical section.
// bool script_basis::from_string(std::string const& mnemonic) {
//     reset();

//     // There is strictly one operation per string token.
//     auto const tokens = split(mnemonic);
//     operation::list ops;
//     ops.resize(tokens.empty() || tokens.front().empty() ? 0 : tokens.size());

//     // Create an op list from the split tokens, one operation per token.
//     for (size_t index = 0; index < ops.size(); ++index) {
//         if ( ! ops[index].from_string(tokens[index])) {
//             return false;
//         }
//     }

//     from_operations(ops);
//     return true;
// }

bool script_basis::from_string(std::string const& mnemonic) {
    reset();

    // There is strictly one operation per string token.
    // auto const tokens = split(mnemonic);
    auto tokens = mnemonic | views::split(' ');

    operation::list ops;
    // ops.resize(tokens.empty() || tokens.front().empty() ? 0 : ranges::distance(tokens));
    if ( ! tokens.empty() && ! tokens.front().empty()) {
        // ops.resize(ranges::distance(tokens));

        // Create an op list from the split tokens, one operation per token.
        for (auto token : tokens) {
            operation op;
            std::string_view v(token.begin(), token.end());
            if ( ! op.from_string(v)) {
                return false;
            }
            ops.push_back(std::move(op));
        }

        // // Create an op list from the split tokens, one operation per token.
        // for (size_t i = 0; i < ops.size(); ++i) {
        //     if ( ! ops[i].from_string(tokens[i])) {
        //         return false;
        //     }
        // }
    }

    from_operations(ops);
    return true;
}

// Concurrent read/write is not supported, so no critical section.
void script_basis::from_operations(operation::list const& ops) {
    ////reset();
    bytes_ = operations_to_data(ops);
    valid_ = true;
}

// private/static
data_chunk script_basis::operations_to_data(operation::list const& ops) {
    data_chunk out;
    auto const size = serialized_size(ops);
    out.reserve(size);
    auto const concatenate = [&out](operation const& op) {
        auto bytes = op.to_data();
        std::move(bytes.begin(), bytes.end(), std::back_inserter(out));
    };

    std::for_each(ops.begin(), ops.end(), concatenate);
    KTH_ASSERT(out.size() == size);
    return out;
}

// private/static
size_t script_basis::serialized_size(operation::list const& ops) {
    auto const op_size = [](size_t total, operation const& op) {
        return total + op.serialized_size();
    };

    return std::accumulate(ops.begin(), ops.end(), size_t{0}, op_size);
}

// protected
// Concurrent read/write is not supported, so no critical section.
void script_basis::reset() {
    bytes_.clear();
    bytes_.shrink_to_fit();
    valid_ = false;
}

bool script_basis::is_valid() const {
    // All script bytes are valid under some circumstance (e.g. coinbase).
    // This returns false if a prefix and byte count does not match.
    return valid_;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk script_basis::to_data(bool prefix) const {
    data_chunk data;
    auto const size = serialized_size(prefix);
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream, prefix);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void script_basis::to_data(data_sink& stream, bool prefix) const {
    ostream_writer sink_w(stream);
    to_data(sink_w, prefix);
}

std::string script_basis::to_string(uint32_t active_forks) const {
    auto first = true;
    std::ostringstream text;

    for (auto const& op : operations(*this)) {
        text << (first ? "" : " ") << op.to_string(active_forks);
        first = false;
    }

    // An invalid operation has a specialized serialization.
    return text.str();
}

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

size_t script_basis::serialized_size(bool prefix) const {
    auto size = bytes_.size();

    if (prefix) {
        size += infrastructure::message::variable_uint_size(size);
    }

    return size;
}

data_chunk const& script_basis::bytes() const {
    return bytes_;
}

// Signing (unversioned).
//-----------------------------------------------------------------------------

inline
hash_digest signature_hash(transaction const& tx, uint32_t sighash_type) {
    // There is no rational interpretation of a signature hash for a coinbase.
    KTH_ASSERT( ! tx.is_coinbase());

    auto serialized = tx.to_data(true, false);
    extend_data(serialized, to_little_endian(sighash_type));
    return bitcoin_hash(serialized);
}

//*****************************************************************************
// CONSENSUS: Due to masking of bits 6/7 (8 is the anyone_can_pay flag),
// there are 4 possible 7 bit values that can set "single" and 4 others that
// can set none, and yet all other values set "all".
//*****************************************************************************
inline
sighash_algorithm to_sighash_enum(uint8_t sighash_type) {
    switch (sighash_type & sighash_algorithm::mask) {
        case sighash_algorithm::single:
            return sighash_algorithm::single;
        case sighash_algorithm::none:
            return sighash_algorithm::none;
        default:
            return sighash_algorithm::all;
    }
}

inline
uint8_t is_sighash_enum(uint8_t sighash_type, sighash_algorithm value) {
    return static_cast<uint8_t>(
        to_sighash_enum(sighash_type) == value
    );
}

static
size_t preimage_size(size_t script_size) {
    return sizeof(uint32_t) + hash_size + hash_size + point::satoshi_fixed_size() + script_size + sizeof(uint64_t) + sizeof(uint32_t) + hash_size + sizeof(uint32_t) + sizeof(uint32_t);
}

// private/static
hash_digest script_basis::generate_version_0_signature_hash(transaction const& tx,
                                                      uint32_t input_index,
                                                      script_basis const& script_code,
                                                      uint64_t value,
                                                      uint8_t sighash_type) {
    // Unlike unversioned algorithm this does not allow an invalid input index.
    KTH_ASSERT(input_index < tx.inputs().size());
    auto const& input = tx.inputs()[input_index];
    auto const size = preimage_size(script_code.serialized_size(true));

    data_chunk data;
    data.reserve(size);
    data_sink ostream(data);
    ostream_writer sink_w(ostream);

    // Flags derived from the signature hash byte.
    auto const sighash = to_sighash_enum(sighash_type);
    auto const any = (sighash_type & sighash_algorithm::anyone_can_pay) != 0;

#if defined(KTH_CURRENCY_BCH)
    auto const single = (sighash == sighash_algorithm::single || sighash == sighash_algorithm::cash_forkid_all);

    //Note(kth: Not used for the moment:
    // auto const none = (sighash == sighash_algorithm::none || sighash == sighash_algorithm::cash_forkid_all);

    auto const all = (sighash == sighash_algorithm::all || sighash == sighash_algorithm::cash_forkid_all);
#else
    auto const single = (sighash == sighash_algorithm::single);

    //Note(kth: Not used for the moment:
    // auto const none = (sighash == sighash_algorithm::none);

    auto const all = (sighash == sighash_algorithm::all);
#endif

    // 1. transaction version (4-byte little endian).
    sink_w.write_little_endian(tx.version());

    // 2. inpoints hash (32-byte hash).
    sink_w.write_hash( ! any ? tx.inpoints_hash() : null_hash);

    // 3. sequences hash (32-byte hash).
    sink_w.write_hash( ! any && all ? tx.sequences_hash() : null_hash);

    // 4. outpoint (32-byte hash + 4-byte little endian).
    input.previous_output().to_data(sink_w);

    // 5. script of the input (with prefix).
    script_code.to_data(sink_w, true);

    // 6. value of the output spent by this input (8-byte little endian).
    sink_w.write_little_endian(value);

    // 7. sequence of the input (4-byte little endian).
    sink_w.write_little_endian(input.sequence());

    // 8. outputs hash (32-byte hash).
    sink_w.write_hash(all ? tx.outputs_hash() : (single && input_index < tx.outputs().size() ? bitcoin_hash(tx.outputs()[input_index].to_data()) : null_hash));

    // 9. transaction locktime (4-byte little endian).
    sink_w.write_little_endian(tx.locktime());

    // 10. sighash type of the signature (4-byte [not 1] little endian).
    sink_w.write_4_bytes_little_endian(sighash_type);

    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return bitcoin_hash(data);
}

// Utilities (static).
//-----------------------------------------------------------------------------

bool script_basis::is_push_only(operation::list const& ops) {
    auto const push = [](operation const& op) {
        return op.is_push();
    };

    return std::all_of(ops.begin(), ops.end(), push);
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip16 validation rules.
//*****************************************************************************
bool script_basis::is_relaxed_push(operation::list const& ops) {
    auto const push = [&](operation const& op) {
        return op.is_relaxed_push();
    };

    return std::all_of(ops.begin(), ops.end(), push);
}

//*****************************************************************************
// CONSENSUS: BIP34 requires coinbase input script to begin with one byte that
// indicates the height size. This is inconsistent with an extreme future where
// the size byte overflows. However satoshi actually requires nominal encoding.
//*****************************************************************************
bool script_basis::is_coinbase_pattern(operation::list const& ops, size_t height) {
    return !ops.empty() && ops[0].is_nominal_push() && ops[0].data() == number(height).data();
}

//*****************************************************************************
// CONSENSUS: this pattern is used to commit to bip141 witness data.
//*****************************************************************************
#if defined(KTH_SEGWIT_ENABLED)
bool script_basis::is_commitment_pattern(operation::list const& ops) {
    static auto const header = to_big_endian(witness_head);

    // Bytes after commitment are optional with no consensus meaning (bip141).
    // Commitment is not executable so invalid trailing operations are allowed.
    return ops.size() > 1 && ops[0].code() == opcode::return_ && ops[1].code() == opcode::push_size_36 && std::equal(header.begin(), header.end(), ops[1].data().begin());
}
#endif

//*****************************************************************************
// CONSENSUS: this pattern is used in bip141 validation rules.
//*****************************************************************************
#if defined(KTH_SEGWIT_ENABLED)
bool script_basis::is_witness_program_pattern(operation::list const& ops) {
    return ops.size() == 2 && ops[0].is_version() && ops[1].data().size() >= min_witness_program && ops[1].data().size() <= max_witness_program;
}
#endif

// The satoshi client enables configurable data size for policy.
bool script_basis::is_null_data_pattern(operation::list const& ops) {
    return ops.size() == 2 && ops[0].code() == opcode::return_ && ops[1].is_minimal_push() && ops[1].data().size() <= max_null_data_size;
}

// TODO(legacy): expand this to the 20 signature op_check_multisig limit.
// The current 16 (or 20) limit does not affect server indexing because bare
// multisig is not indexable and p2sh multisig is byte-limited to 15 sigs.
// The satoshi client policy limit is 3 signatures for bare multisig.
bool script_basis::is_pay_multisig_pattern(operation::list const& ops) {
    static constexpr auto op_1 = static_cast<uint8_t>(opcode::push_positive_1);
    static constexpr auto op_16 = static_cast<uint8_t>(opcode::push_positive_16);

    auto const op_count = ops.size();

    if (op_count < 4 || ops[op_count - 1].code() != opcode::checkmultisig) {
        return false;
    }

    auto const op_m = static_cast<uint8_t>(ops[0].code());
    auto const op_n = static_cast<uint8_t>(ops[op_count - 2].code());

    if (op_m < op_1 || op_m > op_n || op_n < op_1 || op_n > op_16) {
        return false;
    }

    auto const number = op_n - op_1 + 1u;
    auto const points = op_count - 3u;

    if (number != points) {
        return false;
    }

    for (auto op = ops.begin() + 1; op != ops.end() - 2; ++op) {
        if ( ! is_public_key(op->data())) {
            return false;
        }
    }

    return true;
}

// The satoshi client considers this non-standard for policy.
bool script_basis::is_pay_public_key_pattern(operation::list const& ops) {
    return ops.size() == 2 && is_public_key(ops[0].data()) && ops[1].code() == opcode::checksig;
}

bool script_basis::is_pay_key_hash_pattern(operation::list const& ops) {
    return ops.size() == 5 && ops[0].code() == opcode::dup && ops[1].code() == opcode::hash160 && ops[2].data().size() == short_hash_size && ops[3].code() == opcode::equalverify && ops[4].code() == opcode::checksig;
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip16 validation rules.
//*****************************************************************************
bool script_basis::is_pay_script_hash_pattern(operation::list const& ops) {
    return ops.size() == 3 && ops[0].code() == opcode::hash160 && ops[1].code() == opcode::push_size_20 && ops[2].code() == opcode::equal;
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip141 validation rules.
//*****************************************************************************
#if defined(KTH_SEGWIT_ENABLED)
bool script_basis::is_pay_witness_script_hash_pattern(operation::list const& ops) {
    return ops.size() == 2 && ops[0].code() == opcode::push_size_0 && ops[1].code() == opcode::push_size_32;
}
#endif

// The first push is based on wacky satoshi op_check_multisig behavior that
// we must perpetuate, though it's appearance here is policy not consensus.
// Limiting to push_size_0 eliminates pattern ambiguity with little downside.
bool script_basis::is_sign_multisig_pattern(operation::list const& ops) {
    return ops.size() >= 2 && ops[0].code() == opcode::push_size_0 && std::all_of(ops.begin() + 1, ops.end(), [](operation const& op) { return is_endorsement(op.data()); });
}

bool script_basis::is_sign_public_key_pattern(operation::list const& ops) {
    return ops.size() == 1 && is_endorsement(ops[0].data());
}

//*****************************************************************************
// CONSENSUS: this pattern is used to activate bip141 validation rules.
//*****************************************************************************
bool script_basis::is_sign_key_hash_pattern(operation::list const& ops) {
    return ops.size() == 2 && is_endorsement(ops[0].data()) && is_public_key(ops[1].data());
}

// Ambiguous with is_sign_key_hash when second/last op is a public key.
// Ambiguous with is_sign_public_key_pattern when only op is an endorsement.
bool script_basis::is_sign_script_hash_pattern(operation::list const& ops) {
    return !ops.empty() && is_push_only(ops) && !ops.back().data().empty();
}

operation::list script_basis::to_null_data_pattern(data_slice data) {
    if (data.size() > max_null_data_size) {
        return {};
    }

    return operation::list{{opcode::return_},
                           {to_chunk(data)}};
}

operation::list script_basis::to_pay_public_key_pattern(data_slice point) {
    if ( ! is_public_key(point)) {
        return {};
    }

    return operation::list{{to_chunk(point)},
                           {opcode::checksig}};
}

operation::list script_basis::to_pay_key_hash_pattern(short_hash const& hash) {
    return operation::list{
        {opcode::dup},
        {opcode::hash160},
        {to_chunk(hash)},
        {opcode::equalverify},
        {opcode::checksig}};
}

operation::list script_basis::to_pay_script_hash_pattern(short_hash const& hash) {
    return operation::list{
        {opcode::hash160},
        {to_chunk(hash)},
        {opcode::equal}};
}

operation::list script_basis::to_pay_multisig_pattern(uint8_t signatures,
                                                point_list const& points) {
    data_stack chunks;
    chunks.reserve(points.size());
    auto const conversion = [&chunks](ec_compressed const& point) {
        chunks.push_back(to_chunk(point));
    };

    // Operation ordering matters, don't use std::transform here.
    std::for_each(points.begin(), points.end(), conversion);
    return to_pay_multisig_pattern(signatures, chunks);
}

// TODO(legacy): expand this to a 20 signature limit.
// This supports up to 16 signatures, however check_multisig is limited to 20.
// The embedded script is limited to 520 bytes, an effective limit of 15 for
// p2sh multisig, which can be as low as 7 when using all uncompressed keys.
operation::list script_basis::to_pay_multisig_pattern(uint8_t signatures, data_stack const& points) {
    static constexpr auto op_81 = static_cast<uint8_t>(opcode::push_positive_1);
    static constexpr auto op_96 = static_cast<uint8_t>(opcode::push_positive_16);
    static constexpr auto zero = op_81 - 1;
    static constexpr auto max = op_96 - zero;

    auto const m = signatures;
    auto const n = points.size();

    if (m < 1 || m > n || n < 1 || n > max) {
        return operation::list();
    }

    auto const op_m = static_cast<opcode>(m + zero);
    auto const op_n = static_cast<opcode>(points.size() + zero);

    operation::list ops;
    ops.reserve(points.size() + 3);
    ops.emplace_back(op_m);

    for (auto const& point : points) {
        if ( ! is_public_key(point)) {
            return {};
        }

        ops.emplace_back(point);
    }

    ops.emplace_back(op_n);
    ops.emplace_back(opcode::checkmultisig);
    return ops;
}

// Utilities (non-static).
//-----------------------------------------------------------------------------

// Count 1..16 multisig accurately for embedded (bip16) and witness (bip141).
inline size_t multisig_sigops(bool accurate, opcode code) {
    return accurate && operation::is_positive(code) ? operation::opcode_to_positive(code) : multisig_default_sigops;
}


//*****************************************************************************
// CONSENSUS: this is a pointless, broken, premature optimization attempt.
// The comparison and erase are not limited to a single operation and so can
// erase arbitrary upstream data from the script.
//*****************************************************************************
void script_basis::find_and_delete_(data_chunk const& endorsement) {
    // If this is empty it would produce an empty script but not operation.
    // So we test it for empty prior to operation reserialization.
    if (endorsement.empty()) {
        return;
    }

    // The value must be serialized to script using non-minimal encoding.
    // Non-minimally-encoded target values will therefore not match.
    auto const value = operation(endorsement, false).to_data();

    operation op;
    data_source stream(bytes_);
    istream_reader stream_r(stream);
    std::vector<data_chunk::iterator> found;

    // The exhaustion test handles stream end and op deserialization failure.
    for (auto it = bytes_.begin(); !stream_r.is_exhausted();
         it += stream_r ? op.serialized_size() : 0) {
        // Track all found values for later deletion.
        for (; starts_with(it, bytes_.end(), value); it += value.size()) {
            stream_r.skip(value.size());
            found.push_back(it);
        }

        // Read the next op code following last found value.
        op.from_data(stream_r);
    }

    // Delete any found values, reversed to prevent iterator invalidation.
    for (auto const it : reverse(found)) {
        bytes_.erase(it, it + value.size());
    }
}

// Concurrent read/write is not supported, so no critical section.
void script_basis::find_and_delete(data_stack const& endorsements) {
    for (auto const& endorsement : endorsements) {
        find_and_delete_(endorsement);
    }
    bytes_.shrink_to_fit();
}

// ------------------------------------------------------------------

operation::list operations(script_basis const& script) {
    data_source istream(script.bytes());
    istream_reader stream_r(istream);
    auto const size = script.bytes().size();

    operation::list res;
    // One operation per byte is the upper limit of operations.
    res.reserve(size);

    // ************************************************************************
    // CONSENSUS: In the case of a coinbase script we must parse the entire
    // script, beyond just the BIP34 requirements, so that sigops can be
    // calculated from the script. These are counted despite being irrelevant.
    // In this case an invalid script is parsed to the extent possible.
    // ************************************************************************

    // If an op fails it is pushed to operations and the loop terminates.
    // To validate the ops the caller must test the last op.is_valid(), or may
    // text script.is_valid_operations(), which is done in script validation.
    while ( ! stream_r.is_exhausted()) {
        res.push_back(create<operation>(stream_r));
    }

    res.shrink_to_fit();
    return res;
}

} // namespace kth::domain::chain
