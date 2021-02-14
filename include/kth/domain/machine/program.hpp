// Copyright (c) 2016-2021 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MACHINE_PROGRAM_HPP
#define KTH_DOMAIN_MACHINE_PROGRAM_HPP

#include <cstdint>

#include <kth/domain/chain/script.hpp>
#include <kth/domain/chain/transaction.hpp>
#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/domain/machine/opcode.hpp>
#include <kth/domain/machine/operation.hpp>
#include <kth/infrastructure/machine/number.hpp>
#include <kth/infrastructure/machine/script_version.hpp>
#include <kth/infrastructure/utility/data.hpp>

namespace kth::domain::machine {

using operation = ::kth::domain::machine::operation;        //TODO(fernando): why this?
using script_version = ::kth::infrastructure::machine::script_version;
using number = ::kth::infrastructure::machine::number;

class KD_API program {
public:
    using value_type = data_stack::value_type;
    using op_iterator = operation::iterator;

    //TODO(fernando): check this comment 
    // Older libstdc++ does not allow erase with const iterator.
    // This is a bug that requires we up the minimum compiler version.
    // So presently stack_iterator is a non-const iterator.
    ////using stack_iterator = data_stack::const_iterator;
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
    [[nodiscard]]
    bool is_valid() const;

    [[nodiscard]]
    uint32_t forks() const;

    [[nodiscard]]
    uint32_t input_index() const;

    [[nodiscard]]
    uint64_t value() const;

    [[nodiscard]]
    script_version version() const;

    [[nodiscard]]
    chain::transaction const& transaction() const;

    /// Program registers.
    [[nodiscard]]
    op_iterator begin() const;

    [[nodiscard]]
    op_iterator jump() const;

    [[nodiscard]]
    op_iterator end() const;

    [[nodiscard]]
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
    [[nodiscard]]
    bool empty() const;

    [[nodiscard]]
    bool stack_true(bool clean) const;

    [[nodiscard]]
    bool stack_result(bool clean) const;

    [[nodiscard]]
    bool is_stack_overflow() const;

    [[nodiscard]]
    bool if_(operation const& op) const;
    
    [[nodiscard]]
    value_type const& item(size_t index) const;


    value_type& item(size_t index);

    bool top(number& out_number, size_t maxiumum_size = max_number_size) const;

    [[nodiscard]]
    stack_iterator position(size_t index) const;

    stack_mutable_iterator position(size_t index);

    [[nodiscard]]
    operation::list subscript() const;

    [[nodiscard]]
    size_t size() const;

    // Alternate stack.
    //-------------------------------------------------------------------------

    [[nodiscard]]
    bool empty_alternate() const;

    void push_alternate(value_type&& value);
    value_type pop_alternate();

    // Conditional stack.
    //-------------------------------------------------------------------------

    void open(bool value);
    void negate();
    void close();

    [[nodiscard]]
    bool closed() const;

    [[nodiscard]]
    bool succeeded() const;

private:
    // A space-efficient dynamic bitset (specialized).
    using bool_stack = std::vector<bool>;

    void reserve_stacks();

    [[nodiscard]]
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

} // namespace kth::domain::machine

#include <kth/domain/impl/machine/program.ipp>

#endif
