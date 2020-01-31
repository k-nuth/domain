// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_HEADING_HPP
#define KTH_MESSAGE_HEADING_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <string>

#include <boost/array.hpp>

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/math/checksum.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace kth {
namespace message {

enum class message_type {
    unknown,
    address,
    alert,
    block,
    block_transactions,
    compact_block,
    fee_filter,
    filter_add,
    filter_clear,
    filter_load,
    get_address,
    get_block_transactions,
    get_blocks,
    get_data,
    get_headers,
    headers,
    inventory,
    memory_pool,
    merkle_block,
    not_found,
    ping,
    pong,
    reject,
    send_compact,
    send_headers,
    transaction,
    verack,
    version
};

class BC_API heading {
public:
    static size_t maximum_size();
    static size_t maximum_payload_size(uint32_t version, bool witness);
    static size_t satoshi_fixed_size();
    static heading factory_from_data(data_chunk const& data);
    static heading factory_from_data(std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static heading factory_from_data(R& source) {
        heading instance;
        instance.from_data(source);
        return instance;
    }

    //static heading factory_from_data(reader& source);

    heading() = default;
    heading(uint32_t magic, std::string const& command, uint32_t payload_size, uint32_t checksum);
    heading(uint32_t magic, std::string&& command, uint32_t payload_size, uint32_t checksum);

    // heading(heading const& x) = default;
    // heading(heading&& x) = default;
    // // This class is move assignable but not copy assignable.
    // heading& operator=(heading&& x) = default;
    // heading& operator=(heading const&) = default;

    bool operator==(heading const& x) const;
    bool operator!=(heading const& x) const;


    [[nodiscard]] uint32_t magic() const;
    void set_magic(uint32_t value);

    std::string& command();
    [[nodiscard]] std::string const& command() const;
    void set_command(std::string const& value);
    void set_command(std::string&& value);

    [[nodiscard]] uint32_t payload_size() const;
    void set_payload_size(uint32_t value);

    [[nodiscard]] uint32_t checksum() const;
    void set_checksum(uint32_t value);

    [[nodiscard]] message_type type() const;

    bool from_data(data_chunk const& data);
    bool from_data(std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(R& source) {
        reset();
        magic_ = source.read_4_bytes_little_endian();
        command_ = source.read_string(command_size);
        payload_size_ = source.read_4_bytes_little_endian();
        checksum_ = source.read_4_bytes_little_endian();

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(reader& source);
    [[nodiscard]] data_chunk to_data() const;
    void to_data(data_sink& stream) const;

    template <Writer W>
    void to_data(W& sink) const {
        sink.write_4_bytes_little_endian(magic_);
        sink.write_string(command_, command_size);
        sink.write_4_bytes_little_endian(payload_size_);
        sink.write_4_bytes_little_endian(checksum_);
    }

    //void to_data(writer& sink) const;
    [[nodiscard]] bool is_valid() const;
    void reset();


private:
    uint32_t magic_{0};
    std::string command_;
    uint32_t payload_size_{0};
    uint32_t checksum_{0};
};

}  // namespace message
}  // namespace kth

#endif
