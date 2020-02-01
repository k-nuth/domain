// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_SEND_COMPACT_BLOCKS_HPP
#define KTH_MESSAGE_SEND_COMPACT_BLOCKS_HPP

#include <istream>
#include <memory>
#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API send_compact {
public:
    using ptr = std::shared_ptr<send_compact>;
    using const_ptr = std::shared_ptr<const send_compact>;

    static send_compact factory_from_data(uint32_t version, data_chunk const& data);
    static send_compact factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static send_compact factory_from_data(uint32_t version, R& source) {
        send_compact instance;
        instance.from_data(version, source);
        return instance;
    }

    //static send_compact factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    send_compact() = default;
    send_compact(bool high_bandwidth_mode, uint64_t version);

    // send_compact(send_compact const& x) = default;
    // send_compact(send_compact&& x) = default;
    // /// This class is move assignable but not copy assignable.
    // send_compact& operator=(send_compact&& x) = default;
    // send_compact& operator=(send_compact const&) = default;

    bool operator==(send_compact const& x) const;
    bool operator!=(send_compact const& x) const;


    bool high_bandwidth_mode() const;
    void set_high_bandwidth_mode(bool mode);

    uint64_t version() const;
    void set_version(uint64_t version);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const mode = source.read_byte();

        if (mode > 1) {
            source.invalidate();
}

        high_bandwidth_mode_ = (mode == 1);
        this->version_ = source.read_8_bytes_little_endian();

        if (version < send_compact::version_minimum) {
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
    void to_data(uint32_t  /*version*/, W& sink) const {
        sink.write_byte(static_cast<uint8_t>(high_bandwidth_mode_));
        sink.write_8_bytes_little_endian(this->version_);
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;


    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    bool high_bandwidth_mode_{false};
    uint64_t version_{0};
};

}  // namespace message
}  // namespace kth

#endif
