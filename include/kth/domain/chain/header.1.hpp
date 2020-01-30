// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_HEADER_HPP
#define KTH_CHAIN_HEADER_HPP

#include <cstddef>
#include <cstdint>
#include <istream>
#include <memory>
#include <string>
#include <vector>

#include <kth/domain/chain/chain_state.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/error.hpp>
#include <kth/infrastructure/math/hash.hpp>
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

class BC_API header {
public:
    typedef std::vector<header> list;
    typedef std::shared_ptr<header> ptr;
    typedef std::shared_ptr<header const> const_ptr;
    typedef std::vector<header> ptr_list;
    typedef std::vector<const_ptr> const_ptr_list;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation {
        size_t height = 0;
        uint32_t median_time_past = 0;
    };

    // Constructors.
    //-----------------------------------------------------------------------------

    header();
    header(header const& x);
    header(header const& x, hash_digest const& hash);
    header(uint32_t version, hash_digest const& previous_block_hash, hash_digest const& merkle, uint32_t timestamp, uint32_t bits, uint32_t nonce);

    // Operators.
    //-----------------------------------------------------------------------------

    /// This class is copy assignable.
    header& operator=(header const& x);

    bool operator==(header const& x) const;
    bool operator!=(header const& x) const;

    // Deserialization.
    //-----------------------------------------------------------------------------

    static header factory_from_data(data_chunk const& data, bool wire = true);
    // static header factory_from_data(std::istream& stream, bool wire=true);
    static header factory_from_data(std::istream& stream, bool wire = true);

    template <Reader R, KTH_IS_READER(R)>
    static header factory_from_data(R& source, bool wire = true) {
        header instance;
        instance.from_data(source, wire);
        return instance;
    }

    //static header factory_from_data(reader& source, bool wire=true);

    bool from_data(data_chunk const& data, bool wire = true);

    //TODO(fernando): check what happend when replacing std::istream to data_source
    // bool from_data(std::istream& stream, bool wire=true);
    bool from_data(std::istream& stream, bool wire = true);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(R& source, bool wire = true) {
        ////reset();

        version_ = source.read_4_bytes_little_endian();
        previous_block_hash_ = source.read_hash();
        merkle_ = source.read_hash();
        timestamp_ = source.read_4_bytes_little_endian();
        bits_ = source.read_4_bytes_little_endian();
        nonce_ = source.read_4_bytes_little_endian();

        if ( ! wire) {
            validation.median_time_past = source.read_4_bytes_little_endian();
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(reader& source, bool wire=true);

    bool is_valid() const;

    // Serialization.
    //-----------------------------------------------------------------------------

    data_chunk to_data(bool wire = true) const;
    // void to_data(data_sink& stream, bool wire=true) const;
    void to_data(data_sink& stream, bool wire = true) const;

    template <Writer W>
    void to_data(W& sink, bool wire = true) const {
        sink.write_4_bytes_little_endian(version_);
        sink.write_hash(previous_block_hash_);
        sink.write_hash(merkle_);
        sink.write_4_bytes_little_endian(timestamp_);
        sink.write_4_bytes_little_endian(bits_);
        sink.write_4_bytes_little_endian(nonce_);

        if ( ! wire) {
            sink.write_4_bytes_little_endian(validation.median_time_past);
        }
    }

    //void to_data(writer& sink, bool wire=true) const;

    // Properties (size, accessors, cache).
    //-----------------------------------------------------------------------------
    static uint256_t proof(uint32_t bits);
    uint256_t proof() const;

    static size_t satoshi_fixed_size();
    size_t serialized_size(bool wire = true) const;

    uint32_t version() const;
    void set_version(uint32_t value);

    // Deprecated (unsafe).
    hash_digest& previous_block_hash();

    hash_digest const& previous_block_hash() const;
    void set_previous_block_hash(hash_digest const& value);

    // Deprecated (unsafe).
    hash_digest& merkle();

    hash_digest const& merkle() const;
    void set_merkle(hash_digest const& value);

    uint32_t timestamp() const;
    void set_timestamp(uint32_t value);

    uint32_t bits() const;
    void set_bits(uint32_t value);

    uint32_t nonce() const;
    void set_nonce(uint32_t value);

    hash_digest hash() const;

#ifdef KTH_CURRENCY_LTC
    hash_digest litecoin_proof_of_work_hash() const;
#endif  //KTH_CURRENCY_LTC

    // Validation.
    //-----------------------------------------------------------------------------

    bool is_valid_timestamp() const;
    bool is_valid_proof_of_work(bool retarget = true) const;

    code check(bool retarget = false) const;
    code accept(chain_state const& state) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation validation;

protected:
    // So that block may call reset from its own.
    friend class block;

    void reset();
    void invalidate_cache() const;

private:
    mutable upgrade_mutex mutex_;
    mutable std::shared_ptr<hash_digest> hash_;

    uint32_t version_;
    hash_digest previous_block_hash_;
    hash_digest merkle_;
    uint32_t timestamp_;
    uint32_t bits_;
    uint32_t nonce_;
};

}  // namespace chain
}  // namespace kth

//#include <knuth/concepts_undef.hpp>

#endif
