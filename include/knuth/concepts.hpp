// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_CONCEPTS_HPP_
#define KTH_CONCEPTS_HPP_

<<<<<<< HEAD
#define Reader typename
#define Writer typename
=======
#define Reader typename //NOLINT
#define Writer typename //NOLINT
>>>>>>> dev

namespace knuth {

template <typename R, bool result = std::is_same<decltype(((R*)nullptr)->read_size_little_endian()), size_t>::value>  //NOLINT
constexpr bool is_reader_helper(int /*unused*/) {
    return result;
}

template <typename R>
constexpr bool is_reader_helper(...) {  //NOLINT
    return false;
}

template <typename R>
constexpr bool is_reader() {
    return is_reader_helper<R>(0);
}

template <typename W, bool result = std::is_same<decltype(((W*)nullptr)->write_size_little_endian(0u)), void>::value>  //NOLINT
constexpr bool is_writer_helper(int /*unused*/) {
    return result;
}

template <typename W>
constexpr bool is_writer_helper(...) {  //NOLINT
    return false;
}

template <typename W>
constexpr bool is_writer() {
    return is_writer_helper<W>(0);
}

}  // namespace knuth

<<<<<<< HEAD
#define KTH_IS_READER(R) typename std::enable_if<knuth::is_reader<R>(), int>::type = 0
#define KTH_IS_WRITER(W) typename std::enable_if<knuth::is_writer<W>(), int>::type = 0
=======
#define KTH_IS_READER(R) typename std::enable_if<knuth::is_reader<R>(), int>::type = 0  //NOLINT
#define KTH_IS_WRITER(W) typename std::enable_if<knuth::is_writer<W>(), int>::type = 0  //NOLINT
>>>>>>> dev

#endif  //KTH_CONCEPTS_HPP_
