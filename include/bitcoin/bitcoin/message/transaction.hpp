// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_MESSAGE_TRANSACTION_HPP
#define KTH_MESSAGE_TRANSACTION_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>

#include <bitcoin/bitcoin/chain/input.hpp>
#include <bitcoin/bitcoin/chain/output.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace libbitcoin {
namespace message {

class BC_API transaction : public chain::transaction {
public:
    using ptr = std::shared_ptr<transaction>;
    using const_ptr = std::shared_ptr<const transaction>;
    using ptr_list = std::vector<ptr>;
    using const_ptr_list = std::vector<const_ptr>;
    using const_ptr_list_ptr = std::shared_ptr<const_ptr_list>;
    using const_ptr_list_const_ptr = std::shared_ptr<const const_ptr_list>;

    transaction() = default;
    transaction(uint32_t version, uint32_t locktime, chain::input::list&& inputs, chain::output::list&& outputs);
    transaction(uint32_t version, uint32_t locktime, const chain::input::list& inputs, const chain::output::list& outputs);
    transaction(chain::transaction const& x);
    transaction(chain::transaction&& x);

    transaction& operator=(chain::transaction&& x);



    transaction(transaction const& x) = default;
    transaction(transaction&& x) = default;
    /// This class is move assignable but not copy assignable.
    transaction& operator=(transaction&& x) = default;
    transaction& operator=(transaction const&) = default;

    bool operator==(chain::transaction const& x) const;
    bool operator!=(chain::transaction const& x) const;

    bool operator==(transaction const& x) const;
    bool operator!=(transaction const& x) const;


    static transaction factory_from_data(uint32_t version, data_chunk const& data);
    static transaction factory_from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    static transaction factory_from_data(uint32_t version, R& source) {
        transaction instance;
        instance.from_data(version, source);
        return instance;
    }

    bool from_data(uint32_t version, data_chunk const& data);
    bool from_data(uint32_t version, std::istream& stream);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(uint32_t  /*version*/, R& source) {
        return chain::transaction::from_data(source, true, true);
    }

    data_chunk to_data(uint32_t version, bool witness = true) const;
    void to_data(uint32_t version, data_sink& stream, bool witness = true) const;

    template <Writer W>
    void to_data(uint32_t  /*version*/, W& sink, bool witness = true) const {
        chain::transaction::to_data(sink, true, witness);
    }

    size_t serialized_size(uint32_t version) const;

    static std::string const command;
    static uint32_t const version_minimum;
    static uint32_t const version_maximum;
};

}  // namespace message
}  // namespace kth

#endif
