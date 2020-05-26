// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_PONG_HPP
#define KTH_MESSAGE_PONG_HPP

#include <cstdint>
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

class BC_API pong {
public:
    using ptr = std::shared_ptr<pong>;
    using const_ptr = std::shared_ptr<const pong>;

    static
    pong factory_from_data(uint32_t version, data_chunk const& data);
    
    static
    pong factory_from_data(uint32_t version, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    static
    pong factory_from_data(uint32_t version, R& source) {
        pong instance;
        instance.from_data(version, source);
        return instance;
    }

    static
    size_t satoshi_fixed_size(uint32_t version);

    pong() = default;
    pong(uint64_t nonce);

    // pong(pong const& x) = default;
    // // This class is move assignable but not copy assignable.
    // pong& operator=(pong&& x) = default;
    // pong& operator=(pong const&) = default;

    bool operator==(pong const& x) const;
    bool operator!=(pong const& x) const;


    [[nodiscard]] 
    uint64_t nonce() const;
    
    void set_nonce(uint64_t value);

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        reset();

        valid_ = true;
        nonce_ = source.read_8_bytes_little_endian();

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(uint32_t version, reader& source);

    [[nodiscard]] 
    data_chunk to_data(uint32_t version) const;
    
    void to_data(uint32_t version, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t  /*version*/, W& sink) const {
        sink.write_8_bytes_little_endian(nonce_);
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
    uint64_t nonce_{0};
    bool valid_{false};
};

}  // namespace kth::message

#endif
