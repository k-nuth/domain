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
#ifndef LIBBITCOIN_MESSAGE_REJECT_HPP
#define LIBBITCOIN_MESSAGE_REJECT_HPP

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/block.hpp>
#include <bitcoin/bitcoin/message/transaction.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API reject {
   public:
    enum class reason_code : uint8_t {
        /// The reason code is not defined.
        undefined = 0x00,

        /// The message was malformed.
        malformed = 0x01,

        /// In response to block or tx message: invalid (data is hash).
        invalid = 0x10,

        /// In response to version message: version.
        /// In respose to block message: version (data is hash).
        obsolete = 0x11,

        /// In respose to tx message: double spend (data is hash).
        /// In respose to version message: more than one received.
        duplicate = 0x12,

        /// In respose to tx message: nonstandard (data is hash).
        nonstandard = 0x40,

        /// In respose to tx message: dust output(s) (data is hash).
        dust = 0x41,

        /// In respose to tx message: insufficient fee (data is hash).
        insufficient_fee = 0x42,

        /// In response to block message: failed checkpoint (data is hash).
        checkpoint = 0x43
    };

    typedef std::shared_ptr<reject> ptr;
    typedef std::shared_ptr<const reject> const_ptr;

    static reject factory_from_data(uint32_t version, const data_chunk& data);
    static reject factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static reject factory_from_data(uint32_t version, R& source) {
        reject instance;
        instance.from_data(version, source);
        return instance;
    }

    //static reject factory_from_data(uint32_t version, reader& source);

    reject();

    reject(reason_code code, const std::string& message, const std::string& reason);
    reject(reason_code code, std::string&& message, std::string&& reason);

    reject(reason_code code, const std::string& message, const std::string& reason, const hash_digest& data);
    reject(reason_code code, std::string&& message, std::string&& reason, hash_digest&& data);

    reject(const reject& other);
    reject(reject&& other);

    reason_code code() const;
    void set_code(reason_code value);

    std::string& message();
    const std::string& message() const;
    void set_message(const std::string& value);
    void set_message(std::string&& value);

    std::string& reason();
    const std::string& reason() const;
    void set_reason(const std::string& value);
    void set_reason(std::string&& value);

    hash_digest& data();
    const hash_digest& data() const;
    void set_data(const hash_digest& value);
    void set_data(hash_digest&& value);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        message_ = source.read_string();
        code_ = reason_from_byte(source.read_byte());
        reason_ = source.read_string();

        if ((message_ == block::command) ||
            (message_ == transaction::command)) {
            // Some nodes do not follow the documented convention of supplying hash
            // for tx and block rejects. Use this to prevent error on empty stream.
            const auto bytes = source.read_bytes();

            if (bytes.size() == hash_size)
                build_array(data_, {bytes});
        }

        if (version < reject::version_minimum)
            source.invalidate();

        if (!source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_string(message_);
        sink.write_byte(reason_to_byte(code_));
        sink.write_string(reason_);

        if ((message_ == block::command) ||
            (message_ == transaction::command)) {
            sink.write_hash(data_);
        }
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    // This class is move assignable but not copy assignable.
    reject& operator=(reject&& other);
    void operator=(const reject&) = delete;

    bool operator==(const reject& other) const;
    bool operator!=(const reject& other) const;

    static const std::string command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

   private:
    static reason_code reason_from_byte(uint8_t byte);
    static uint8_t reason_to_byte(reason_code value);

    reason_code code_;
    std::string message_;
    std::string reason_;
    hash_digest data_;
};

}  // namespace message
}  // namespace libbitcoin

#endif
