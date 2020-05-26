// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CHAIN_OUTPUT_POINT_HPP
#define KTH_CHAIN_OUTPUT_POINT_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

#include <kth/domain/chain/output.hpp>
#include <kth/domain/chain/point.hpp>
#include <kth/domain/chain/script.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>

namespace kth {
namespace chain {

class BC_API output_point : public point {
public:
    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    struct validation_type {
        /// An output is spent if a valid transaction has a valid claim on it.
        /// When validating blocks only long chain blocks can have a claim.
        /// When validating memory pool tx another mempool tx can have a claim.
        bool spent = false;

        /// A spend is confirmed if spender is in long chain (not memory pool).
        bool confirmed = false;

        /// The previous output is a coinbase (must verify spender maturity).
        bool coinbase = false;

        /// Prevout height is used for coinbase maturity and relative lock time.
        size_t height = 0;

        /// Median time past is used for relative lock time.
        uint32_t median_time_past = 0;

        /// The output cache contains the output referenced by the input point.
        /// If the cache.value is not_found (default) the output is not found.
        output cache = output{};

        //TODO(fernando): add a compilation flag to exclude this...
        /// Tells if the output cache was found in the mempool or in the UTXO Set.
        bool from_mempool = false;
    };

    // Constructors.
    //-------------------------------------------------------------------------

    output_point();
    output_point(hash_digest const& hash, uint32_t index);

    output_point(point const& x);
    output_point& operator=(point const& /*x*/);

    // output_point(output_point const& x) = default;
    // output_point(output_point&& x) = default;
    // // This class is move assignable and copy assignable.
    // output_point& operator=(output_point const&) = default;
    // output_point& operator=(output_point&& x) = default;

    // Operators.
    //-------------------------------------------------------------------------

    friend bool operator==(output_point const& x, point const& y);
    friend bool operator!=(output_point const& x, point const& y);

    friend bool operator==(point const& x, output_point const& y);
    friend bool operator!=(point const& x, output_point const& y);

    friend bool operator==(output_point const& x, output_point const& y);
    friend bool operator!=(output_point const& x, output_point const& y);

    // Deserialization.
    //-------------------------------------------------------------------------

    static output_point factory_from_data(data_chunk const& data, bool wire = true);
    static output_point factory_from_data(std::istream& stream, bool wire = true);
    // static output_point factory_from_data(reader& source, bool wire=true);

    template <typename R, KTH_IS_READER(R)>
    static output_point factory_from_data(R& source, bool wire = true) {
        output_point instance;
        instance.from_data(source, wire);
        return instance;
    }

    // Validation.
    //-------------------------------------------------------------------------

    /// True if cached previous output is mature enough to spend from height.
    [[nodiscard]] 
    bool is_mature(size_t height) const;

    // THIS IS FOR LIBRARY USE ONLY, DO NOT CREATE A DEPENDENCY ON IT.
    mutable validation_type validation;

// protected:
//     // So that input may call reset from its own.
//     friend class input;
};

}  // namespace chain
}  // namespace kth

#endif
