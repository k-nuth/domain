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

#include <bitcoin/bitcoin/utility/property_tree.hpp>

#include <cstdint>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include <bitcoin/bitcoin/config/header.hpp>
#include <bitcoin/bitcoin/config/input.hpp>
#include <bitcoin/bitcoin/config/output.hpp>
#include <bitcoin/bitcoin/config/transaction.hpp>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/math/stealth.hpp>
#include <bitcoin/infrastructure/config/base16.hpp>
#include <bitcoin/infrastructure/config/hash160.hpp>
#include <bitcoin/infrastructure/config/hash256.hpp>
#include <bitcoin/infrastructure/utility/collection.hpp>

namespace libbitcoin {

using namespace pt;
using namespace bc::config;
using namespace bc::machine;
using namespace bc::wallet;

// property_tree is very odd in that what one might consider a node or element,
// having a "containing" name cannot be added into another node without
// creating an intervening name/container. so we create uncontained nodes and
// lists which can then be added to a parent node, creating the named container
// on the add.

// Edit with care - text property names trade DRY for readability.
// Edit with care - tests are affected by property ORDER (keep alphabetical).

// headers

ptree property_list(const config::header& header) {
    chain::header const& block_header = header;

    ptree tree;
    tree.put("bits", block_header.bits());
    tree.put("hash", hash256(block_header.hash()));
    tree.put("merkle_tree_hash", hash256(block_header.merkle()));
    tree.put("nonce", block_header.nonce());
    tree.put("previous_block_hash", hash256(block_header.previous_block_hash()));
    tree.put("time_stamp", block_header.timestamp());
    tree.put("version", block_header.version());
    return tree;
}

ptree property_tree(const config::header& header) {
    ptree tree;
    tree.add_child("header", property_list(header));
    return tree;
}

ptree property_tree(const std::vector<config::header>& headers, bool json) {
    ptree tree;
    tree.add_child("headers", property_tree_list("header", headers, json));
    return tree;
}

// inputs

ptree property_list(chain::input const& tx_input) {
    ptree tree;

    // This does not support pay_multisig or pay_public_key (nonstandard).
    // This will have default versioning, but the address version is unused.
    auto const address = tx_input.address();

    if (address) {
        tree.put("address_hash", hash160(address.hash()));
    }

    tree.put("previous_output.hash", hash256(tx_input.previous_output().hash()));
    tree.put("previous_output.index", tx_input.previous_output().index());
    tree.put("script", tx_input.script().to_string(rule_fork::all_rules));
    tree.put("sequence", tx_input.sequence());

#ifndef KNUTH_CURRENCY_BCH
    if (tx_input.is_segregated()) {
        tree.put("witness", tx_input.witness().to_string());
    }
#endif

    return tree;
}

ptree property_tree(chain::input const& tx_input) {
    ptree tree;
    tree.add_child("input", property_list(tx_input));
    return tree;
}

ptree property_tree(const chain::input::list& tx_inputs, bool json) {
    ptree tree;
    tree.add_child("inputs", property_tree_list("input", tx_inputs, json));
    return tree;
}

ptree property_list(const config::input& input) {
    chain::input const& tx_input = input;
    return property_list(tx_input);
}

ptree property_tree(const config::input& input) {
    ptree tree;
    tree.add_child("input", property_list(input));
    return tree;
}

ptree property_tree(const std::vector<config::input>& inputs, bool json) {
    auto const tx_inputs = cast<input, chain::input>(inputs);

    ptree tree;
    tree.add_child("inputs", property_tree_list("input", tx_inputs, json));
    return tree;
}

// outputs

ptree property_list(const chain::output& tx_output) {
    ptree tree;

    // This does not support pay_multisig or pay_public_key (nonstandard).
    // This will have default versioning, but the address version is unused.
    auto const address = tx_output.address();

    if (address) {
        tree.put("address_hash", hash160(address.hash()));
    }

    tree.put("script", tx_output.script().to_string(rule_fork::all_rules));

    // TODO(libbitcoin): this will eventually change due to privacy problems, see:
    // lists.dyne.org/lurker/message/20140812.214120.317490ae.en.html

    if ( ! address) {
        uint32_t stealth_prefix;
        ec_compressed ephemeral_key;

        if (to_stealth_prefix(stealth_prefix, tx_output.script()) &&
            extract_ephemeral_key(ephemeral_key, tx_output.script())) {
            tree.put("stealth.prefix", stealth_prefix);
            tree.put("stealth.ephemeral_public_key", ec_public(ephemeral_key));
        }
    }

    tree.put("value", tx_output.value());
    return tree;
}

ptree property_tree(const chain::output& tx_output) {
    ptree tree;
    tree.add_child("output", property_list(tx_output));
    return tree;
}

ptree property_tree(const chain::output::list& tx_outputs, bool json) {
    ptree tree;
    tree.add_child("outputs", property_tree_list("output", tx_outputs, json));
    return tree;
}

// points

ptree property_list(const chain::point_value& point) {
    ptree tree;
    tree.put("hash", hash256(point.hash()));
    tree.put("index", point.index());
    tree.put("value", point.value());
    return tree;
}

ptree property_tree(const chain::points_value& values, bool json) {
    ptree tree;
    tree.add_child("points", property_tree_list("point", values.points, json));
    return tree;
}

// transactions

ptree property_list(const config::transaction& transaction, bool json) {
    chain::transaction const& tx = transaction;

    ptree tree;
    tree.put("hash", hash256(tx.hash()));
    tree.add_child("inputs", property_tree_list("input", tx.inputs(), json));
    tree.put("lock_time", tx.locktime());
    tree.add_child("outputs", property_tree_list("output", tx.outputs(), json));
    tree.put("version", tx.version());
    return tree;
}

ptree property_tree(const config::transaction& transaction, bool json) {
    ptree tree;
    tree.add_child("transaction", property_list(transaction, json));
    return tree;
}

ptree property_tree(const std::vector<config::transaction>& transactions, bool json) {
    ptree tree;
    tree.add_child("transactions", property_tree_list_of_lists("transaction",
                                                               transactions, json));
    return tree;
}

// wrapper

ptree property_list(const wallet::wrapped_data& wrapper) {
    ptree tree;
    tree.put("checksum", wrapper.checksum);
    tree.put("payload", base16(wrapper.payload));
    tree.put("version", wrapper.version);
    return tree;
}

ptree property_tree(const wallet::wrapped_data& wrapper) {
    ptree tree;
    tree.add_child("wrapper", property_list(wrapper));
    return tree;
}

// metadata

ptree property_list(hash_digest const& hash, size_t height, size_t index) {
    ptree tree;
    tree.put("hash", hash256(hash));
    tree.put("height", height);
    tree.put("index", index);
    return tree;
}

ptree property_tree(hash_digest const& hash, size_t height, size_t index) {
    ptree tree;
    tree.add_child("metadata", property_list(hash, height, index));
    return tree;
}

// settings

ptree property_tree(const settings_list& settings) {
    ptree list;

    for (auto const& setting : settings) {
        list.put(setting.first, setting.second);
    }

    ptree tree;
    tree.add_child("settings", list);
    return tree;
}

}  // namespace libbitcoin
