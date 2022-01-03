// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_HEADERS_HPP
#define KTH_DOMAIN_MESSAGE_HEADERS_HPP

#include <cstdint>
#include <initializer_list>
#include <istream>
#include <memory>
#include <string>

#include <kth/domain/define.hpp>
#include <kth/domain/message/header.hpp>
#include <kth/domain/message/inventory.hpp>
#include <kth/domain/message/inventory_vector.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API headers {
public:
    using ptr = std::shared_ptr<headers>;
    using const_ptr = std::shared_ptr<const headers>;

    headers() = default;
    headers(header::list const& values);
    headers(header::list&& values);
    headers(std::initializer_list<header> const& values);

    bool operator==(headers const& x) const;
    bool operator!=(headers const& x) const;

    header::list& elements();
    
    [[nodiscard]]
    header::list const& elements() const;
    
    void set_elements(header::list const& values);
    void set_elements(header::list&& values);

    [[nodiscard]]
    bool is_sequential() const;
    
    void to_hashes(hash_list& out) const;
    void to_inventory(inventory_vector::list& out, inventory::type_id type) const;

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, uint32_t version) {
        reset();

        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > max_get_headers) {
            source.invalidate();
        } else {
            elements_.resize(count);
        }

        // Order is required.
        for (auto& element : elements_) {
            if ( ! element.from_data(source, version)) {
                break;
            }
        }

        if (version < headers::version_minimum) {
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

    template <typename W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_variable_little_endian(elements_.size());

        for (auto const& element : elements_) {
            element.to_data(version, sink);
        }
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
    header::list elements_;
};

} // namespace kth::domain::message

#endif
