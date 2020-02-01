// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
#include <bitcoin/bitcoin/message/alert.hpp>

// #include <bitcoin/infrastructure/message/message_tools.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/message/message_tools.hpp>
#include <bitcoin/infrastructure/utility/assert.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/limits.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {
=======
#include <kth/domain/message/alert.hpp>

// #include <kth/infrastructure/message/message_tools.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/assert.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

namespace kth::message {
>>>>>>> dev

std::string const alert::command = "alert";
uint32_t const alert::version_minimum = version::level::minimum;
uint32_t const alert::version_maximum = version::level::maximum;

alert alert::factory_from_data(uint32_t version, data_chunk const& data) {
    alert instance;
    instance.from_data(version, data);
    return instance;
}

alert alert::factory_from_data(uint32_t version, std::istream& stream) {
    alert instance;
    instance.from_data(version, stream);
    return instance;
}

alert::alert(data_chunk const& payload, data_chunk const& signature)
    : payload_(payload), signature_(signature) {
}

alert::alert(data_chunk&& payload, data_chunk&& signature)
    : payload_(std::move(payload)), signature_(std::move(signature)) {
}

// alert::alert(alert const& x)
//     : alert(x.payload_, x.signature_) {
// }

// alert::alert(alert&& x) noexcept
//     : alert(std::move(x.payload_), std::move(x.signature_)) {
// }

// alert& alert::operator=(alert&& x) noexcept {
//     payload_ = std::move(x.payload_);
//     signature_ = std::move(x.signature_);
//     return *this;
// }

bool alert::operator==(alert const& x) const {
    return (payload_ == x.payload_) && (signature_ == x.signature_);
}

bool alert::operator!=(alert const& x) const {
    return !(*this == x);
}

bool alert::is_valid() const {
    return !payload_.empty() || !signature_.empty();
}

void alert::reset() {
    payload_.clear();
    payload_.shrink_to_fit();
    signature_.clear();
    signature_.shrink_to_fit();
}

bool alert::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool alert::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk alert::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void alert::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t alert::serialized_size(uint32_t /*version*/) const {
    return message::variable_uint_size(payload_.size()) + payload_.size() +
           message::variable_uint_size(signature_.size()) + signature_.size();
}

data_chunk& alert::payload() {
    return payload_;
}

data_chunk const& alert::payload() const {
    return payload_;
}

void alert::set_payload(data_chunk const& value) {
    payload_ = value;
}

void alert::set_payload(data_chunk&& value) {
    payload_ = std::move(value);
}

data_chunk& alert::signature() {
    return signature_;
}

data_chunk const& alert::signature() const {
    return signature_;
}

void alert::set_signature(data_chunk const& value) {
    signature_ = value;
}

void alert::set_signature(data_chunk&& value) {
    signature_ = std::move(value);
}

<<<<<<< HEAD
}  // namespace message
=======
>>>>>>> dev
}  // namespace kth
