// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
#include <bitcoin/bitcoin/chain/output_basis.hpp>
=======
#include <kth/domain/chain/output_basis.hpp>
>>>>>>> dev

#include <algorithm>
#include <cstdint>
#include <sstream>

#include <kth/domain/constants.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::chain {

// using namespace bc::wallet;

// This is a consensus critical value that must be set on reset.
uint64_t const output_basis::not_found = sighash_null_value;

// Constructors.
//-----------------------------------------------------------------------------

// output_basis::output_basis()
//     : value_(not_found)
// {}

// output_basis::output_basis(output_basis&& x) noexcept
//     : value_(x.value_),
//       script_(std::move(x.script_))
// {}

// output_basis::output_basis(output_basis const& x)
//     : value_(x.value_),
//       script_(x.script_)
// {}

output_basis::output_basis(uint64_t value, chain::script&& script)
    : value_(value),
      script_(std::move(script))
{}

output_basis::output_basis(uint64_t value, chain::script const& script)
    : value_(value),
      script_(script)
{}

// output_basis& output_basis::operator=(output_basis&& x) noexcept {
//     value_ = x.value_;
//     script_ = std::move(x.script_);
//     return *this;
// }

// output_basis& output_basis::operator=(output_basis const& x) {
//     value_ = x.value_;
//     script_ = x.script_;
//     return *this;
// }


// Operators.
//-----------------------------------------------------------------------------

bool output_basis::operator==(output_basis const& x) const {
    return (value_ == x.value_) && (script_ == x.script_);
}

bool output_basis::operator!=(output_basis const& x) const {
    return !(*this == x);
}

// Deserialization.
//-----------------------------------------------------------------------------

output_basis output_basis::factory_from_data(data_chunk const& data, bool wire) {
    output_basis instance;
    instance.from_data(data, wire);
    return instance;
}

output_basis output_basis::factory_from_data(std::istream& stream, bool wire) {
    output_basis instance;
    instance.from_data(stream, wire);
    return instance;
}

bool output_basis::from_data(data_chunk const& data, bool wire) {
    data_source istream(data);
    return from_data(istream, wire);
}

bool output_basis::from_data(std::istream& stream, bool wire) {
    istream_reader stream_r(stream);
    return from_data(stream_r, wire);
}

// protected
void output_basis::reset() {
    value_ = output_basis::not_found;
    script_.reset();
}

// Empty scripts are valid, validation relies on not_found only.
bool output_basis::is_valid() const {
    return value_ != output_basis::not_found;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk output_basis::to_data(bool wire) const {
    data_chunk data;
    auto const size = serialized_size(wire);
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream, wire);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void output_basis::to_data(data_sink& stream, bool wire) const {
    ostream_writer sink_w(stream);
    to_data(sink_w, wire);
}

// Size.
//-----------------------------------------------------------------------------

size_t output_basis::serialized_size(bool /*wire*/) const {
    return sizeof(value_) + script_.serialized_size(true);
}

// Accessors.
//-----------------------------------------------------------------------------

uint64_t output_basis::value() const {
    return value_;
}

void output_basis::set_value(uint64_t value) {
    value_ = value;
}

chain::script& output_basis::script() {
    return script_;
}

chain::script const& output_basis::script() const {
    return script_;
}

void output_basis::set_script(chain::script const& value) {
    script_ = value;
}

void output_basis::set_script(chain::script&& value) {
    script_ = std::move(value);
}


// Validation helpers.
//-----------------------------------------------------------------------------

size_t output_basis::signature_operations(bool bip141) const {
#ifdef KTH_CURRENCY_BCH
    bip141 = false;  // No segwit
#endif
    // Penalize quadratic signature operations (bip141).
    auto const sigops_factor = bip141 ? fast_sigops_factor : 1U;

    // Count heavy sigops in the output script.
    return script_.sigops(false) * sigops_factor;
}

bool output_basis::is_dust(uint64_t minimum_output_value) const {
    // If provably unspendable it does not expand the unspent output set.
    return value_ < minimum_output_value && !script_.is_unspendable();
}

bool output_basis::extract_committed_hash(hash_digest& out) const {
    auto const& ops = script_.operations();

    if ( ! script::is_commitment_pattern(ops)) {
        return false;
    }

    // The four byte offset for the witness commitment hash (bip141).
    auto const start = ops[1].data().begin() + sizeof(witness_head);
    std::copy_n(start, hash_size, out.begin());
    return true;
}

<<<<<<< HEAD
}  // namespace chain
=======
>>>>>>> dev
}  // namespace kth
