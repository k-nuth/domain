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
#include <bitcoin/bitcoin/machine/operation.hpp>

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// #include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/infrastructure/formats/base_16.hpp>
#include <bitcoin/infrastructure/machine/opcode.hpp>
#include <bitcoin/infrastructure/utility/assert.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/istream_reader.hpp>
#include <bitcoin/infrastructure/utility/ostream_writer.hpp>
#include <bitcoin/infrastructure/utility/string.hpp>

namespace libbitcoin {
namespace machine {

// Deserialization.
//-----------------------------------------------------------------------------

// static
operation operation::factory_from_data(data_chunk const& encoded) {
    operation instance;
    instance.from_data(encoded);
    return instance;
}

// static
operation operation::factory_from_data(std::istream& stream) {
    operation instance;
    instance.from_data(stream);
    return instance;
}

bool operation::from_data(data_chunk const& encoded) {
    data_source istream(encoded);
    return from_data(istream);
}

bool operation::from_data(std::istream& stream) {
    istream_reader stream_r(stream);
    return from_data(stream_r);
}

// TODO(libbitcoin): optimize for larger data by using a shared byte array.
//bool operation::from_data(reader& source)

inline bool is_push_token(std::string const& token) {
    return token.size() > 1 && token.front() == '[' && token.back() == ']';
}

inline bool is_text_token(std::string const& token) {
    return token.size() > 1 && token.front() == '\'' && token.back() == '\'';
}

inline bool is_valid_data_size(opcode code, size_t size) {
    BC_CONSTEXPR auto op_75 = static_cast<uint8_t>(opcode::push_size_75);
    auto const value = static_cast<uint8_t>(code);
    return value > op_75 || value == size;
}

inline std::string trim_token(std::string const& token) {
    BITCOIN_ASSERT(token.size() > 1);
    return std::string(token.begin() + 1, token.end() - 1);
}

inline 
string_list split_push_token(std::string const& token) {
    return split(trim_token(token), ".", false);
}

static bool opcode_from_data_prefix(opcode& out_code,
                                    std::string const& prefix,
                                    data_chunk const& data) {
    BC_CONSTEXPR auto op_75 = static_cast<uint8_t>(opcode::push_size_75);
    auto const size = data.size();
    out_code = operation::opcode_from_size(size);

    if (prefix == "0") {
        return size <= op_75;
    }
    if (prefix == "1") {
        out_code = opcode::push_one_size;
        return size <= max_uint8;
    }
    if (prefix == "2") {
        out_code = opcode::push_two_size;
        return size <= max_uint16;
    }
    if (prefix == "4") {
        out_code = opcode::push_four_size;
        return size <= max_uint32;
    }

    return false;
}

static bool data_from_number_token(data_chunk& out_data,
                                   std::string const& token) {
    try {
        out_data = number(boost::lexical_cast<int64_t>(token)).data();
        return true;
    } catch (const boost::bad_lexical_cast&) {
        return false;
    }
}

// The removal of spaces in v3 data is a compatability break with our v2.
bool operation::from_string(std::string const& mnemonic) {
    reset();

    if (is_push_token(mnemonic)) {
        // Data encoding uses single token (with optional non-minimality).
        auto const parts = split_push_token(mnemonic);

        if (parts.size() == 1) {
            // Extract operation using nominal data size encoding.
            if (decode_base16(data_, parts[0])) {
                code_ = nominal_opcode_from_data(data_);
                valid_ = true;
            }
        } else if (parts.size() == 2) {
            // Extract operation using explicit data size encoding.
            valid_ = decode_base16(data_, parts[1]) &&
                     opcode_from_data_prefix(code_, parts[0], data_);
        }
    } else if (is_text_token(mnemonic)) {
        auto const text = trim_token(mnemonic);
        data_ = data_chunk{text.begin(), text.end()};
        code_ = nominal_opcode_from_data(data_);
        valid_ = true;
    } else if (opcode_from_string(code_, mnemonic)) {
        // push_one_size, push_two_size and push_four_size succeed with empty.
        // push_size_1 through push_size_75 always fail because they are empty.
        valid_ = is_valid_data_size(code_, data_.size());
    } else if (data_from_number_token(data_, mnemonic)) {
        // [-1, 0, 1..16] integers captured by opcode_from_string, others here.
        // Otherwise minimal_opcode_from_data could convert integers here.
        code_ = nominal_opcode_from_data(data_);
        valid_ = true;
    }

    if ( ! valid_) {
        reset();
    }

    return valid_;
}

bool operation::is_valid() const {
    return valid_;
}

// protected
void operation::reset() {
    code_ = invalid_code;
    data_.clear();
    valid_ = false;
}

// Serialization.
//-----------------------------------------------------------------------------

data_chunk operation::to_data() const {
    data_chunk data;
    auto const size = serialized_size();
    data.reserve(size);
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    BITCOIN_ASSERT(data.size() == size);
    return data;
}

void operation::to_data(data_sink& stream) const {
    ostream_writer sink_w(stream);
    to_data(sink_w);
}

static std::string opcode_to_prefix(opcode code, data_chunk const& data) {
    // If opcode is minimal for a size-based encoding, do not set a prefix.
    if (code == operation::opcode_from_size(data.size())) {
        return "";
    }

    switch (code) {
        case opcode::push_one_size:
            return "1.";
        case opcode::push_two_size:
            return "2.";
        case opcode::push_four_size:
            return "4.";
        default:
            return "0.";
    }
}

// The removal of spaces in v3 data is a compatability break with our v2.
std::string operation::to_string(uint32_t active_forks) const {
    if ( ! valid_) {
        return "<invalid>";
    }

    if (data_.empty()) {
        return opcode_to_string(code_, active_forks);
    }

    // Data encoding uses single token with explicit size prefix as required.
    return "[" + opcode_to_prefix(code_, data_) + encode_base16(data_) + "]";
}

}  // namespace machine
}  // namespace libbitcoin
