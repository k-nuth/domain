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
#ifndef LIBBITCOIN_MESSAGE_ALERT_HPP
#define LIBBITCOIN_MESSAGE_ALERT_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API alert {
public:
    typedef std::shared_ptr<alert> ptr;
    typedef std::shared_ptr<const alert> const_ptr;

    static alert factory_from_data(uint32_t version, data_chunk const& data);
    static alert factory_from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static alert factory_from_data(uint32_t version, R& source) {
        alert instance;
        instance.from_data(version, source);
        return instance;
    }

    //static alert factory_from_data(uint32_t version, reader& source);

    alert();
    alert(data_chunk const& payload, data_chunk const& signature);
    alert(data_chunk&& payload, data_chunk&& signature);
    alert(const alert& x);
    alert(alert&& x);

    data_chunk& payload();
    data_chunk const& payload() const;
    void set_payload(data_chunk const& value);
    void set_payload(data_chunk&& value);

    data_chunk& signature();
    data_chunk const& signature() const;
    void set_signature(data_chunk const& value);
    void set_signature(data_chunk&& value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        payload_ = source.read_bytes(source.read_size_little_endian());
        signature_ = source.read_bytes(source.read_size_little_endian());

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(uint32_t version, reader& source);

    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_variable_little_endian(payload_.size());
        sink.write_bytes(payload_);
        sink.write_variable_little_endian(signature_.size());
        sink.write_bytes(signature_);
    }

    //void to_data(uint32_t version, writer& sink) const;

    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    /// This class is move assignable but not copy assignable.
    alert& operator=(alert&& x);
    void operator=(const alert&) = delete;

    bool operator==(const alert& x) const;
    bool operator!=(const alert& x) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    data_chunk payload_;
    data_chunk signature_;
};

}  // namespace message
}  // namespace libbitcoin

#endif
