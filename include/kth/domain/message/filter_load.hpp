// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_FILTER_LOAD_HPP
#define KTH_DOMAIN_MESSAGE_FILTER_LOAD_HPP

#include <istream>
#include <memory>
#include <string>

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/utility/byte_reader.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API filter_load {
public:
    using ptr = std::shared_ptr<filter_load>;
    using const_ptr = std::shared_ptr<const filter_load>;

    filter_load() = default;
    filter_load(data_chunk const& filter, uint32_t hash_functions, uint32_t tweak, uint8_t flags);
    filter_load(data_chunk&& filter, uint32_t hash_functions, uint32_t tweak, uint8_t flags);

    bool operator==(filter_load const& x) const;
    bool operator!=(filter_load const& x) const;

    data_chunk& filter();

    [[nodiscard]]
    data_chunk const& filter() const;

    void set_filter(data_chunk const& value);
    void set_filter(data_chunk&& value);

    [[nodiscard]]
    uint32_t hash_functions() const;

    void set_hash_functions(uint32_t value);

    [[nodiscard]]
    uint32_t tweak() const;

    void set_tweak(uint32_t value);

    [[nodiscard]]
    uint8_t flags() const;

    void set_flags(uint8_t value);

    // template <typename R, KTH_IS_READER(R)>
    // bool from_data(R& source, uint32_t version) {
    //     reset();

    //     auto const size = source.read_size_little_endian();

    //     if (size > max_filter_load) {
    //         source.invalidate();
    //     } else {
    //         filter_ = source.read_bytes(size);
    //     }

    //     hash_functions_ = source.read_4_bytes_little_endian();

    //     if (hash_functions_ > max_filter_functions) {
    //         source.invalidate();
    //     }

    //     tweak_ = source.read_4_bytes_little_endian();
    //     flags_ = source.read_byte();

    //     if (version < filter_load::version_minimum) {
    //         source.invalidate();
    //     }

    //     if ( ! source) {
    //         reset();
    //     }

    //     return source;
    // }

    //TODO: move the function definition to the cpp file
    static
    expect<filter_load> from_data(byte_reader& reader, uint32_t version) {
        auto const size = reader.read_size_little_endian();
        if ( ! size) {
            return make_unexpected(size.error());
        }
        if (*size > max_filter_load) {
            return make_unexpected(error::invalid_filter_load);
        }
        auto const filter = reader.read_bytes(*size);
        if ( ! filter) {
            return make_unexpected(filter.error());
        }
        auto const hash_functions = reader.read_little_endian<uint32_t>();
        if ( ! hash_functions) {
            return make_unexpected(hash_functions.error());
        }
        auto const tweak = reader.read_little_endian<uint32_t>();
        if ( ! tweak) {
            return make_unexpected(tweak.error());
        }
        auto const flags = reader.read_byte();
        if ( ! flags) {
            return make_unexpected(flags.error());
        }

        if (version < filter_load::version_minimum) {
            return make_unexpected(error::version_too_low);
        }
        return filter_load(
            data_chunk(filter->begin(), filter->end()),
            *hash_functions,
            *tweak,
            *flags
        );
    }

    [[nodiscard]]
    data_chunk to_data(uint32_t version) const;

    void to_data(uint32_t version, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t /*version*/, W& sink) const {
        sink.write_variable_little_endian(filter_.size());
        sink.write_bytes(filter_);
        sink.write_4_bytes_little_endian(hash_functions_);
        sink.write_4_bytes_little_endian(tweak_);
        sink.write_byte(flags_);
    }

    //void to_data(uint32_t version, writer& sink) const;
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
    data_chunk filter_;
    uint32_t hash_functions_{0};
    uint32_t tweak_{0};
    uint8_t flags_{0x00};
};

} // namespace kth::domain::message

#endif
