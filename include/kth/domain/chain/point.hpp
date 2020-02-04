// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_POINT_HPP
#define KTH_CHAIN_POINT_HPP

#include <cstdint>
#include <istream>
#include <string>
#include <vector>

#include <boost/functional/hash.hpp>

#include <kth/domain/chain/point_iterator.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/math/hash.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/data.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/common.hpp>
#include <kth/domain/concepts.hpp>

namespace kth {
namespace chain {

class BC_API point {
public:
    /// This is a sentinel used in .index to indicate no output, e.g. coinbase.
    /// This value is serialized and defined by consensus, not implementation.
    static constexpr uint32_t null_index = no_previous_output;

    using list = std::vector<point>;
    using indexes = std::vector<uint32_t>;

    // Constructors.
    //-------------------------------------------------------------------------

    // constexpr
    point();

    // constexpr
    point(hash_digest const& hash, uint32_t index);

    // constexpr
    point(point const& x) = default;

    // constexpr
    point& operator=(point const& x) = default;

    // Operators.
    //-------------------------------------------------------------------------

    // constexpr    //Note(kth): Could be constexpr in C++20
    friend bool operator==(point const& x, point const& y);

    // constexpr    //Note(kth): Could be constexpr in C++20
    friend bool operator!=(point const& x, point const& y);

    // constexpr
    friend bool operator<(point const& x, point const& y);

    // constexpr
    friend bool operator>(point const& x, point const& y);

    // constexpr
    friend bool operator<=(point const& x, point const& y);

    // constexpr
    friend bool operator>=(point const& x, point const& y);

    // Deserialization.
    //-------------------------------------------------------------------------

    static point factory_from_data(data_chunk const& data, bool wire = true);

    static point factory_from_data(std::istream& stream, bool wire = true);

    template <Reader R, KTH_IS_READER(R)>
    static point factory_from_data(R& source, bool wire = true) {
        point instance;
        instance.from_data(source, wire);
        return instance;
    }

    bool from_data(data_chunk const& data, bool wire = true);
    bool from_data(std::istream& stream, bool wire = true);

    template <Reader R, KTH_IS_READER(R)>
    bool from_data(R& source, bool wire = true) {
        reset();

        valid_ = true;
        hash_ = source.read_hash();

        if (wire) {
            index_ = source.read_4_bytes_little_endian();
        } else {
            index_ = source.read_2_bytes_little_endian();

            if (index_ == max_uint16) {
                index_ = null_index;
            }
        }

        if ( ! source) {
            reset();
        }

        return source;
    }

    // constexpr
    [[nodiscard]] bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    [[nodiscard]] data_chunk to_data(bool wire = true) const;
    void to_data(data_sink& stream, bool wire = true) const;

    template <Writer W>
    void to_data(W& sink, bool wire = true) const {
        sink.write_hash(hash_);

        if (wire) {
            sink.write_4_bytes_little_endian(index_);
        } else {
            KTH_ASSERT(index_ == null_index || index_ < max_uint16);
            sink.write_2_bytes_little_endian(static_cast<uint16_t>(index_));
        }
    }

    // Iteration (limited to store serialization).
    //-------------------------------------------------------------------------

    [[nodiscard]] point_iterator begin() const;
    [[nodiscard]] point_iterator end() const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    // constexpr
    static
    size_t satoshi_fixed_size();

    // constexpr
    [[nodiscard]] size_t serialized_size(bool wire = true) const;

    // deprecated (unsafe)
    // constexpr
    hash_digest& hash();

    // constexpr
    [[nodiscard]] hash_digest const& hash() const;

    // constexpr
    void set_hash(hash_digest const& value);

    // constexpr
    [[nodiscard]] uint32_t index() const;

    // constexpr
    void set_index(uint32_t value);

    // Utilities.
    //-------------------------------------------------------------------------

    /// This is for client-server, not related to consensus or p2p networking.
    [[nodiscard]] uint64_t checksum() const;

    // Validation.
    //-------------------------------------------------------------------------

    // constexpr
    [[nodiscard]] bool is_null() const;

// protected:
    // point(hash_digest const& hash, uint32_t index, bool valid);
    void reset();

private:
    hash_digest hash_{null_hash};
    uint32_t index_{0};
    bool valid_{false};
};

}  // namespace chain
}  // namespace kth

// Standard hash.
//-----------------------------------------------------------------------------

namespace std {

// Extend std namespace with our hash wrapper (database key, not checksum).
template <>
struct hash<bc::chain::point> {
    size_t operator()(const bc::chain::point& point) const {
        size_t seed = 0;
        boost::hash_combine(seed, point.hash());
        boost::hash_combine(seed, point.index());
        return seed;
    }
};

// Extend std namespace with the non-wire size of point (database key size).
template <>
struct tuple_size<bc::chain::point> {
    static auto const value = std::tuple_size<bc::hash_digest>::value + sizeof(uint16_t);

    operator std::size_t() const {
        return value;
    }
};

}  // namespace std

//#include <kth/domain/concepts_undef.hpp>

#endif
