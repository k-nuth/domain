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
#ifndef LIBBITCOIN_MACHINE_OPERATION_HPP
#define LIBBITCOIN_MACHINE_OPERATION_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/infrastructure/define.hpp>
#include <bitcoin/infrastructure/machine/opcode.hpp>
#include <bitcoin/infrastructure/machine/script_pattern.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace machine {

class BI_API operation {
public:
    using list = std::vector<operation>;
    using iterator = list::const_iterator;

    // Constructors.
    //-------------------------------------------------------------------------

    operation();

    operation(operation&& x) noexcept;
    operation(operation const& x);

    operation(data_chunk&& uncoded, bool minimal = true);
    operation(data_chunk const& uncoded, bool minimal = true);

    operation(opcode code);

    // Operators.
    //-------------------------------------------------------------------------

    operation& operator=(operation&& x) noexcept;
    operation& operator=(operation const& x);

    bool operator==(operation const& x) const;
    bool operator!=(operation const& x) const;

    // Deserialization.
    //-------------------------------------------------------------------------

    static operation factory_from_data(data_chunk const& encoded);
    static operation factory_from_data(data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    static operation factory_from_data(R& source) {
        operation instance;
        instance.from_data(source);
        return instance;
    }

    //static operation factory_from_data(reader& source);

    bool from_data(data_chunk const& encoded);
    bool from_data(data_source& stream);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(R& source) {
        ////reset();
        valid_ = true;
        code_ = static_cast<opcode>(source.read_byte());
        auto const size = read_data_size(code_, source);

        // The max_script_size and max_push_data_size constants limit
        // evaluation, but not all scripts evaluate, so use max_block_size
        // to guard memory allocation here.
        if (size > get_max_block_size())  //TODO(bitprim): max_block_size changed to get_max_block_size (check space for BCH)
            source.invalidate();
        else
            data_ = source.read_bytes(size);

        if ( ! source)
            reset();

        return valid_;
    }

    //bool from_data(reader& source);

    bool from_string(std::string const& mnemonic);

    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data() const;
    void to_data(data_sink& stream) const;

    template <Writer W>
    void to_data(W& sink) const {
        auto const size = data_.size();

        sink.write_byte(static_cast<uint8_t>(code_));

        switch (code_) {
            case opcode::push_one_size:
                sink.write_byte(static_cast<uint8_t>(size));
                break;
            case opcode::push_two_size:
                sink.write_2_bytes_little_endian(static_cast<uint16_t>(size));
                break;
            case opcode::push_four_size:
                sink.write_4_bytes_little_endian(static_cast<uint32_t>(size));
                break;
            default:
                break;
        }

        sink.write_bytes(data_);
    }

    //void to_data(writer& sink) const;

    std::string to_string(uint32_t active_forks) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size() const;

    /// Get the op code [0..255], if is_valid is consistent with data.
    opcode code() const;

    /// Get the data, empty if not a push code or if invalid.
    data_chunk const& data() const;

    // Utilities.
    //-------------------------------------------------------------------------

    /// Compute nominal data opcode based on size alone.
    static opcode opcode_from_size(size_t size);

    /// Compute the minimal data opcode for a given chunk of data.
    /// Caller should clear data if converting to non-payload opcode.
    static opcode minimal_opcode_from_data(data_chunk const& data);

    /// Compute the nominal data opcode for a given chunk of data.
    /// Restricted to sized data, avoids conversion to numeric opcodes.
    static opcode nominal_opcode_from_data(data_chunk const& data);

    /// Convert the [1..16] value to the corresponding opcode (or undefined).
    static opcode opcode_from_positive(uint8_t value);

    /// Convert the opcode to the corresponding [1..16] value (or undefined).
    static uint8_t opcode_to_positive(opcode code);

    /// Categories of opcodes.
    static bool is_push(opcode code);
    static bool is_payload(opcode code);
    static bool is_counted(opcode code);
    static bool is_version(opcode code);
    static bool is_numeric(opcode code);
    static bool is_positive(opcode code);
    static bool is_reserved(opcode code);
    static bool is_disabled(opcode code);
    static bool is_conditional(opcode code);
    static bool is_relaxed_push(opcode code);

    /// Categories of operations.
    bool is_push() const;
    bool is_counted() const;
    bool is_version() const;
    bool is_positive() const;
    bool is_disabled() const;
    bool is_conditional() const;
    bool is_relaxed_push() const;
    bool is_oversized() const;
    bool is_minimal_push() const;
    bool is_nominal_push() const;

protected:
    operation(opcode code, data_chunk&& data, bool valid);
    operation(opcode code, data_chunk const& data, bool valid);

    template <typename R>
    static uint32_t read_data_size(opcode code, R& source);

    opcode opcode_from_data(data_chunk const& data, bool minimal);
    void reset();

private:
    opcode code_;
    data_chunk data_;
    bool valid_;
};

}  // namespace machine
}  // namespace libbitcoin

#include <bitcoin/bitcoin/impl/machine/operation.ipp>

#endif
