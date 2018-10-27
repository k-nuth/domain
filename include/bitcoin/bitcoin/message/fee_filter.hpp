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
#ifndef LIBBITCOIN_MESSAGE_FEE_FILTER_HPP
#define LIBBITCOIN_MESSAGE_FEE_FILTER_HPP

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API fee_filter {
public:
    using ptr = std::shared_ptr<fee_filter>;
    using const_ptr = std::shared_ptr<const fee_filter>;

    static fee_filter factory_from_data(uint32_t version, data_chunk const& data);
    static fee_filter factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static fee_filter factory_from_data(uint32_t version, R& source) {
        fee_filter instance;
        instance.from_data(version, source);
        return instance;
    }

    //static fee_filter factory_from_data(uint32_t version, reader& source);

    static size_t satoshi_fixed_size(uint32_t version);

    fee_filter();
    fee_filter(uint64_t minimum);
    fee_filter(const fee_filter& x) = default;
    fee_filter(fee_filter&& x) = default;

    // This class is move assignable but not copy assignable.
    fee_filter& operator=(fee_filter&& x) = default;
    //TODO(fernando): check all the expressions of this form: 'void operator=' it has no sense
    void operator=(const fee_filter&) = delete;

    bool operator==(const fee_filter& x) const;
    bool operator!=(const fee_filter& x) const;

    uint64_t minimum_fee() const;
    void set_minimum_fee(uint64_t value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        // Initialize as valid from deserialization.
        insufficient_version_ = false;

        minimum_fee_ = source.read_8_bytes_little_endian();

        if (version < fee_filter::version_minimum) {
            source.invalidate();
}

        if ( ! source) {
            reset();
}

        return source;
    }

    //bool from_data(uint32_t version, reader& source);

    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_8_bytes_little_endian(minimum_fee_);
    }

    //void to_data(uint32_t version, writer& sink) const;

    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

protected:
    fee_filter(uint64_t minimum, bool insufficient_version);

private:
    uint64_t minimum_fee_;
    bool insufficient_version_;
};

}  // namespace message
}  // namespace libbitcoin

#endif
