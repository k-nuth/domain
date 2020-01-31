// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/get_address.hpp>

#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::message {

std::string const get_address::command = "getaddr";
uint32_t const get_address::version_minimum = version::level::minimum;
uint32_t const get_address::version_maximum = version::level::maximum;

get_address get_address::factory_from_data(uint32_t version, data_chunk const& data) {
    get_address instance;
    instance.from_data(version, data);
    return instance;
}

get_address get_address::factory_from_data(uint32_t version, std::istream& stream) {
    get_address instance;
    instance.from_data(version, stream);
    return instance;
}

bool get_address::is_valid() const {
    return true;
}

void get_address::reset() {
}

bool get_address::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool get_address::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk get_address::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void get_address::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t get_address::serialized_size(uint32_t version) const {
    return get_address::satoshi_fixed_size(version);
}

size_t get_address::satoshi_fixed_size(uint32_t /*version*/) {
    return 0;
}

}  // namespace kth
