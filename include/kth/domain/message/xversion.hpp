// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_ANNOUNCE_XVERSION_HPP_
#define KTH_DOMAIN_MESSAGE_ANNOUNCE_XVERSION_HPP_

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/message/network_address.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

using namespace kth::infrastructure::message;

// Implementation of BU xversion and xverack messages
// https://github.com/BitcoinUnlimited/BitcoinUnlimited/blob/dev/doc/xversionmessage.md

class KD_API xversion {
public:
    using ptr = std::shared_ptr<xversion>;
    using const_ptr = std::shared_ptr<xversion const>;

    xversion() = default;

    bool operator==(xversion const& x) const;
    bool operator!=(xversion const& x) const;
    
    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, uint32_t version) {
        reset();
        //TODO(fernando): we are skiping the message for the moment.
        source.skip_remaining();

        // size_t const n = source.read_variable_little_endian();
        // for (size_t i = 0; i < n; ++i) {
        //     size_t const k = source.read_variable_little_endian();
        //     source.skip
        //     uint64_t k;
        //     k = ReadCompactUint64<Stream>(s);
        //     s >> v;
        //     map[k] = v;
        // }

        if ( ! source) {
            reset();
        }

        return source;
    }

    // [[nodiscard]]
    // data_chunk to_data(uint32_t version) const;
    
    // void to_data(uint32_t version, data_sink& stream) const;

    // template <typename W>
    // void to_data(uint32_t version, W& sink) const {
    //     sink.write_4_bytes_little_endian(value_);
    //     auto const effective_version = std::min(version, value_);
    //     sink.write_8_bytes_little_endian(services_);
    //     sink.write_8_bytes_little_endian(timestamp_);
    //     address_receiver_.to_data(version, sink, false);
    //     address_sender_.to_data(version, sink, false);
    //     sink.write_8_bytes_little_endian(nonce_);
    //     sink.write_string(user_agent_);
    //     sink.write_4_bytes_little_endian(start_height_);

    //     if (effective_version >= level::bip37) {
    //         sink.write_byte(relay_ ? 1 : 0);
    //     }
    // }

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
};

} // namespace kth::domain::message

#endif
