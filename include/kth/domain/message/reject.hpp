// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_REJECT_HPP
#define KTH_DOMAIN_MESSAGE_REJECT_HPP

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include <kth/domain/define.hpp>
#include <kth/domain/message/block.hpp>
#include <kth/domain/message/transaction.hpp>
#include <kth/infrastructure/utility/byte_reader.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API reject {
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

    using ptr = std::shared_ptr<reject>;
    using const_ptr = std::shared_ptr<const reject>;

    reject();
    reject(reason_code code, std::string const& message, std::string const& reason);
    reject(reason_code code, std::string&& message, std::string&& reason);
    reject(reason_code code, std::string const& message, std::string const& reason, hash_digest const& data);
    reject(reason_code code, std::string&& message, std::string&& reason, hash_digest const& data);

    bool operator==(reject const& x) const;
    bool operator!=(reject const& x) const;


    [[nodiscard]]
    reason_code code() const;

    void set_code(reason_code value);

    std::string& message();

    [[nodiscard]]
    std::string const& message() const;

    void set_message(std::string const& value);
    void set_message(std::string&& value);

    std::string& reason();

    [[nodiscard]]
    std::string const& reason() const;

    void set_reason(std::string const& value);
    void set_reason(std::string&& value);

    hash_digest& data();

    [[nodiscard]]
    hash_digest const& data() const;

    void set_data(hash_digest const& value);

    // template <typename R, KTH_IS_READER(R)>
    // bool from_data(R& source, uint32_t version) {
    //     reset();

    //     message_ = source.read_string();
    //     code_ = reason_from_byte(source.read_byte());
    //     reason_ = source.read_string();

    //     if ((message_ == block::command) ||
    //         (message_ == transaction::command)) {
    //         // Some nodes do not follow the documented convention of supplying hash
    //         // for tx and block rejects. Use this to prevent error on empty stream.
    //         auto const bytes = source.read_bytes();

    //         if (bytes.size() == hash_size) {
    //             build_array(data_, {bytes});
    //         }
    //     }

    //     if (version < reject::version_minimum) {
    //         source.invalidate();
    //     }

    //     if ( ! source) {
    //         reset();
    //     }

    //     return source;
    // }

    //TODO: move the function definition to the cpp file
    static
    expect<reject> from_data(byte_reader& reader, uint32_t version) {
        auto message = reader.read_string();
        if ( ! message) {
            return make_unexpected(message.error());
        }
        auto const code = reader.read_byte();
        if ( ! code) {
            return make_unexpected(code.error());
        }
        auto reason = reader.read_string();
        if ( ! reason) {
            return make_unexpected(reason.error());
        }

        hash_digest data;
        if (*message == block::command || *message == transaction::command) {
            // Some nodes do not follow the documented convention of supplying hash
            // for tx and block rejects. Use this to prevent error on empty stream.
            auto const bytes = reader.read_remaining_bytes();
            if ( ! bytes) {
                return make_unexpected(bytes.error());
            }
            if (bytes->size() == hash_size) {
                build_array(data, {data_chunk(bytes->begin(), bytes->end())});
            }
        }

        if (version < reject::version_minimum) {
            return make_unexpected(error::version_too_low);
        }

        return reject(
            reason_from_byte(*code),
            std::move(*message),
            std::move(*reason),
            data
        );
    }

    [[nodiscard]]
    data_chunk to_data(uint32_t version) const;

    void to_data(uint32_t version, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t /*version*/, W& sink) const {
        sink.write_string(message_);
        sink.write_byte(reason_to_byte(code_));
        sink.write_string(reason_);

        if ((message_ == block::command) ||
            (message_ == transaction::command)) {
            sink.write_hash(data_);
        }
    }

    [[nodiscard]]
    bool is_valid() const;

    void reset();

    [[nodiscard]]
    size_t serialized_size(uint32_t version) const;


    static
    std::string const command;

    static
    uint32_t const version_minimum;

    static
    uint32_t const version_maximum;


private:
    static
    reason_code reason_from_byte(uint8_t byte);

    static
    uint8_t reason_to_byte(reason_code value);

    reason_code code_{reason_code::undefined};
    std::string message_;
    std::string reason_;
    hash_digest data_;
};

} // namespace kth::domain::message

#endif
