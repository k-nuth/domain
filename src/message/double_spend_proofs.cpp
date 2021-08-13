// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/message/double_spend_proofs.hpp>

#include <initializer_list>

// #include <kth/infrastructure/message/message_tools.hpp>
#include <kth/domain/message/version.hpp>
#include <kth/domain/multi_crypto_support.hpp>
#include <kth/infrastructure/math/sip_hash.hpp>
#include <kth/infrastructure/message/message_tools.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/limits.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>
#include <kth/infrastructure/utility/pseudo_random.hpp>

namespace kth::domain::message {

std::string const double_spend_proofs::command = "dsproof-beta";
uint32_t const double_spend_proofs::version_minimum = version::level::minimum;
uint32_t const double_spend_proofs::version_maximum = version::level::maximum;

double_spend_proofs::double_spend_proofs(chain::output_point const& out_point, spender const& spender1, spender const& spender2) 
    : out_point_(out_point)
    , spender1_(spender1)
    , spender2_(spender2)
{}

bool double_spend_proofs::is_valid() const {
    return out_point_.is_valid() 
        && spender1_.is_valid() 
        && spender2_.is_valid();
}

void double_spend_proofs::reset() {
    out_point_.reset();
    spender1_.reset();
    spender2_.reset();
}

data_chunk double_spend_proofs::to_data() const {
    data_chunk data;
    auto const size = serialized_size();
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void double_spend_proofs::to_data(data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(sink_w);
}


[[nodiscard]]
chain::output_point const& double_spend_proofs::out_point() const {
    return out_point_;
}

void double_spend_proofs::set_out_point(chain::output_point const& x) {
    out_point_ = x;
}

[[nodiscard]]
double_spend_proofs::spender const& double_spend_proofs::spender1() const {
    return spender1_;
}

void double_spend_proofs::set_spender1(double_spend_proofs::spender const& x) {
    spender1_ = x;
}

[[nodiscard]]
double_spend_proofs::spender const& double_spend_proofs::spender2() const {
    return spender2_;
}

void double_spend_proofs::set_spender2(double_spend_proofs::spender const& x) {
    spender2_ = x;
}

hash_digest hash(double_spend_proofs const& x) {
    return sha256_hash(x.to_data());
}

} // namespace kth::domain::message
