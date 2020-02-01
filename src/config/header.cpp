// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
#include <bitcoin/bitcoin/config/header.hpp>
=======
#include <kth/domain/config/header.hpp>
>>>>>>> dev

#include <iostream>
#include <sstream>
#include <string>

#include <boost/program_options.hpp>

#include <kth/domain/chain/header.hpp>
#include <kth/infrastructure/config/base16.hpp>

namespace kth::config {

using namespace boost::program_options;

header::header(std::string const& hexcode) {
    std::stringstream(hexcode) >> *this;
}

header::header(chain::header const& value)
    : value_(value) {
}

header::header(header const& x)
    : header(x.value_) {
}

header::operator chain::header const&() const {
    return value_;
}

std::istream& operator>>(std::istream& input, header& argument) {
    std::string hexcode;
    input >> hexcode;

    if ( ! argument.value_.from_data(base16(hexcode))) {
        BOOST_THROW_EXCEPTION(invalid_option_value(hexcode));
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, header const& argument) {
    auto const bytes = argument.value_.to_data();

    output << base16(bytes);
    return output;
}

<<<<<<< HEAD
}  // namespace config
=======
>>>>>>> dev
}  // namespace kth
