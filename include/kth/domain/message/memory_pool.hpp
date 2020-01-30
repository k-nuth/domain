// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_MEMORY_POOL_HPP
#define KTH_MESSAGE_MEMORY_POOL_HPP

#include <istream>
#include <memory>
#include <string>

#include <kth/domain/define.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace kth {
namespace message {

class BC_API memory_pool {
public:
    using ptr = std::shared_ptr<memory_pool>;
    using const_ptr = std::shared_ptr<const memory_pool>;

    static memory_pool factory_from_data(uint32_t version, data_chunk const& data);
    static memory_pool factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static memory_pool factory_from_data(uint32_t version, R& source) {
        memory_pool instance;
        instance.from_data(version, source);
        return instance;
    }

    static size_t satoshi_fixed_size(uint32_t version);

    // This is a default instance so is invalid.
    // The only way to make this valid is to deserialize it :/.
    memory_pool() = default;
    memory_pool(memory_pool const& x) = default;
    memory_pool(memory_pool&& x) = default;

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(uint32_t version, R& source) {
        reset();

        // Initialize as valid from deserialization.
        insufficient_version_ = false;

        if (version < memory_pool::version_minimum) {
            source.invalidate();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, data_sink& stream) const;

    template <Writer W>
    void to_data(uint32_t  /*version*/, W&  /*sink*/) const {
    }

    //void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    size_t serialized_size(uint32_t version) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;

protected:
    memory_pool(bool insufficient_version);

private:
    bool insufficient_version_{true};
};

}  // namespace message
}  // namespace kth

#endif
