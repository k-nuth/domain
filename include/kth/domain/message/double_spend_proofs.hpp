// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_DOUBLE_SPEND_PROOFS_HPP
#define KTH_DOMAIN_MESSAGE_DOUBLE_SPEND_PROOFS_HPP

#include <istream>

#include <kth/domain/chain/output_point.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
// #include <kth/domain/message/block.hpp>
// #include <kth/domain/message/prefilled_transaction.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/utils.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::domain::message {

class KD_API double_spend_proofs {
public:
    using ptr = std::shared_ptr<double_spend_proofs>;
    using const_ptr = std::shared_ptr<double_spend_proofs const>;
    using short_id = uint64_t;
    using short_id_list = std::vector<short_id>;

    struct spender {
        uint32_t version = 0;
        uint32_t out_sequence = 0;
        uint32_t locktime = 0;
        hash_digest prev_outs_hash = null_hash;
        hash_digest sequence_hash = null_hash;
        hash_digest outputs_hash = null_hash;
        data_chunk push_data;

        [[nodiscard]]
        bool is_valid() const {
            return version != 0 || 
                   out_sequence != 0 ||
                   locktime != 0 ||
                   prev_outs_hash != null_hash ||
                   sequence_hash != null_hash ||
                   outputs_hash != null_hash;
        }

        void reset() {
            version = 0;
            out_sequence = 0;
            locktime = 0;
            prev_outs_hash = null_hash;
            sequence_hash = null_hash;
            outputs_hash = null_hash;
            push_data.clear();
        }

        friend
        bool operator==(spender const& x, spender const& y) {
            return x.version == y.version && 
                   x.out_sequence == y.out_sequence && 
                   x.locktime == y.locktime && 
                   x.prev_outs_hash == y.prev_outs_hash && 
                   x.sequence_hash == y.sequence_hash && 
                   x.outputs_hash == y.outputs_hash && 
                   x.push_data == y.push_data;
        }

        friend
        bool operator!=(spender const& x, spender const& y) { 
            return !(x == y);
        }

        [[nodiscard]]
        size_t serialized_size() const {
            return sizeof(version) +
                sizeof(out_sequence) +
                sizeof(locktime) +
                hash_size + 
                hash_size + 
                hash_size + 
                push_data.size();
        }

        template <typename W>
        void to_data(W& sink) const {
            sink.write_4_bytes_little_endian(version);
            sink.write_4_bytes_little_endian(out_sequence);
            sink.write_4_bytes_little_endian(locktime);
            sink.write_hash(prev_outs_hash);
            sink.write_hash(sequence_hash);
            sink.write_hash(outputs_hash);
            sink.write_bytes(push_data);
        }
    };

    double_spend_proofs() = default;
    double_spend_proofs(chain::output_point const& out_point, spender const& spender1, spender const& spender2);

    friend
    bool operator==(double_spend_proofs const& x, double_spend_proofs const& y) {
        return x.out_point_ == y.out_point_ && 
            x.spender1_ == y.spender1_ && 
            x.spender2_ == y.spender2_;
    }

    friend
    bool operator!=(double_spend_proofs const& x, double_spend_proofs const& y) {
        return !(x == y);
    }

    [[nodiscard]]
    chain::output_point const& out_point() const;
    void set_out_point(chain::output_point const& x);

    [[nodiscard]]
    spender const& spender1() const;
    void set_spender1(spender const& x);

    [[nodiscard]]
    spender const& spender2() const;
    void set_spender2(spender const& x);

    // template <typename R, KTH_IS_READER(R)>
    // bool from_data(R& source) {
    //     reset();

    //     if ( ! header_.from_data(source)) {
    //         return false;
    //     }

    //     nonce_ = source.read_8_bytes_little_endian();
    //     auto count = source.read_size_little_endian();

    //     // Guard against potential for arbitary memory allocation.
    //     if (count > get_max_block_size_network_independent()) {
    //         source.invalidate();
    //     } else {
    //         short_ids_.reserve(count);
    //     }

    //     //TODO: move to function
    //     // Order is required.
    //     for (size_t id = 0; id < count && source; ++id) {
    //         uint32_t lsb = source.read_4_bytes_little_endian();
    //         uint16_t msb = source.read_2_bytes_little_endian();
    //         short_ids_.push_back((uint64_t(msb) << 32) | uint64_t(lsb));
    //         //short_ids_.push_back(source.read_mini_hash());
    //     }

    //     count = source.read_size_little_endian();

    //     // Guard against potential for arbitary memory allocation.
    //     if (count > get_max_block_size_network_independent()) {
    //         source.invalidate();
    //     } else {
    //         transactions_.resize(count);
    //     }

    //     // NOTE: Witness flag is controlled by prefilled tx
    //     // Order is required.
    //     for (auto& tx : transactions_) {
    //         if ( ! entity_from_data(tx, version, source)) {
    //             break;
    //         }
    //     }

    //     if (version < double_spend_proofs::version_minimum) {
    //         source.invalidate();
    //     }

    //     if ( ! source) {
    //         reset();
    //     }

    //     return source;
    // }

    [[nodiscard]]
    data_chunk to_data() const;
    
    void to_data(data_sink& stream) const;

    template <typename W>
    void to_data(W& sink) const {
        out_point_.to_data(sink);
        spender1_.to_data(sink);
        spender2_.to_data(sink);
    }

    [[nodiscard]]
    bool is_valid() const;
    
    void reset();
    
    [[nodiscard]]
    size_t serialized_size() const {
        return out_point_.serialized_size() +
            spender1_.serialized_size() +
            spender2_.serialized_size();
    }

    static
    std::string const command;
    
    static
    uint32_t const version_minimum;
    
    static
    uint32_t const version_maximum;

private:
    chain::output_point out_point_;
    spender spender1_;
    spender spender2_;
};

hash_digest hash(double_spend_proofs const& x);

} // namespace kth::domain::message

#endif // KTH_DOMAIN_MESSAGE_DOUBLE_SPEND_PROOFS_HPP
