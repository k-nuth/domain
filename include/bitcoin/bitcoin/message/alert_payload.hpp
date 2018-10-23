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
#ifndef LIBBITCOIN_MESSAGE_ALERT_FORMATTED_PAYLOAD_HPP
#define LIBBITCOIN_MESSAGE_ALERT_FORMATTED_PAYLOAD_HPP

#include <istream>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/math/elliptic_curve.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API alert_payload {
   public:
    static alert_payload factory_from_data(uint32_t version, data_chunk const& data);
    static alert_payload factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static alert_payload factory_from_data(uint32_t version, R& source) {
        alert_payload instance;
        instance.from_data(version, source);
        return instance;
    }

    //static alert_payload factory_from_data(uint32_t version, reader& source);

    alert_payload();
    alert_payload(uint32_t version, uint64_t relay_until, uint64_t expiration, uint32_t id, uint32_t cancel, const std::vector<uint32_t>& set_cancel, uint32_t min_version, uint32_t max_version, const std::vector<std::string>& set_sub_version, uint32_t priority, std::string const& comment, std::string const& status_bar, std::string const& reserved);
    alert_payload(uint32_t version, uint64_t relay_until, uint64_t expiration, uint32_t id, uint32_t cancel, std::vector<uint32_t>&& set_cancel, uint32_t min_version, uint32_t max_version, std::vector<std::string>&& set_sub_version, uint32_t priority, std::string&& comment, std::string&& status_bar, std::string&& reserved);
    alert_payload(const alert_payload& other);
    alert_payload(alert_payload&& other);

    uint32_t version() const;
    void set_version(uint32_t value);

    uint64_t relay_until() const;
    void set_relay_until(uint64_t value);

    uint64_t expiration() const;
    void set_expiration(uint64_t value);

    uint32_t id() const;
    void set_id(uint32_t value);

    uint32_t cancel() const;
    void set_cancel(uint32_t value);

    std::vector<uint32_t>& set_cancel();
    const std::vector<uint32_t>& set_cancel() const;
    void set_set_cancel(const std::vector<uint32_t>& value);
    void set_set_cancel(std::vector<uint32_t>&& value);

    uint32_t min_version() const;
    void set_min_version(uint32_t value);

    uint32_t max_version() const;
    void set_max_version(uint32_t value);

    std::vector<std::string>& set_sub_version();
    const std::vector<std::string>& set_sub_version() const;
    void set_set_sub_version(const std::vector<std::string>& value);
    void set_set_sub_version(std::vector<std::string>&& value);

    uint32_t priority() const;
    void set_priority(uint32_t value);

    std::string& comment();
    std::string const& comment() const;
    void set_comment(std::string const& value);
    void set_comment(std::string&& value);

    std::string& status_bar();
    std::string const& status_bar() const;
    void set_status_bar(std::string const& value);
    void set_status_bar(std::string&& value);

    std::string& reserved();
    std::string const& reserved() const;
    void set_reserved(std::string const& value);
    void set_reserved(std::string&& value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        this->version_ = source.read_4_bytes_little_endian();
        relay_until_ = source.read_8_bytes_little_endian();
        expiration_ = source.read_8_bytes_little_endian();
        id_ = source.read_4_bytes_little_endian();
        cancel_ = source.read_4_bytes_little_endian();
        set_cancel_.reserve(source.read_size_little_endian());

        for (size_t i = 0; i < set_cancel_.capacity() && source; i++)
            set_cancel_.push_back(source.read_4_bytes_little_endian());

        min_version_ = source.read_4_bytes_little_endian();
        max_version_ = source.read_4_bytes_little_endian();
        set_sub_version_.reserve(source.read_size_little_endian());

        for (size_t i = 0; i < set_sub_version_.capacity() && source; i++)
            set_sub_version_.push_back(source.read_string());

        priority_ = source.read_4_bytes_little_endian();
        comment_ = source.read_string();
        status_bar_ = source.read_string();
        reserved_ = source.read_string();

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_4_bytes_little_endian(this->version_);
        sink.write_8_bytes_little_endian(relay_until_);
        sink.write_8_bytes_little_endian(expiration_);
        sink.write_4_bytes_little_endian(id_);
        sink.write_4_bytes_little_endian(cancel_);
        sink.write_variable_little_endian(set_cancel_.size());

        for (auto const& entry : set_cancel_)
            sink.write_4_bytes_little_endian(entry);

        sink.write_4_bytes_little_endian(min_version_);
        sink.write_4_bytes_little_endian(max_version_);
        sink.write_variable_little_endian(set_sub_version_.size());

        for (auto const& entry : set_sub_version_)
            sink.write_string(entry);

        sink.write_4_bytes_little_endian(priority_);
        sink.write_string(comment_);
        sink.write_string(status_bar_);
        sink.write_string(reserved_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    /// This class is move assignable but not copy assignable.
    alert_payload& operator=(alert_payload&& other);
    void operator=(const alert_payload&) = delete;

    bool operator==(const alert_payload& other) const;
    bool operator!=(const alert_payload& other) const;

    static const ec_uncompressed satoshi_public_key;

   private:
    uint32_t version_;
    uint64_t relay_until_;
    uint64_t expiration_;
    uint32_t id_;
    uint32_t cancel_;
    std::vector<uint32_t> set_cancel_;
    uint32_t min_version_;
    uint32_t max_version_;
    std::vector<std::string> set_sub_version_;
    uint32_t priority_;
    std::string comment_;
    std::string status_bar_;
    std::string reserved_;
};

}  // namespace message
}  // namespace libbitcoin

#endif
