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
#ifndef LIBBITCOIN_MACHINE_PROGRAM_HPP
#define LIBBITCOIN_MACHINE_PROGRAM_HPP

#include <cstdint>

#include <bitcoin/bitcoin/chain/script.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/constants.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/machine/operation.hpp>
#include <bitcoin/infrastructure/machine/number.hpp>
#include <bitcoin/bitcoin/machine/opcode.hpp>
#include <bitcoin/infrastructure/machine/script_version.hpp>
#include <bitcoin/infrastructure/utility/data.hpp>

namespace libbitcoin {
namespace machine {

class BC_API program {
public:
    using value_type = data_stack::value_type;
    using op_iterator = operation::iterator;

    // Older libstdc++ does not allow erase with const iterator.
    // This is a bug that requires we up the minimum compiler version.
    // So presently stack_iterator is a non-const iterator.
    ////typedef data_stack::const_iterator stack_iterator;
    // using stack_iterator = data_stack::iterator;
    using stack_iterator = data_stack::const_iterator;
    using stack_mutable_iterator = data_stack::iterator;

    /// Create an instance that does not expect to verify signatures.
    /// This is useful for script utilities but not with input validation.
    /// This can only run individual operations via run(op, program).
    program();

    /// Create an instance that does not expect to verify signatures.
    /// This is useful for script utilities but not with input validation.
    /// This can run ops via run(op, program) or the script via run(program).
    program(chain::script const& script);

    /// Create an instance with empty stacks, value unused/max (input run).
    program(chain::script const& script, chain::transaction const& transaction, uint32_t input_index, uint32_t forks);

    /// Create an instance with initialized stack (witness run, v0 by default).
    program(chain::script const& script, chain::transaction const& transaction, uint32_t input_index, uint32_t forks, data_stack&& stack, uint64_t value, script_version version = script_version::zero);

    /// Create using copied tx, input, forks, value, stack (prevout run).
    program(chain::script const& script, const program& x);

    /// Create using copied tx, input, forks, value and moved stack (p2sh run).
    program(chain::script const& script, program&& x, bool move);

    /// Constant registers.
    bool is_valid() const;
    uint32_t forks() const;
    uint32_t input_index() const;
    uint64_t value() const;
    script_version version() const;
    chain::transaction const& transaction() const;

    /// Program registers.
    op_iterator begin() const;
    op_iterator jump() const;
    op_iterator end() const;
    size_t operation_count() const;

    /// Instructions.
    code evaluate();
    code evaluate(operation const& op);
    bool increment_operation_count(operation const& op);
    bool increment_operation_count(int32_t public_keys);
    bool set_jump_register(operation const& op, int32_t offset);

    // Primary stack.
    //-------------------------------------------------------------------------

    /// Primary push.
    void push(bool value);
    void push_move(value_type&& item);
    void push_copy(value_type const& item);

    /// Primary pop.
    data_chunk pop();
    bool pop(int32_t& out_value);
    bool pop(number& out_number, size_t maxiumum_size = max_number_size);
    bool pop_binary(number& first, number& second);
    bool pop_ternary(number& first, number& second, number& third);
    bool pop_position(stack_iterator& out_position);
    bool pop(data_stack& section, size_t count);

    /// Primary push/pop optimizations (active).
    void duplicate(size_t index);

    void swap(size_t index_left, size_t index_right);

    void erase(stack_iterator const& position);
    void erase(stack_iterator const& first, stack_iterator const& last);

    /// Primary push/pop optimizations (passive).
    bool empty() const;
    bool stack_true(bool clean) const;
    bool stack_result(bool clean) const;
    bool is_stack_overflow() const;
    bool if_(operation const& op) const;
    
    value_type const& item(size_t index) const;
    value_type& item(size_t index);

    bool top(number& out_number, size_t maxiumum_size = max_number_size) const;
    // bool top(number& out_number, size_t maxiumum_size = max_number_size) /*const*/;

    stack_iterator position(size_t index) const;
    stack_mutable_iterator position(size_t index);



    operation::list subscript() const;
    size_t size() const;

    // Alternate stack.
    //-------------------------------------------------------------------------

    bool empty_alternate() const;
    void push_alternate(value_type&& value);
    value_type pop_alternate();

    // Conditional stack.
    //-------------------------------------------------------------------------

    void open(bool value);
    void negate();
    void close();
    bool closed() const;
    bool succeeded() const;

private:
    // A space-efficient dynamic bitset (specialized).
    using bool_stack = std::vector<bool>;

    void reserve_stacks();
    bool stack_to_bool(bool clean) const;

    chain::script const& script_;
    chain::transaction const& transaction_;
    uint32_t const input_index_{0};
    uint32_t const forks_{0};
    uint64_t const value_{0};

    script_version version_{script_version::unversioned};
    size_t negative_count_{0};
    size_t operation_count_{0};
    op_iterator jump_;
    data_stack primary_;
    data_stack alternate_;
    bool_stack condition_;
};

}  // namespace machine
}  // namespace libbitcoin

#include <bitcoin/bitcoin/impl/machine/program.ipp>

#endif
