// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_KEOKEN_MESSAGE_CREATE_ASSET_HPP_
#define KTH_KEOKEN_MESSAGE_CREATE_ASSET_HPP_

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>
#include <knuth/keoken/constants.hpp>
#include <knuth/keoken/message/base.hpp>
#include <knuth/keoken/primitives.hpp>

namespace knuth {
namespace keoken {
namespace message {

class BC_API create_asset {
public:
    static uint16_t const version = 0;
    static uint16_t const type = 0;
    static const size_t max_name_size = 32u + 1u;  //32 real chars plus null-terminated

    // Constructors.
    //-------------------------------------------------------------------------

    create_asset(std::string name, amount_t amount);

    // Semiregular.
    //-------------------------------------------------------------------------

    create_asset() = default;
    // create_asset(create_asset const& x) = default;
    // create_asset(create_asset&& x) = default;
    // create_asset& operator=(create_asset const& x) = default;
    // create_asset& operator=(create_asset&& x) = default;

    // Regular.
    //-------------------------------------------------------------------------

    friend bool operator==(create_asset const& a, create_asset const& b);

    friend bool operator!=(create_asset const& a, create_asset const& b);

    // Deserialization.
    //-------------------------------------------------------------------------

    static create_asset factory_from_data(bc::data_chunk const& data);
    static create_asset factory_from_data(std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static create_asset factory_from_data(R& source) {
        create_asset instance;
        instance.from_data(source);
        return instance;
    }

    bool from_data(bc::data_chunk const& data);
    bool from_data(std::istream& stream);

    //Note: from_data and to_data are not longer simetrical.
    template <Reader R, KTH_IS_READER(R)>
    bool from_data(R& source) {
        auto name_opt = read_null_terminated_string(source, max_name_size);
        if ( ! name_opt) {
            source.invalidate();
            return false;
        }

        if (name_opt->size() < min_asset_name_size) {  //NOLINT
            source.invalidate();
            return false;
        }

        name_ = *name_opt;
        amount_ = source.read_8_bytes_big_endian();

        // if ( ! source)
        //     reset();

        return source;
    }

    // Serialization.
    //-------------------------------------------------------------------------

    bc::data_chunk to_data() const;
    void to_data(bc::data_sink& stream) const;

    //Note: from_data and to_data are not simetrical.
    template <Writer W>
    void to_data(W& sink) const {
        base::to_data(sink, version, type);
        sink.write_bytes(reinterpret_cast<uint8_t const*>(name_.data()), name_.size() + 1);  //NOLINT
        sink.write_8_bytes_big_endian(amount_);
    }

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size() const;

    std::string const& name() const;
    void set_name(std::string const& x);
    void set_name(std::string&& x);

    amount_t amount() const;
    void set_amount(amount_t x);

private:
    // char name[17];      // 16 + 1. minus the \0 termination
    std::string name_;
    amount_t amount_;
};

}  // namespace message
}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_MESSAGE_CREATE_ASSET_HPP_
