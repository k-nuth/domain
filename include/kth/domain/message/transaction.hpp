// Copyright (c) 2016-2022 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_TRANSACTION_HPP
#define KTH_DOMAIN_MESSAGE_TRANSACTION_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>

#include <kth/domain/chain/input.hpp>
#include <kth/domain/chain/output.hpp>
#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API transaction : public chain::transaction {
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

    // Witness is always deserialized if present.
    // NOTE: Witness on BCH is dissabled on the chain::block class
    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, uint32_t /*version*/) {
        return chain::transaction::from_data(source, true, true);
    }

    data_chunk to_data(uint32_t version, bool witness = true) const;
    void to_data(uint32_t version, data_sink& stream, bool witness = true) const;

    template <typename W>
    void to_data(uint32_t /*version*/, W& sink, bool witness = true) const {
        chain::transaction::to_data(sink, true, witness);
    }

    size_t serialized_size(uint32_t version) const;

    static
    std::string const command;

    static
    uint32_t const version_minimum;

    static
    uint32_t const version_maximum;

};

} // namespace kth::domain::message

#endif
