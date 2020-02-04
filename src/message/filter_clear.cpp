// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/filter_clear.hpp>

#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::message {

std::string const filter_clear::command = "filterclear";
uint32_t const filter_clear::version_minimum = version::level::bip37;
uint32_t const filter_clear::version_maximum = version::level::maximum;

filter_clear filter_clear::factory_from_data(uint32_t version, data_chunk const& data) {
    filter_clear instance;
    instance.from_data(version, data);
    return instance;
}

filter_clear filter_clear::factory_from_data(uint32_t version, std::istream& stream) {
    filter_clear instance;
    instance.from_data(version, stream);
    return instance;
}

//filter_clear filter_clear::factory_from_data(uint32_t version, reader& source)
//{
//    filter_clear instance;
//    instance.from_data(version, source);
//    return instance;
//}


// protected
filter_clear::filter_clear(bool insufficient_version)
    : insufficient_version_(insufficient_version) {
}

// filter_clear::filter_clear(filter_clear const& x)
//     : filter_clear(x.insufficient_version_) {
// }

// filter_clear::filter_clear(filter_clear&& x) noexcept
//     : filter_clear(x.insufficient_version_) {
// }

bool filter_clear::is_valid() const {
    return !insufficient_version_;
}

// This is again a default instance so is invalid.
void filter_clear::reset() {
    insufficient_version_ = true;
}

bool filter_clear::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool filter_clear::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk filter_clear::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void filter_clear::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t filter_clear::serialized_size(uint32_t version) const {
    return filter_clear::satoshi_fixed_size(version);
}

size_t filter_clear::satoshi_fixed_size(uint32_t /*version*/) {
    return 0;
}

}  // namespace kth
