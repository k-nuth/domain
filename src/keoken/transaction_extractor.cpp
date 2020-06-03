// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <kth/domain/keoken/transaction_extractor.hpp>

#include <type_traits>

#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/machine/operation.hpp>

#include <kth/domain/keoken/constants.hpp>

namespace kth::keoken {

using kth::data_chunk;
using kth::domain::machine::opcode;
using kth::domain::machine::operation;

template <typename I>
data_chunk get_keoken_data(I f, I l) {
    //precondition:
    //postcondition: if return value is an empty vector, it is not a keoken output.

    if (f == l) {
        return data_chunk{};
    }

    if (f->code() != opcode::return_) {
        return data_chunk{};
    }

    ++f;  //move to the next machine::operation
    if (f == l) {
        return data_chunk{};
    }

    if (f->code() != opcode::push_size_4) {
        return data_chunk{};
    }

    if ( ! std::equal(f->data().begin(), f->data().end(), static_cast<uint8_t const*>(protocol_name))) {
        return data_chunk{};
    }

    ++f;  //move to the next machine::operation
    if (f == l) {
        return data_chunk{};
    }

    if (to_underlying(f->code()) < kp_min_size) {
        return data_chunk{};
    }

    if (to_underlying(f->code()) > kp_max_size) {
        return data_chunk{};
    }

    return f->data();  //TODO(fernando): check if we can std::move this vector
}

data_chunk get_keoken_data(operation::list const& ops) {
    return get_keoken_data(ops.begin(), ops.end());
}

data_chunk first_keoken_output(kth::domain::chain::transaction const& tx) {
    //precondition: tx is a confirmed transaction
    for (auto const& out : tx.outputs()) {
        auto const& keo_data = get_keoken_data(out.script().operations());
        if ( ! keo_data.empty()) {
            return keo_data;
        }
    }
    return data_chunk{};
}

} // namespace keoken
} // namespace kth
