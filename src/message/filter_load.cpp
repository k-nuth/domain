// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/filter_load.hpp>

#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/assert.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth {
namespace message {

std::string const filter_load::command = "filterload";
uint32_t const filter_load::version_minimum = version::level::bip37;
uint32_t const filter_load::version_maximum = version::level::maximum;

filter_load filter_load::factory_from_data(uint32_t version, data_chunk const& data) {
    filter_load instance;
    instance.from_data(version, data);
    return instance;
}

filter_load filter_load::factory_from_data(uint32_t version, std::istream& stream) {
    filter_load instance;
    instance.from_data(version, stream);
    return instance;
}

filter_load::filter_load(data_chunk const& filter, uint32_t hash_functions, uint32_t tweak, uint8_t flags)
    : filter_(filter), hash_functions_(hash_functions), tweak_(tweak), flags_(flags) {
}

filter_load::filter_load(data_chunk&& filter, uint32_t hash_functions, uint32_t tweak, uint8_t flags)
    : filter_(std::move(filter)), hash_functions_(hash_functions), tweak_(tweak), flags_(flags) {
}

// filter_load::filter_load(filter_load const& x)
//     : filter_load(x.filter_, x.hash_functions_, x.tweak_, x.flags_) {
// }

// filter_load::filter_load(filter_load&& x) noexcept
//     : filter_load(std::move(x.filter_), x.hash_functions_, x.tweak_, x.flags_) {
// }

// filter_load& filter_load::operator=(filter_load&& x) noexcept {
//     filter_ = std::move(x.filter_);
//     hash_functions_ = x.hash_functions_;
//     tweak_ = x.tweak_;
//     flags_ = x.flags_;
//     return *this;
// }

bool filter_load::operator==(filter_load const& x) const {
    return (filter_ == x.filter_) && (hash_functions_ == x.hash_functions_) && (tweak_ == x.tweak_) && (flags_ == x.flags_);
}

bool filter_load::operator!=(filter_load const& x) const {
    return !(*this == x);
}

bool filter_load::is_valid() const {
    return !filter_.empty() || (hash_functions_ != 0) || (tweak_ != 0) || (flags_ != 0x00);
}

void filter_load::reset() {
    filter_.clear();
    filter_.shrink_to_fit();
    hash_functions_ = 0;
    tweak_ = 0;
    flags_ = 0x00;
}

bool filter_load::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool filter_load::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk filter_load::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void filter_load::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t filter_load::serialized_size(uint32_t /*version*/) const {
    return 1u + 4u + 4u + message::variable_uint_size(filter_.size()) + filter_.size();
}

data_chunk& filter_load::filter() {
    return filter_;
}

data_chunk const& filter_load::filter() const {
    return filter_;
}

void filter_load::set_filter(data_chunk const& value) {
    filter_ = value;
}

void filter_load::set_filter(data_chunk&& value) {
    filter_ = std::move(value);
}

uint32_t filter_load::hash_functions() const {
    return hash_functions_;
}

void filter_load::set_hash_functions(uint32_t value) {
    hash_functions_ = value;
}

uint32_t filter_load::tweak() const {
    return tweak_;
}

void filter_load::set_tweak(uint32_t value) {
    tweak_ = value;
}

uint8_t filter_load::flags() const {
    return flags_;
}

void filter_load::set_flags(uint8_t value) {
    flags_ = value;
}

}  // namespace message
}  // namespace kth
