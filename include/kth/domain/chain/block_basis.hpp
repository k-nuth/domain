// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_CHAIN_BLOCK_BASIS_HPP
#define KTH_DOMAIN_CHAIN_BLOCK_BASIS_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <kth/domain/chain/chain_state.hpp>
#include <kth/domain/chain/header.hpp>
#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/common.hpp>
#include <kth/domain/concepts.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/multi_crypto_settings.hpp>
#include <kth/domain/utils.hpp>
#include <kth/infrastructure/error.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/asio.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/thread.hpp>
#include <kth/infrastructure/utility/writer.hpp>

namespace kth::domain::chain {

constexpr
size_t weight(size_t serialized_size_true, size_t serialized_size_false) {
    // Block weight is 3 * Base size * + 1 * Total size (bip141).
    return base_size_contribution * serialized_size_false + total_size_contribution * serialized_size_true;
}

class BC_API block_basis {
public:
    using list = std::vector<block_basis>;
    using indexes = std::vector<size_t>;

    // Constructors.
    //-------------------------------------------------------------------------

    block_basis() = default;
    block_basis(chain::header const& header, transaction::list&& transactions);
    block_basis(chain::header const& header, transaction::list const& transactions);

    // Operators.
    //-------------------------------------------------------------------------
    bool operator==(block_basis const& x) const;
    bool operator!=(block_basis const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static
    block_basis factory_from_data(data_chunk const& data, bool witness = false);
    
    static
    block_basis factory_from_data(std::istream& stream, bool witness = false);

    template <typename R, KTH_IS_READER(R)>
    static
    block_basis factory_from_data(R& source, bool witness = false) {
        block_basis instance;
        instance.from_data(source, witness_val(witness));
        return instance;
    }

    bool from_data(data_chunk const& data, bool witness = false);
    bool from_data(std::istream& stream, bool witness = false);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(R& source, bool witness = false) {
        // validation.start_deserialize = asio::steady_clock::now();
        reset();

        if ( ! header_.from_data(source, true)) {
            return false;
        }

        auto const count = source.read_size_little_endian();

        // Guard against potential for arbitary memory allocation.
        if (count > get_max_block_size()) {
            source.invalidate();
        } else {
            transactions_.resize(count);
        }

        // Order is required, explicit loop allows early termination.
        for (auto& tx : transactions_) {
            if ( ! tx.from_data(source, true, witness_val(witness))) {
                break;
            }
        }

#if defined(KTH_SEGWIT_ENABLED)
        // TODO(legacy): optimize by having reader skip witness data.
        if ( ! witness_val(witness)) {
            strip_witness();
        }
#endif

        if ( ! source) {
            reset();
        }

        // validation.end_deserialize = asio::steady_clock::now();
        return source;
    }

    [[nodiscard]] 
    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    [[nodiscard]] 
    data_chunk to_data(size_t serialized_size, bool witness = false) const;

    void to_data(data_sink& stream, bool witness = false) const;

    template <typename W>
    void to_data(W& sink, bool witness = false) const {
        header_.to_data(sink, true);
        sink.write_size_little_endian(transactions_.size());
        auto const to = [&sink, witness](transaction const& tx) {
            tx.to_data(sink, true, witness_val(witness));
        };

        std::for_each(transactions_.begin(), transactions_.end(), to);
    }

    [[nodiscard]] 
    hash_list to_hashes(bool witness = false) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    // deprecated (unsafe)
    chain::header& header();

    [[nodiscard]] 
    chain::header const& header() const;

    void set_header(chain::header const& value);

    // deprecated (unsafe)
    transaction::list& transactions();

    [[nodiscard]] 
    transaction::list const& transactions() const;

    void set_transactions(transaction::list const& value);
    void set_transactions(transaction::list&& value);

    [[nodiscard]] 
    hash_digest hash() const;

    // Utilities.
    //-------------------------------------------------------------------------

    static
    size_t locator_size(size_t top);
    
    static
    indexes locator_heights(size_t top);

#if defined(KTH_SEGWIT_ENABLED)
    void strip_witness();
#endif

    // Validation.
    //-------------------------------------------------------------------------

    static
    uint64_t subsidy(size_t height, bool retarget = true);
    
    static
    uint256_t proof(uint32_t bits);

    [[nodiscard]] 
    uint64_t fees() const;
    
    [[nodiscard]] 
    uint64_t claim() const;
    
    [[nodiscard]] 
    uint64_t reward(size_t height) const;
    
    [[nodiscard]] 
    uint256_t proof() const;
    
    [[nodiscard]]
    hash_digest generate_merkle_root(bool witness = false) const;
    
    [[nodiscard]] 
    size_t signature_operations(bool bip16, bool bip141) const;
    
    [[nodiscard]] 
    bool is_extra_coinbases() const;
    
    [[nodiscard]] 
    bool is_final(size_t height, uint32_t block_time) const;
    
    [[nodiscard]] 
    bool is_distinct_transaction_set() const;
    
    [[nodiscard]] 
    bool is_valid_coinbase_claim(size_t height) const;
    
    [[nodiscard]] 
    bool is_valid_coinbase_script(size_t height) const;
    
#if defined(KTH_SEGWIT_ENABLED)
    [[nodiscard]]
    bool is_valid_witness_commitment() const;
#endif    

    [[nodiscard]] 
    bool is_forward_reference() const;
    
    [[nodiscard]] 
    bool is_canonical_ordered() const;
    
    [[nodiscard]] 
    bool is_internal_double_spend() const;
    
    [[nodiscard]] 
    bool is_valid_merkle_root() const;
    
    [[nodiscard]] 
    code check(size_t serialized_size_false) const;
    
    [[nodiscard]] 
    code check_transactions() const;
        
    [[nodiscard]] 
    code accept(chain_state const& state, size_t serialized_size, size_t weight, bool transactions = true) const;
    
    [[nodiscard]] 
    code accept_transactions(chain_state const& state) const;
        
    [[nodiscard]] 
    code connect(chain_state const& state) const;
    
    [[nodiscard]] 
    code connect_transactions(chain_state const& state) const;

// protected:
    void reset();
    
    [[nodiscard]] 
    size_t non_coinbase_input_count() const;

private:
    chain::header header_;
    transaction::list transactions_;
};

size_t total_inputs(block_basis const& blk, bool with_coinbase = true);

#if defined(KTH_SEGWIT_ENABLED)
bool is_segregated(block_basis const& blk);
#endif

size_t serialized_size(block_basis const& blk, bool witness = false);

}  // namespace kth::chain

#endif // KTH_CHAIN_BLOCK_BASIS_HPP_
