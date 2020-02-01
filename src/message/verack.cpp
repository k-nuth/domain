// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <bitcoin/bitcoin/message/verack.hpp>

#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const verack::command = "verack";
uint32_t const verack::version_minimum = version::level::minimum;
uint32_t const verack::version_maximum = version::level::maximum;

verack verack::factory_from_data(uint32_t version, data_chunk const& data) {
    verack instance;
    instance.from_data(version, data);
    return instance;
}

verack verack::factory_from_data(uint32_t version, std::istream& stream) {
    verack instance;
    instance.from_data(version, stream);
    return instance;
}

bool verack::is_valid() const {
    return true;
}

void verack::reset() {
}

bool verack::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool verack::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk verack::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

//TODO(fernando): empty?
void verack::to_data(uint32_t /*version*/, data_sink& /*stream*/) const {
}

size_t verack::serialized_size(uint32_t version) const {
    return verack::satoshi_fixed_size(version);
}

size_t verack::satoshi_fixed_size(uint32_t /*version*/) {
    return 0;
}

}  // namespace message
}  // namespace kth
