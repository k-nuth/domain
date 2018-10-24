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
#ifndef LIBBITCOIN_CHAIN_WITNESS_HPP
#define LIBBITCOIN_CHAIN_WITNESS_HPP

#include <cstddef>
#include <istream>
#include <string>

#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/machine/operation.hpp>
#include <bitcoin/infrastructure/utility/container_sink.hpp>
#include <bitcoin/infrastructure/utility/container_source.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>
#include <bitcoin/infrastructure/utility/reader.hpp>
#include <bitcoin/infrastructure/utility/thread.hpp>
#include <bitcoin/infrastructure/utility/writer.hpp>

#include <bitprim/common.hpp>
#include <bitprim/concepts.hpp>

namespace libbitcoin {
namespace chain {

class BC_API witness {
public:
    typedef machine::operation operation;
    typedef data_stack::const_iterator iterator;

    // Constructors.
    //-------------------------------------------------------------------------

    witness();

    witness(witness&& x);
    witness(witness const& x);

    witness(data_stack&& stack);
    witness(data_stack const& stack);

    witness(data_chunk&& encoded, bool prefix);
    witness(data_chunk const& encoded, bool prefix);

    // Operators.
    //-------------------------------------------------------------------------

    /// This class is move assignable and copy assignable.
    witness& operator=(witness&& x);
    witness& operator=(witness const& x);

    bool operator==(witness const& x) const;
    bool operator!=(witness const& x) const;

    // Deserialization (from witness stack).
    //-------------------------------------------------------------------------
    // Prefixed data assumed valid here though caller may confirm with is_valid.

    static witness factory_from_data(data_chunk const& encoded, bool prefix);
    static witness factory_from_data(data_source& stream, bool prefix);

    template <Reader R, BITPRIM_IS_READER(R)>
    static witness factory_from_data(R& source, bool prefix) {
        witness instance;
        instance.from_data(source, prefix);
        return instance;
    }

    //static witness factory_from_data(reader& source, bool prefix);

    /// Deserialization invalidates the iterator.
    bool from_data(data_chunk const& encoded, bool prefix);
    bool from_data(data_source& stream, bool prefix);

    template <Reader R, BITPRIM_IS_READER(R)>
    bool from_data(R& source, bool prefix) {
        reset();
        valid_ = true;

        auto const read_element = [](R& source) {
            // Tokens encoded as variable integer prefixed byte array (bip144).
            auto const size = source.read_size_little_endian();

            // The max_script_size and max_push_data_size constants limit
            // evaluation, but not all stacks evaluate, so use max_block_weight
            // to guard memory allocation here.
            if (size > max_block_weight) {
                source.invalidate();
                return data_chunk{};
            }

            return source.read_bytes(size);
        };

        // TODO(libbitcoin): optimize store serialization to avoid loop, reading data directly.
        if (prefix) {
            // Witness prefix is an element count, not byte length (unlike script).
            // On wire each witness is prefixed with number of elements (bip144).
            for (auto count = source.read_size_little_endian(); count > 0; --count)
                stack_.push_back(read_element(source));
        } else {
            while (!source.is_exhausted())
                stack_.push_back(read_element(source));
        }

        if ( ! source)
            reset();

        return source;
    }

    //bool from_data(reader& source, bool prefix);

    /// The witness deserialized ccording to count and size prefixing.
    bool is_valid() const;

    // Serialization.
    //-------------------------------------------------------------------------

    data_chunk to_data(bool prefix) const;
    void to_data(data_sink& stream, bool prefix) const;

    template <Writer W>
    void to_data(W& sink, bool prefix) const {
        // Witness prefix is an element count, not byte length (unlike script).
        if (prefix)
            sink.write_size_little_endian(stack_.size());

        auto const serialize = [&sink](data_chunk const& element) {
            // Tokens encoded as variable integer prefixed byte array (bip144).
            sink.write_size_little_endian(element.size());
            sink.write_bytes(element);
        };

        // TODO(libbitcoin): optimize store serialization to avoid loop, writing data directly.
        std::for_each(stack_.begin(), stack_.end(), serialize);
    }

    //void to_data(writer& sink, bool prefix) const;

    std::string to_string() const;

    // Iteration.
    //-------------------------------------------------------------------------

    void clear();
    bool empty() const;
    size_t size() const;
    data_chunk const& front() const;
    data_chunk const& back() const;
    iterator begin() const;
    iterator end() const;
    data_chunk const& operator[](size_t index) const;

    // Properties (size, accessors, cache).
    //-------------------------------------------------------------------------

    size_t serialized_size(bool prefix) const;
    data_stack const& stack() const;

    // Utilities.
    //-------------------------------------------------------------------------

    static bool is_push_size(data_stack const& stack);
    static bool is_reserved_pattern(data_stack const& stack);

    bool extract_sigop_script(script& out_script,
                              script const& program_script) const;
    bool extract_embedded_script(script& out_script, data_stack& out_stack, script const& program_script) const;

    // Validation.
    //-------------------------------------------------------------------------

    code verify(transaction const& tx, uint32_t input_index, uint32_t forks, script const& program_script, uint64_t value) const;

protected:
    // So that input may call reset from its own.
    friend class input;

    void reset();

private:
    static size_t serialized_size(data_stack const& stack);
    static operation::list to_pay_key_hash(data_chunk&& program);

    bool valid_;
    data_stack stack_;
};

}  // namespace chain
}  // namespace libbitcoin

//#include <bitprim/concepts_undef.hpp>

#endif
