/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/message/fee_filter.hpp>

#include <bitcoin/bitcoin/message/version.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>

namespace libbitcoin {
namespace message {

std::string const fee_filter::command = "feefilter";
uint32_t const fee_filter::version_minimum = version::level::bip133;
uint32_t const fee_filter::version_maximum = version::level::bip133;

fee_filter fee_filter::factory_from_data(uint32_t version, data_chunk const& data) {
    fee_filter instance;
    instance.from_data(version, data);
    return instance;
}

fee_filter fee_filter::factory_from_data(uint32_t version, std::istream& stream) {
    fee_filter instance;
    instance.from_data(version, stream);
    return instance;
}

size_t fee_filter::satoshi_fixed_size(uint32_t /*version*/) {
    return sizeof(minimum_fee_);
}

// This is a default instance so is invalid.
fee_filter::fee_filter()
    : minimum_fee_(0), insufficient_version_(true) {
}

// This is not a default instance so is valid.
fee_filter::fee_filter(uint64_t minimum)
    : minimum_fee_(minimum), insufficient_version_(false) {
}

// protected
fee_filter::fee_filter(uint64_t minimum, bool insufficient_version)
    : minimum_fee_(minimum), insufficient_version_(insufficient_version) {
}

// fee_filter::fee_filter(const fee_filter& x)
//     : fee_filter(x.minimum_fee_, x.insufficient_version_) {
// }

// fee_filter::fee_filter(fee_filter&& x) noexcept
//     : fee_filter(x.minimum_fee_, x.insufficient_version_) {
// }

// fee_filter& fee_filter::operator=(fee_filter&& x) noexcept {
//     minimum_fee_ = x.minimum_fee_;
//     insufficient_version_ = x.insufficient_version_;
//     return *this;
// }

//TODO(fernando): it does not compare all the data members, is it OK?
bool fee_filter::operator==(const fee_filter& x) const {
    return (minimum_fee_ == x.minimum_fee_);
}

bool fee_filter::operator!=(const fee_filter& x) const {
    return !(*this == x);
}

bool fee_filter::from_data(uint32_t version, data_chunk const& data) {
    data_source istream(data);
    return from_data(version, istream);
}

bool fee_filter::from_data(uint32_t version, std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(version, stream_r);
}

data_chunk fee_filter::to_data(uint32_t version) const {
    data_chunk data;
    auto const size = serialized_size(version);
    data.reserve(size);
    data_sink ostream(data);
    to_data(version, ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void fee_filter::to_data(uint32_t version, data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(version, sink_w);
}

bool fee_filter::is_valid() const {
    return !insufficient_version_ || (minimum_fee_ > 0);
}

// This is again a default instance so is invalid.
void fee_filter::reset() {
    insufficient_version_ = true;
    minimum_fee_ = 0;
}

size_t fee_filter::serialized_size(uint32_t version) const {
    return satoshi_fixed_size(version);
}

uint64_t fee_filter::minimum_fee() const {
    return minimum_fee_;
}

void fee_filter::set_minimum_fee(uint64_t value) {
    minimum_fee_ = value;

    // This is no longer a default instance, so is valid.
    insufficient_version_ = false;
}

}  // namespace message
}  // namespace libbitcoin
