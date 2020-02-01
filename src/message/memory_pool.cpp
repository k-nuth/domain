// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/bitcoin/message/memory_pool.hpp>

#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const memory_pool::command = "mempool";
uint32_t const memory_pool::version_minimum = version::level::bip35;
uint32_t const memory_pool::version_maximum = version::level::maximum;

memory_pool memory_pool::factory_from_data(uint32_t version, data_chunk const& data) {
    memory_pool instance;
    instance.from_data(version, data);
    return instance;
}

memory_pool memory_pool::factory_from_data(uint32_t version, std::istream& stream) {
    memory_pool instance;
    instance.from_data(version, stream);
    return instance;
}

// protected
memory_pool::memory_pool(bool insufficient_version)
    : insufficient_version_(insufficient_version) {
}

// memory_pool::memory_pool(const memory_pool& x)
//     : memory_pool(x.insufficient_version_) {
// }

// memory_pool::memory_pool(memory_pool&& x) noexcept
//     : memory_pool(x.insufficient_version_) 
// {}

bool memory_pool::is_valid() const {
    return !insufficient_version_;
}

// This is again a default instance so is invalid.
void memory_pool::reset() {
    insufficient_version_ = true;
}

bool memory_pool::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool memory_pool::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk memory_pool::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void memory_pool::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t memory_pool::serialized_size(uint32_t version) const {
    return memory_pool::satoshi_fixed_size(version);
}

size_t memory_pool::satoshi_fixed_size(uint32_t /*version*/) {
    return 0;
}

}  // namespace message
}  // namespace kth
