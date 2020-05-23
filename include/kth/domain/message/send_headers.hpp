// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_SEND_HEADERS_HPP
#define KTH_DOMAIN_MESSAGE_SEND_HEADERS_HPP

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

class BC_API send_headers {
public:
    using ptr = std::shared_ptr<send_headers>;
    using const_ptr = std::shared_ptr<const send_headers>;

    static
    send_headers factory_from_data(uint32_t version, data_chunk const& data);
    
    static
    send_headers factory_from_data(uint32_t version, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    static
    send_headers factory_from_data(uint32_t version, R& source) {
        send_headers instance;
        instance.from_data(version, source);
        return instance;
    }

    static
    size_t satoshi_fixed_size(uint32_t version);

    send_headers() = default;
    send_headers(send_headers const& x) = default;
    send_headers(send_headers&& x) = default;

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        // Initialize as valid from deserialization.
        insufficient_version_ = false;

        if (version < send_headers::version_minimum) {
            insufficient_version_ = true;
            source.invalidate();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    [[nodiscard]] 
    data_chunk to_data(uint32_t version) const;
    
    void to_data(uint32_t version, data_sink& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    
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


protected:
    send_headers(bool insufficient_version);

private:
    bool insufficient_version_{true};
};

}  // namespace kth::message

#endif
