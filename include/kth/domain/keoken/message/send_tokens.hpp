// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_MESSAGE_SEND_TOKENS_HPP_
#define KTH_KEOKEN_MESSAGE_SEND_TOKENS_HPP_

#include <kth/domain/define.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>
#include <kth/domain/keoken/message/base.hpp>
#include <kth/domain/keoken/primitives.hpp>

namespace kth::keoken::message {

class KD_API send_tokens {
public:
    static
    uint16_t const version = 0;
    
    static
    uint16_t const type = 1;

    // Constructors.
    //-------------------------------------------------------------------------

    send_tokens(asset_id_t asset_id, amount_t amount);

    // Semiregular.
    //-------------------------------------------------------------------------

    send_tokens() = default;
    //Copy and Move ctors and assig ops are generated by default

    // Regular.
    //-------------------------------------------------------------------------

    friend bool operator==(send_tokens const& a, send_tokens const& b);

    friend bool operator!=(send_tokens const& a, send_tokens const& b);

    // Deserialization.
    //-------------------------------------------------------------------------

    //Note: from_data and to_data are not longer simetrical.
    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source) {
        asset_id_ = source.read_4_bytes_big_endian();
        amount_ = source.read_8_bytes_big_endian();

        // if ( ! source)
        //     reset();

        return source;
    }

    // Serialization.
    //-------------------------------------------------------------------------

    kth::data_chunk to_data() const;
    void to_data(kth::data_sink& stream) const;

    //Note: from_data and to_data are not longer simetrical.
    template <typename W>
    void to_data(W& sink) const {
        base::to_data(sink, version, type);
        sink.write_4_bytes_big_endian(asset_id_);
        sink.write_8_bytes_big_endian(amount_);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size() const;

    asset_id_t asset_id() const;
    void set_asset_id(asset_id_t x);

    amount_t amount() const;
    void set_amount(amount_t x);

private:
    asset_id_t asset_id_;
    amount_t amount_;
};

} // namespace kth::keoken::message

#endif  //KTH_KEOKEN_MESSAGE_SEND_TOKENS_HPP_
