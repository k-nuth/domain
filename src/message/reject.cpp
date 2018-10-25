/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/message/reject.hpp>

// #include <bitcoin/bitcoin/message/block.hpp>
#include <bitcoin/bitcoin/message/messages.hpp>
// #include <bitcoin/bitcoin/message/transaction.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const reject::command = "reject";
uint32_t const reject::version_minimum = version::level::bip61;
uint32_t const reject::version_maximum = version::level::maximum;

reject reject::factory_from_data(uint32_t version, data_chunk const& data) {
    reject instance;
    instance.from_data(version, data);
    return instance;
}

reject reject::factory_from_data(uint32_t version, data_source& stream) {
    reject instance;
    instance.from_data(version, stream);
    return instance;
}

reject::reject()
    : code_(reason_code::undefined), data_(null_hash) {
}

reject::reject(reason_code code, std::string const& message, std::string const& reason)
    : reject(code, message, reason, null_hash) {
}

reject::reject(reason_code code, std::string&& message, std::string&& reason)
    : code_(code),
      message_(std::move(message)),
      reason_(std::move(reason)),
      data_(null_hash) {
}

reject::reject(reason_code code, std::string const& message, std::string const& reason, hash_digest const& data)
    : code_(code),
      message_(message),
      reason_(reason),
      data_(data) {
}

reject::reject(reason_code code, std::string&& message, std::string&& reason, hash_digest const& data)
    : code_(code),
      message_(std::move(message)),
      reason_(std::move(reason)),
      data_(data) {
}

// reject::reject(reject const& x)
//     : reject(x.code_, x.message_, x.reason_, x.data_) {
// }

// reject::reject(reject&& x) noexcept
//     : reject(x.code_, std::move(x.message_), std::move(x.reason_), std::move(x.data_)) 
// {}

// reject& reject::operator=(reject&& x) noexcept {
//     code_ = x.code_;
//     reason_ = std::move(x.reason_);
//     message_ = std::move(x.message_);
//     data_ = std::move(x.data_);
//     return *this;
// }

bool reject::operator==(reject const& x) const {
    return (code_ == x.code_) && (reason_ == x.reason_) && (message_ == x.message_) && (data_ == x.data_);
}

bool reject::operator!=(reject const& x) const {
    return !(*this == x);
}


bool reject::is_valid() const {
    return !message_.empty() || (code_ != reason_code::undefined) || !reason_.empty() || (data_ != null_hash);
}

void reject::reset() {
    message_.clear();
    message_.shrink_to_fit();
    code_ = reason_code::undefined;
    reason_.clear();
    reason_.shrink_to_fit();
    data_.fill(0);
}

bool reject::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool reject::from_data(uint32_t version, data_source& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk reject::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void reject::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

size_t reject::serialized_size(uint32_t /*version*/) const {
    size_t size = 1u + message::variable_uint_size(message_.size()) +
                  message_.size() + message::variable_uint_size(reason_.size()) +
                  reason_.size();

    if ((message_ == block::command) ||
        (message_ == transaction::command)) {
        size += hash_size;
    }

    return size;
}

reject::reason_code reject::code() const {
    return code_;
}

void reject::set_code(reason_code value) {
    code_ = value;
}

std::string& reject::message() {
    return message_;
}

std::string const& reject::message() const {
    return message_;
}

void reject::set_message(std::string const& value) {
    message_ = value;
}

void reject::set_message(std::string&& value) {
    message_ = std::move(value);
}

std::string& reject::reason() {
    return reason_;
}

std::string const& reject::reason() const {
    return reason_;
}

void reject::set_reason(std::string const& value) {
    reason_ = value;
}

void reject::set_reason(std::string&& value) {
    reason_ = std::move(value);
}

hash_digest& reject::data() {
    return data_;
}

hash_digest const& reject::data() const {
    return data_;
}

void reject::set_data(hash_digest const& value) {
    data_ = value;
}

reject::reason_code reject::reason_from_byte(uint8_t byte) {
    switch (byte) {
        case 0x01:
            return reason_code::malformed;
        case 0x10:
            return reason_code::invalid;
        case 0x11:
            return reason_code::obsolete;
        case 0x12:
            return reason_code::duplicate;
        case 0x40:
            return reason_code::nonstandard;
        case 0x41:
            return reason_code::dust;
        case 0x42:
            return reason_code::insufficient_fee;
        case 0x43:
            return reason_code::checkpoint;
        default:
            return reason_code::undefined;
    }
}

uint8_t reject::reason_to_byte(reason_code value) {
    switch (value) {
        case reason_code::malformed:
            return 0x01;
        case reason_code::invalid:
            return 0x10;
        case reason_code::obsolete:
            return 0x11;
        case reason_code::duplicate:
            return 0x12;
        case reason_code::nonstandard:
            return 0x40;
        case reason_code::dust:
            return 0x41;
        case reason_code::insufficient_fee:
            return 0x42;
        case reason_code::checkpoint:
            return 0x43;
        default:
            return 0x00;
    }
}

}  // namespace message
}  // namespace libbitcoin
