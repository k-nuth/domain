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
#include <bitcoin/infrastructure/math/hash.hpp>

// #include <algorithm>
// #include <cstddef>
// #include <cstdint>
// #include <errno.h>
// #include <new>
// #include <stdexcept>
// #include "../math/external/crypto_scrypt.h"
// #include "../math/external/hmac_sha256.h"
// #include "../math/external/hmac_sha512.h"
// #include "../math/external/pkcs5_pbkdf2.h"
// #include "../math/external/ripemd160.h"
// #include "../math/external/sha1.h"
// #include "../math/external/sha256.h"
// #include "../math/external/sha512.h"

#ifdef KNUTH_CURRENCY_LTC
#include "../math/external/scrypt.h"
#endif  //KNUTH_CURRENCY_LTC

namespace libbitcoin {

#ifdef KNUTH_CURRENCY_LTC
hash_digest litecoin_hash(data_slice data) {
    hash_digest hash;
    scrypt_1024_1_1_256(reinterpret_cast<char const*>(data.data()),
                        reinterpret_cast<char*>(hash.data()));
    return hash;
}
#endif  //KNUTH_CURRENCY_LTC

}  // namespace libbitcoin
