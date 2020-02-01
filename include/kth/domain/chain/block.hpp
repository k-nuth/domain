// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_BLOCK_HPP
#define KTH_CHAIN_BLOCK_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <kth/domain/chain/block_basis.hpp>
#include <kth/domain/chain/chain_state.hpp>
#include <kth/domain/chain/header.hpp>
#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/error.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/asio.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <knuth/common.hpp>
#include <knuth/concepts.hpp>

namespace kth {
namespace chain {

class BC_API block : public block_basis {
public:
    using list = std::vector<block>;
    using indexes = std::vector<size_t>;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation_t {
        uint64_t originator = 0;
        code error = error::not_found;
        chain_state::ptr state = nullptr;

        // Similate organization and instead just validate the block.
        bool simulate = false;

        asio::time_point start_deserialize;
        asio::time_point end_deserialize;
        asio::time_point start_check;
        asio::time_point start_populate;
        asio::time_point start_accept;
        asio::time_point start_connect;
        asio::time_point start_notify;
        asio::time_point start_pop;
        asio::time_point start_push;
        asio::time_point end_push;
        float cache_efficiency;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    block() = default;
    block(chain::header const& header, transaction::list&& transactions);
    block(chain::header const& header, transaction::list const& transactions);

    //Note(kth): cannot be defaulted because of the mutex data member.
    block(block const& x);
    block(block&& x) noexcept;
    /// This class is move assignable and copy assignable.
    block& operator=(block const& x);
    block& operator=(block&& x) noexcept;


    // Operators.
    //-------------------------------------------------------------------------
    // bool operator==(block const& x) const;
    // bool operator!=(block const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static block factory_from_data(data_chunk const& data, bool witness = false);
    static block factory_from_data(std::istream& stream, bool witness = false);

    template <Reader R, KTH_IS_READER(R)>
    static block factory_from_data(R& source, bool witness = false) {
        block instance;
        instance.from_data(source, witness_val(witness));
        return instance;
    }

    bool from_data(data_chunk const& data, bool witness = false);
    bool from_data(std::istream& stream, bool witness = false);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(R& source, bool witness = false) {
        validation.start_deserialize = asio::steady_clock::now();
        block_basis::from_data(source, witness);
        validation.end_deserialize = asio::steady_clock::now();
        return source;
    }


    // bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    using block_basis::to_data;
    data_chunk to_data(bool witness = false) const;


    // void to_data(data_sink& stream, bool witness = false) const;

    // template <Writer W>
    // void to_data(W& sink, bool witness = false) const {
    //     header_.to_data(sink, true);
    //     sink.write_size_little_endian(transactions_.size());
    //     auto const to = [&sink, witness](transaction const& tx) {
    //         tx.to_data(sink, true, witness_val(witness));
    //     };

    //     std::for_each(transactions_.begin(), transactions_.end(), to);
    // }

    // hash_list to_hashes(bool witness = false) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool witness = false) const;

    // void set_header(chain::header const& value);
    void set_transactions(transaction::list const& value);
    void set_transactions(transaction::list&& value);

    // hash_digest hash() const;

    // Utilities.
    //-------------------------------------------------------------------------

    static block genesis_mainnet();
    static block genesis_testnet();
    static block genesis_regtest();
    static size_t locator_size(size_t top);
    static indexes locator_heights(size_t top);

#ifndef KTH_CURRENCY_BCH
    /// Clear witness from all inputs (does not change default hash).
    void strip_witness();
#endif

    // Validation.
    //-------------------------------------------------------------------------

    // static uint64_t subsidy(size_t height, bool retarget = true);
    // static uint256_t proof(uint32_t bits);

    // uint64_t fees() const;
    // uint64_t claim() const;
    // uint64_t reward(size_t height) const;
    // uint256_t proof() const;
    // hash_digest generate_merkle_root(bool witness = false) const;
    size_t signature_operations() const;
    // size_t signature_operations(bool bip16, bool bip141) const;

    using block_basis::signature_operations;

    size_t total_inputs(bool with_coinbase = true) const;
    size_t weight() const;

    // bool is_extra_coinbases() const;
    // bool is_final(size_t height, uint32_t block_time) const;
    // bool is_distinct_transaction_set() const;
    // bool is_valid_coinbase_claim(size_t height) const;
    // bool is_valid_coinbase_script(size_t height) const;
    // bool is_valid_witness_commitment() const;
    // bool is_forward_reference() const;
    // bool is_internal_double_spend() const;
    // bool is_valid_merkle_root() const;
    bool is_segregated() const;

    code check() const;
    // code check_transactions() const;
    code accept(bool transactions = true) const;
    code accept(chain_state const& state, bool transactions = true) const;
    // code accept_transactions(chain_state const& state) const;
    code connect() const;
    // code connect(chain_state const& state) const;
    // code connect_transactions(chain_state const& state) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation_t validation{};

// protected:
    // void reset();
    // size_t non_coinbase_input_count() const;

private:

    // These share a mutext as they are not expected to contend.
    mutable boost::optional<bool> segregated_;
    mutable boost::optional<size_t> total_inputs_;
    mutable boost::optional<size_t> base_size_;
    mutable boost::optional<size_t> total_size_;
    mutable upgrade_mutex mutex_;
};

}  // namespace chain
}  // namespace kth

//#include <knuth/concepts_undef.hpp>

#endif
