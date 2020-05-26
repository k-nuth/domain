// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_GET_ADDRESS_HPP
#define KTH_MESSAGE_GET_ADDRESS_HPP

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

class BC_API get_address {
public:
    using ptr = std::shared_ptr<get_address>;
    using const_ptr = std::shared_ptr<const get_address>;

    static
    get_address factory_from_data(uint32_t version, data_chunk const& data);
    
    static
    get_address factory_from_data(uint32_t version, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    static
    get_address factory_from_data(uint32_t version, R& source) {
        get_address instance;
        instance.from_data(version, source);
        return instance;
    }

    static
    size_t satoshi_fixed_size(uint32_t version);

    get_address() = default;

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        reset();
        return source;
    }

    [[nodiscard]] 
    data_chunk to_data(uint32_t version) const;
    
    void to_data(uint32_t version, data_sink& stream) const;

    //TODO(fernando): this function is empty!!!!!
    template <typename W>
    void to_data(uint32_t  /*version*/, W&  /*sink*/) const {

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
};

}  // namespace kth::message

#endif
