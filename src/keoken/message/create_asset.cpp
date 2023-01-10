// Copyright (c) 2016-2023 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <kth/domain/keoken/message/create_asset.hpp>

#include <kth/infrastructure/utility/istream_reader.hpp>
#include <kth/infrastructure/utility/ostream_writer.hpp>

#include <kth/domain/keoken/message/base.hpp>
#include <kth/domain/keoken/utility.hpp>

namespace kth::keoken::message {

using kth::data_chunk;
using kth::data_sink;
using kth::data_source;
using kth::istream_reader;
using kth::ostream_writer;
// using kth::writer;

// Constructors.
//-------------------------------------------------------------------------

create_asset::create_asset(std::string name, amount_t amount)
    : name_(std::move(name)), amount_(amount)
{}

// Operators.
//-----------------------------------------------------------------------------

// friend
bool operator==(create_asset const& a, create_asset const& b) {
    return a.name_ == b.name_ && a.amount_ == b.amount_;
}

// friend
bool operator!=(create_asset const& a, create_asset const& b) {
    return !(a == b);
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk create_asset::to_data() const {
    data_chunk data;
    auto const size = serialized_size();
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    KTH_ASSERT(data.size() == size);
    return data;
}

void create_asset::to_data(data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(sink_w);
}

// Properties (size, accessors, cache).
//-----------------------------------------------------------------------------

size_t create_asset::serialized_size() const {
    return base::serialized_size() +
           sizeof(amount_) +
           name_.size() +
           1;  //null terminated string
}

std::string const& create_asset::name() const {
    return name_;
}

void create_asset::set_name(std::string const& x) {
    name_ = x;
}

void create_asset::set_name(std::string&& x) {
    name_ = std::move(x);
}

amount_t create_asset::amount() const {
    return amount_;
}

void create_asset::set_amount(amount_t x) {
    amount_ = x;
}

} // namespace kth::keoken::message
