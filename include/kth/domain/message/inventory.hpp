// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_INVENTORY_HPP
#define KTH_MESSAGE_INVENTORY_HPP

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <istream>
#include <memory>
#include <string>

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/message/inventory_vector.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace kth {
namespace message {

class BC_API inventory {
public:
    using ptr = std::shared_ptr<inventory>;
    using const_ptr = std::shared_ptr<const inventory>;
    using type_id = inventory_vector::type_id;

    static inventory factory_from_data(uint32_t version, data_chunk const& data);
    static inventory factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static inventory factory_from_data(uint32_t version, R& source) {
        inventory instance;
        instance.from_data(version, source);
        return instance;
    }

    inventory() = default;
    inventory(inventory_vector::list const& values);
    inventory(inventory_vector::list&& values);
    inventory(hash_list const& hashes, type_id type);
    inventory(std::initializer_list<inventory_vector> const& values);

    // inventory(inventory const& x) = default;
    // inventory(inventory&& x) = default;
    // // This class is move assignable but not copy assignable.
    // inventory& operator=(inventory&& x) = default;
    // inventory& operator=(inventory const&) = default;

    bool operator==(inventory const& x) const;
    bool operator!=(inventory const& x) const;


    inventory_vector::list& inventories();
    [[nodiscard]] inventory_vector::list const& inventories() const;
    void set_inventories(inventory_vector::list const& value);
    void set_inventories(inventory_vector::list&& value);

    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, data_chunk const& data);

    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    /*virtual*/  //TODO(fernando): check if this function is used in a run-time-polymorphic way
    bool from_data(uint32_t version, R& source) {
        reset();

        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > max_inventory) {
            source.invalidate();
        } else {
            inventories_.resize(count);
        }

        // Order is required.
        for (auto& inventory : inventories_) {
            if ( ! inventory.from_data(version, source)) {
                break;
            }
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    [[nodiscard]] data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t version, W& sink) const {
        sink.write_variable_little_endian(inventories_.size());

        for (auto const& inventory : inventories_) {
            inventory.to_data(version, sink);
}
    }

    //void to_data(uint32_t version, writer& sink) const;
    void to_hashes(hash_list& out, type_id type) const;
    void reduce(inventory_vector::list& out, type_id type) const;
    [[nodiscard]] bool is_valid() const;
    void reset();
    [[nodiscard]] size_t serialized_size(uint32_t version) const;
    [[nodiscard]] size_t count(type_id type) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

private:
    inventory_vector::list inventories_;
};

}  // namespace message
}  // namespace kth

#endif
