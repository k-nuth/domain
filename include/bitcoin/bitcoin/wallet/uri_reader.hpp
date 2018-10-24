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
#ifndef LIBBITCOIN_WALLET_URI_READER_HPP
#define LIBBITCOIN_WALLET_URI_READER_HPP

#include <string>

#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/infrastructure/wallet/uri.hpp>

namespace libbitcoin {
namespace wallet {

/**
 * Interface for URI deserialization.
 * The URI parser calls these methods as it extracts each URI component.
 * A false return from any setter is expected to terminate the parser.
 */
class BC_API uri_reader {
public:
    /**
     * Parses any URI string into its individual components.
     * @param[in]  uri     The URI to parse.
     * @param[in]  strict  Only accept properly-escaped parameters.
     * @return The parsed URI or a default instance if the `uri` is malformed
     * according to the  `UriReader`.
     */
    template <class UriReader>
    static UriReader parse(std::string const& uri, bool strict = true) {
        wallet::uri parsed;
        if ( ! parsed.decode(uri, strict))
            return UriReader();

        UriReader out;
        out.set_strict(strict);
        out.set_scheme(parsed.scheme());
        if (parsed.has_authority() && !out.set_authority(parsed.authority()))
            return UriReader();

        if ( ! parsed.path().empty() && !out.set_path(parsed.path()))
            return UriReader();

        if (parsed.has_fragment() && !out.set_fragment(parsed.fragment()))
            return UriReader();

        auto const query = parsed.decode_query();
        for (auto const& term : query) {
            auto const& key = term.first;
            auto const& value = term.second;
            if ( ! key.empty() && !out.set_parameter(key, value))
                return UriReader();
        }

        return out;
    }

    /// uri_reader interface.
    virtual void set_strict(bool strict) = 0;
    virtual bool set_scheme(std::string const& scheme) = 0;
    virtual bool set_authority(std::string const& authority) = 0;
    virtual bool set_path(std::string const& path) = 0;
    virtual bool set_fragment(std::string const& fragment) = 0;
    virtual bool set_parameter(std::string const& key,
                               std::string const& value) = 0;
};

}  // namespace wallet
}  // namespace libbitcoin

#endif
