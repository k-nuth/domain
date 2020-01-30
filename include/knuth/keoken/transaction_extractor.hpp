/**
 * Copyright (c) 2016-2018 Knuth Project.
 *
 * This file is part of Knuth Project.
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
#ifndef KTH_KEOKEN_TRANSACTION_EXTRACTOR_HPP_
#define KTH_KEOKEN_TRANSACTION_EXTRACTOR_HPP_

#include <kth/domain/chain/transaction.hpp>
#include <kth/infrastructure/utility/data.hpp>

namespace knuth {
namespace keoken {

bc::data_chunk first_keoken_output(bc::chain::transaction const& tx);

}  // namespace keoken
}  // namespace knuth

#endif  //KTH_KEOKEN_TRANSACTION_EXTRACTOR_HPP_
