// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/header.hpp>

#include <cstddef>
#include <cstdint>
#include <istream>
#include <utility>

#include <kth/domain/chain/header.hpp>
#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::domain::message {

std::string const header::command = "headers";
uint32_t const header::version_minimum = version::level::minimum;
uint32_t const header::version_maximum = version::level::maximum;

size_t header::satoshi_fixed_size(uint32_t version) {
    auto const canonical = (version == version::level::canonical);
    return chain::header::satoshi_fixed_size() + (canonical ? 0 : infrastructure::message::variable_uint_size(0));
}

header::header(uint32_t version,
               hash_digest const& previous_block_hash,
               hash_digest const& merkle,
               uint32_t timestamp,
               uint32_t bits,
               uint32_t nonce)
    : chain::header(version, previous_block_hash, merkle, timestamp, bits, nonce) {}

header::header(chain::header const& x)
    : chain::header(x) {
}

// header::header(header const& x)
//     : chain::header(x) {
// }

// header::header(header&& x) noexcept
//     : chain::header(std::move(x)) 
// {}

header& header::operator=(chain::header const& x) {
    chain::header::operator=(x);
    return *this;
}

// header& header::operator=(header&& x) noexcept {
//     chain::header::operator=(std::move(x));
//     return *this;
// }

header& header::operator=(header const& x) {
    chain::header::operator=(x);
    return *this;
}

bool header::operator==(chain::header const& x) const {
    return chain::header::operator==(x);
}

bool header::operator!=(chain::header const& x) const {
    return chain::header::operator!=(x);
}

bool header::operator==(header const& x) const {
    return chain::header::operator==(x);
}

bool header::operator!=(header const& x) const {
    return chain::header::operator!=(x);
}

bool header::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool header::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk header::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void header::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

void header::reset() {
    chain::header::reset();
}

size_t header::serialized_size(uint32_t version) const {
    return satoshi_fixed_size(version);
}

}  // namespace kth
