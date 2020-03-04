// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_XVERACK_HPP_
#define KTH_MESSAGE_XVERACK_HPP_

#include <istream>
#include <memory>
#include <string>

#include <kth/domain/define.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/common.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::message {

// Implementation of BU xversion and xverack messages
// https://github.com/BitcoinUnlimited/BitcoinUnlimited/blob/dev/doc/xversionmessage.md


// The checksum is ignored by the xverack command.
class BC_API xverack {
public:
    using ptr = std::shared_ptr<xverack>;
    using const_ptr = std::shared_ptr<const xverack>;

    static xverack factory_from_data(uint32_t version, data_chunk const& data);
    static xverack factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static xverack factory_from_data(uint32_t version, R& source) {
        xverack instance;
        instance.from_data(version, source);
        return instance;
    }

    //static xverack factory_from_data(uint32_t version, reader& source);
    static size_t satoshi_fixed_size(uint32_t version);

    xverack() = default;

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        reset();
        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    [[nodiscard]] data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    [[nodiscard]] bool is_valid() const;
    void reset();
    [[nodiscard]] size_t serialized_size(uint32_t version) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace kth::message

#endif
