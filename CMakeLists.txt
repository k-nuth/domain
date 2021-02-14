# Copyright (c) 2016-2021 Knuth Project developers.
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.


cmake_minimum_required(VERSION 3.8)

# kth-domain
#==============================================================================
project(kth-domain
  VERSION 0
  LANGUAGES CXX C)
  # VERSION 0.11.0


#if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
#  # using Clang
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
#  # using GCC
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
#  # using Intel C++
#elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#  # using Visual Studio C++
#endif()
#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#  # using regular Clang or AppleClang
#endif()

# message(CMAKE_CXX_COMPILER_ID)
# message(${CMAKE_CXX_COMPILER_ID})


set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Check for baseline language coverage in the compiler for the C++17 standard.
#------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Process options.
#==============================================================================

# Implement --use-conan
#------------------------------------------------------------------------------
option(USE_CONAN "Use Conan Build Tool." OFF)
option(NO_CONAN_AT_ALL "Conan totally disabled." OFF)

if (NO_CONAN_AT_ALL)
  set(USE_CONAN OFF)
endif()


# Inherit --enable-shared and define BOOST_TEST_DYN_LINK.
#------------------------------------------------------------------------------
option(ENABLE_SHARED "" OFF)

# Implement POSITION_INDEPENDENT_CODE
#------------------------------------------------------------------------------
option(ENABLE_POSITION_INDEPENDENT_CODE "Enable POSITION_INDEPENDENT_CODE property" ON)

# Implement --with-tests and declare WITH_TESTS.
#------------------------------------------------------------------------------
option(WITH_TESTS "Compile with unit tests." ON)

# Implement --with-examples and declare WITH_EXAMPLES.
#------------------------------------------------------------------------------
option(WITH_EXAMPLES "Compile with examples." OFF)


# Implement --with-cached-rpc-data and declare WITH_CACHED_RPC_DATA.
#------------------------------------------------------------------------------
option(WITH_CACHED_RPC_DATA "Cached RPC data enabled." OFF)
if (WITH_CACHED_RPC_DATA)
  message(STATUS "Knuth: Cached RPC data enabled")
  add_definitions(-DKTH_CACHED_RPC_DATA)
endif()

# Implement --with-keoken and declare WITH_KEOKEN.
#------------------------------------------------------------------------------
option(WITH_KEOKEN "Keoken enabled." OFF)
if (WITH_KEOKEN)
  message(STATUS "Knuth: Keoken enabled")
  add_definitions(-DWITH_KEOKEN)
endif()

# Implement --with-icu and define BOOST_HAS_ICU and output ${icu}.
#------------------------------------------------------------------------------
option(WITH_ICU "Compile with International Components for Unicode." OFF)
if (WITH_ICU)
  add_definitions(-DWITH_ICU -DBOOST_HAS_ICU)
  if (NO_CONAN_AT_ALL)
    message(FATAL_ERROR "WITH_ICU not yet implemented")
  endif (NO_CONAN_AT_ALL)
endif()

# Implement --with-png and output ${png}.
#------------------------------------------------------------------------------
option(WITH_PNG "Compile with Libpng support." OFF)
if (WITH_PNG)
  add_definitions(-DWITH_PNG)
endif()

# Implement --with-qrencode and output ${qrencode}.
#------------------------------------------------------------------------------
option(WITH_QRENCODE "Compile with QREncode." OFF)
if (WITH_QRENCODE)
  add_definitions(-DWITH_QRENCODE)

  if (NO_CONAN_AT_ALL)
    message(FATAL_ERROR "WITH_QRENCODE not yet implemented")
  endif (NO_CONAN_AT_ALL)
endif()

# Implement --JUST_KTH_SOURCES for linting.
#------------------------------------------------------------------------------
option(JUST_KTH_SOURCES "Just Knuth source code to be linted." OFF)
if (JUST_KTH_SOURCES)
  message(STATUS "Knuth: Just Knuth source code to be linted: enabled")
endif()

## Implement --DISABLE_GET_BLOCKS.
##------------------------------------------------------------------------------
#option(DISABLE_GET_BLOCKS "Option to disable the getblocks message." OFF)
#if (DISABLE_GET_BLOCKS)
#  message(STATUS "Knuth: getblocks message is disabled")
#  add_definitions(-DKTH_PROTOCOL_DISABLE_GET_BLOCKS)
#else()
#  message(STATUS "Knuth: getblocks message is enabled")
#endif()


# # Implement --binlog.
# #------------------------------------------------------------------------------
# option(BINLOG "Uses BinLog logging library." OFF)
# if (BINLOG)
#   add_definitions(-DKTH_USE_BINLOG)
# endif()

set(LOG_LIBRARY "boost" CACHE STRING "Setting for the logging library (boost|spdlog|binlog).")

if (${LOG_LIBRARY} STREQUAL "boost")
  add_definitions(-DKTH_LOG_LIBRARY_BOOST)
elseif (${LOG_LIBRARY} STREQUAL "spdlog")
  add_definitions(-DKTH_LOG_LIBRARY_SPDLOG)
elseif (${LOG_LIBRARY} STREQUAL "binlog")
  add_definitions(-DKTH_LOG_LIBRARY_BINLOG)
else()
  message(FATAL_ERROR "Invalid Logging Library: ${LOG_LIBRARY}")
endif()

message(STATUS "Knuth: Logging Library: ${LOG_LIBRARY}")

set(KTH_PROJECT_VERSION "-" CACHE STRING "Specify the Knuth Project Version.")
# message(${KTH_PROJECT_VERSION})
message( STATUS "Knuth: Compiling version ${KTH_PROJECT_VERSION}")

set(MICROARCHITECTURE "x86_64" CACHE STRING "Specify the Microarchitecture (x86_64|...).")
message( STATUS "Knuth: Compiling for ${MICROARCHITECTURE}")

set(MARCH_ID "" CACHE STRING "Specify the Microarchitecture ID (x86_64|...).")
message( STATUS "Knuth: Compiling for Microarchitecture ID ${MARCH_ID}")

message( STATUS "Knuth: CONAN_CXX_FLAGS ${CONAN_CXX_FLAGS}")
message( STATUS "Knuth: CONAN_C_FLAGS ${CONAN_C_FLAGS}")


set(CURRENCY "BCH" CACHE STRING "Specify the Cryptocurrency (BCH|BTC|LTC).")

if (${CURRENCY} STREQUAL "BCH")
  add_definitions(-DKTH_CURRENCY_BCH)
elseif (${CURRENCY} STREQUAL "BTC")
  add_definitions(-DKTH_CURRENCY_BTC)
elseif (${CURRENCY} STREQUAL "LTC")
  add_definitions(-DKTH_CURRENCY_LTC)
else()
  message(FATAL_ERROR "Invalid Cryptocurrency: ${CURRENCY}")
endif()

message(STATUS "Knuth: Cryptocurrency: ${CURRENCY}")


# Conan infrastructure
#==============================================================================
if(EXISTS ${CMAKE_SOURCE_DIR}/ci_utils/cmake/knuthbuildinfo.cmake)
    include(${CMAKE_SOURCE_DIR}/ci_utils/cmake/knuthbuildinfo.cmake)
else()
    message( STATUS "knuthbuildinfo doent exists")
endif()




# get_directory_property( DirDefs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
# foreach( d ${DirDefs} )
#     message( STATUS "Found Define: " ${d} )
# endforeach()
# message( STATUS "DirDefs: " ${DirDefs} )

# remove_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
# remove_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)

# get_directory_property( DirDefs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS )
# foreach( d ${DirDefs} )
#     message( STATUS "Found Define: " ${d} )
# endforeach()
# message( STATUS "DirDefs: " ${DirDefs} )


set(Boost_DEBUG ON)

# Check dependencies.
#==============================================================================
#------------------------------------------------------------------------------
if (NO_CONAN_AT_ALL)
  # Require Boost of at least version 1.56.0 and output ${boost_CPPFLAGS/LDFLAGS}.
  #------------------------------------------------------------------------------
  if (NOT ENABLE_SHARED)
    set(Boost_USE_STATIC_LIBS ON)
  endif()

  if (MSVC)
    find_package(
      Boost 1.66.0 REQUIRED
      COMPONENTS atomic chrono date_time filesystem iostreams locale log log_setup program_options system thread unit_test_framework
      # regex
    )
  else()
    find_package(
      Boost 1.66.0 REQUIRED
      COMPONENTS chrono date_time filesystem iostreams locale log program_options system thread unit_test_framework
      # regex
    )
  endif()
endif()



# Require pthread and output ${pthread_CPPFLAGS/LIBS}.
#------------------------------------------------------------------------------
find_package(Threads REQUIRED)  #TODO(fernando): manage with conan

# Require png of at least version 1.6.25 and output ${png_CPPFLAGS/LIBS/PKG}.
#------------------------------------------------------------------------------
if (WITH_PNG)
    if (NO_CONAN_AT_ALL)
        find_package(PNG 1.6.25 REQUIRED)

        add_library(png INTERFACE IMPORTED)
        set_target_properties(png PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${PNG_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${PNG_LIBRARIES}"
            INTERFACE_COMPILE_DEFINITIONS "${PNG_DEFINITIONS}")
    endif(NO_CONAN_AT_ALL)
endif()


# Require OpenSSL
#------------------------------------------------------------------------------
# if (WITH_LITECOIN)
if (${CURRENCY} STREQUAL "LTC")
  find_package(OpenSSL 1.0.1 REQUIRED)  #TODO(fernando): manage with conan
endif()

# Require rt if on linux and output ${rt_LIBS}.
#------------------------------------------------------------------------------

# Require icu-i18n of at least version 51.2 and output ${icu_i18n_CPPFLAGS/LIBS/PKG}.
#------------------------------------------------------------------------------

# Require qrencode of at least version 3.4.4 and output ${qrencode_CPPFLAGS/LIBS/PKG}.



if (NOT USE_CONAN)
# Require secp256k1 of at least version 0 and output ${secp256k1_CPPFLAGS/LIBS/PKG}.
# Require kth-infrastructure of at least version 0 and output ${kth_infrastructure_CPPFLAGS/LIBS/PKG}.
#------------------------------------------------------------------------------
  if (NOT TARGET secp256k1)
    find_package(secp256k1 0 REQUIRED) 
  endif()

  if (NOT TARGET kth-infrastructure)
    find_package(kth-infrastructure 0 REQUIRED) 
  endif()
endif()


# Set flags.
#==============================================================================
include(CheckCXXCompilerFlag)

# Including common functions
#------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/ci_utils/cmake)

include(KnuthTools)


# Warn on all stuff.
#------------------------------------------------------------------------------
if (NOT MSVC)
  _add_c_compile_flag(-Wall _has_all_warning_flag)
else()
  _add_c_compile_flag(-W4 _has_all_warning_flag)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Warn on extra stuff.
#------------------------------------------------------------------------------
if (NOT MSVC)
  _add_c_compile_flag(-Wextra _has_extra_warning_flag)
endif()

# Be really annoying.
#------------------------------------------------------------------------------
_add_c_compile_flag(-Wpedantic _has_pedantic_warning_flag)
if (_has_pedantic_warning_flag)
  _add_c_compile_flag(-pedantic _has_pedantic_flag)
endif()

if(ENABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
endif()

if(ENABLE_USAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

if(ENABLE_TSAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif()

# Conform to style.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-Wno-missing-braces _has_no_missing_braces_warning_flag)

# Conflict in stdlib under clang. Enabled in clang only.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-Wno-mismatched-tags _has_no_mismatched_tags_warning_flag)

# Clean up boost 1.55 headers. Enabled in gcc only.
#------------------------------------------------------------------------------
_add_c_compile_flag(-Wno-deprecated-declarations _has_no_deprecated_declarations_warning_flag)

# Protect stack.
#------------------------------------------------------------------------------
_add_link_flag(-fstack-protector _has_stack_protector_flag)

# Protect stack comprehensively.
#------------------------------------------------------------------------------
_add_link_flag(-fstack-protector-all _has_stack_protector_all_flag)

# Hide internal functions from external libs. Enabled in gcc only.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-fvisibility-hidden _has_visibility_hidden_flag)

# Hide inlines from external libs. Enabled in gcc only.
#------------------------------------------------------------------------------
_add_cxx_compile_flag(-fvisibility-inlines-hidden _has_visibility_inlines_hidden_flag)

# Target Windows Vista. Enabled in msvc only.
#------------------------------------------------------------------------------
if (MSVC) # OR MINGW)
  add_definitions(-D_WIN32_WINNT=0x0600)
endif()

# Build
#==============================================================================

# src/kth-domain.la => ${libdir}
#------------------------------------------------------------------------------
set(MODE STATIC)
if (ENABLE_SHARED)
  set(MODE SHARED)
endif()

set(kth_domain_sources_just_legacy
        src/chain/block_basis.cpp
        src/chain/block.cpp
        src/chain/chain_state.cpp
        src/chain/compact.cpp
        src/chain/header_basis.cpp
        src/chain/header.cpp
        src/chain/input_basis.cpp
        src/chain/input.cpp
        src/chain/output_basis.cpp
        src/chain/output.cpp
        src/chain/output_point.cpp
        src/chain/point.cpp
        src/chain/point_iterator.cpp
        src/chain/point_value.cpp
        src/chain/points_value.cpp
        src/chain/script_basis.cpp
        src/chain/script.cpp
        src/chain/transaction_basis.cpp
        src/chain/transaction.cpp

        src/machine/interpreter.cpp

        src/machine/opcode.cpp
        src/machine/operation.cpp
        src/machine/program.cpp

        src/config/header.cpp
        src/config/input.cpp
        src/config/output.cpp
        src/config/point.cpp
        src/config/transaction.cpp
        src/config/script.cpp
        src/config/ec_private.cpp
        src/config/endorsement.cpp
        
        src/utility/property_tree.cpp

        src/multi_crypto_support.cpp
        src/version.cpp

        src/math/stealth.cpp
        src/math/external/scrypt.h

        src/message/address.cpp
        src/message/alert.cpp
        src/message/alert_payload.cpp
        src/message/block.cpp
        src/message/block_transactions.cpp
        src/message/compact_block.cpp
        src/message/fee_filter.cpp
        src/message/filter_add.cpp
        src/message/filter_clear.cpp
        src/message/filter_load.cpp
        src/message/get_address.cpp
        src/message/get_block_transactions.cpp

        src/message/get_data.cpp
        src/message/get_headers.cpp
        src/message/header.cpp
        src/message/headers.cpp
        src/message/heading.cpp
        src/message/inventory.cpp
        src/message/inventory_vector.cpp
        src/message/memory_pool.cpp
        src/message/merkle_block.cpp
        src/message/not_found.cpp
        src/message/ping.cpp
        src/message/pong.cpp
        src/message/prefilled_transaction.cpp
        src/message/reject.cpp
        src/message/send_compact.cpp
        src/message/send_headers.cpp
        src/message/transaction.cpp
        src/message/verack.cpp
        src/message/version.cpp
        src/message/xverack.cpp
        src/message/xversion.cpp
        
        src/wallet/bitcoin_uri.cpp
        src/wallet/ec_private.cpp
        src/wallet/ec_public.cpp
        src/wallet/ek_private.cpp
        src/wallet/ek_public.cpp
        src/wallet/ek_token.cpp
        src/wallet/encrypted_keys.cpp
        src/wallet/message.cpp
        src/wallet/parse_encrypted_keys/parse_encrypted_key.hpp
        src/wallet/parse_encrypted_keys/parse_encrypted_key.ipp
        src/wallet/parse_encrypted_keys/parse_encrypted_prefix.hpp
        src/wallet/parse_encrypted_keys/parse_encrypted_prefix.ipp
        src/wallet/parse_encrypted_keys/parse_encrypted_private.cpp
        src/wallet/parse_encrypted_keys/parse_encrypted_private.hpp
        src/wallet/parse_encrypted_keys/parse_encrypted_public.cpp
        src/wallet/parse_encrypted_keys/parse_encrypted_public.hpp
        src/wallet/parse_encrypted_keys/parse_encrypted_token.cpp
        src/wallet/parse_encrypted_keys/parse_encrypted_token.hpp
        src/wallet/payment_address.cpp
        src/wallet/select_outputs.cpp
        src/wallet/select_outputs.cpp
        src/wallet/stealth_address.cpp
        src/wallet/stealth_receiver.cpp
        src/wallet/stealth_sender.cpp
)

# if (NOT DISABLE_GET_BLOCKS)
  set(kth_domain_sources_just_legacy
    ${kth_domain_sources_just_legacy}
    src/message/get_blocks.cpp
  )
# endif()

set(kth_domain_sources_just_kth
    src/wallet/transaction_functions.cpp
)

if (NOT ${CURRENCY} STREQUAL "BCH")
    set(kth_domain_sources_just_legacy
      ${kth_domain_sources_just_legacy}
      src/chain/witness.cpp
    )
endif()

if (${CURRENCY} STREQUAL "LTC")
    set(kth_domain_sources_just_kth 
      ${kth_domain_sources_just_kth}         
      src/math/hash.cpp
      src/math/external/scrypt.cpp
      src/math/external/scrypt-sse2.cpp)
endif()

if (WITH_KEOKEN)
  set(kth_domain_sources_just_kth 
    ${kth_domain_sources_just_kth}
    src/kth/domain/keoken/transaction_extractor.cpp
    src/kth/domain/keoken/entities/asset.cpp
    src/kth/domain/keoken/message/base.cpp
    src/kth/domain/keoken/message/create_asset.cpp
    src/kth/domain/keoken/message/send_tokens.cpp
    src/kth/domain/keoken/wallet/create_transanction.cpp
  )
endif()

if (NOT JUST_KTH_SOURCES)
  set(kth_domain_sources 
    ${kth_domain_sources_just_legacy}
  )
endif()

set(kth_domain_sources 
  ${kth_domain_sources}   
  ${kth_domain_sources_just_kth}         
)

add_library(kth-domain ${MODE} ${kth_domain_sources})

if (ENABLE_POSITION_INDEPENDENT_CODE)
    set_property(TARGET kth-domain PROPERTY POSITION_INDEPENDENT_CODE ON)
endif(ENABLE_POSITION_INDEPENDENT_CODE)



#set_target_properties(kth-domain PROPERTIES CXX_VISIBILITY_PRESET hidden)
#set_target_properties(kth-domain PROPERTIES CXX_VISIBILITY_PRESET internal)

#-fvisibility=default|internal|hidden|protected

# add_library(kth-domain ${MODE}
#        src/wallet/uri.cpp)

target_include_directories(kth-domain PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
  
if (NOT ENABLE_SHARED)
    target_compile_definitions(kth-domain PUBLIC -DKD_STATIC) #TODO(fernando): manage with Conan????
endif()

target_compile_definitions(kth-domain PUBLIC -DKTH_PROJECT_VERSION="${KTH_PROJECT_VERSION}") #TODO(fernando): manage with Conan????

target_include_directories(kth-domain SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

# if (WITH_LITECOIN)
if (${CURRENCY} STREQUAL "LTC")
  target_include_directories(kth-domain SYSTEM PUBLIC ${OPENSSL_INCLUDE_DIR})
endif()



if (NOT NO_CONAN_AT_ALL)
  target_link_libraries(kth-domain PUBLIC ${CONAN_LIBS})
else()
  if (MSVC)
    target_link_libraries(kth-domain PUBLIC
      ${Boost_ATOMIC_LIBRARY}
      ${Boost_LOG_SETUP_LIBRARY}
    )
  endif()

  target_link_libraries(kth-domain PUBLIC
    ${Boost_CHRONO_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_IOSTREAMS_LIBRARY}
    ${Boost_LOCALE_LIBRARY}
    ${Boost_LOG_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    # ${Boost_REGEX_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
  )
endif()

target_link_libraries(kth-domain PUBLIC ${CMAKE_THREAD_LIBS_INIT})  #TODO(fernando): manage with Conan????

# if (NO_CONAN_AT_ALL)
    if (MINGW) #(WIN32)
        target_link_libraries(kth-domain PUBLIC ws2_32 wsock32)
    endif()
# endif()

if (WITH_PNG)
    if (NO_CONAN_AT_ALL)
        target_link_libraries(kth-domain PUBLIC png)
    endif (NO_CONAN_AT_ALL)
endif()

if (${CURRENCY} STREQUAL "LTC")
  target_link_libraries(kth-domain PUBLIC ${OPENSSL_CRYPTO_LIBRARY})  #TODO(fernando): manage with Conan

  # OPENSSL_CRYPTO_LIBRARY
  # OPENSSL_SSL_LIBRARY
  # OPENSSL_LIBRARIES
endif()

if (NOT USE_CONAN)
  target_link_libraries(kth-domain PUBLIC secp256k1 kth-infrastructure)
endif()

_group_sources(kth-domain "${CMAKE_CURRENT_LIST_DIR}")

# Tests
#==============================================================================

if (WITH_TESTS)
  enable_testing()
endif()


# local: test/kth_domain_test
#------------------------------------------------------------------------------
if (WITH_TESTS)

  add_executable(kth_domain_test
        test/chain/block.cpp
        test/chain/compact.cpp
        test/chain/header.cpp
        test/chain/input.cpp
        test/chain/output.cpp
        test/chain/output_point.cpp
        test/chain/point.cpp
        test/chain/point_iterator.cpp
        test/chain/point_value.cpp
        test/chain/points_value.cpp
        test/chain/satoshi_words.cpp
        test/chain/script.cpp
        test/chain/transaction.cpp

        test/main.cpp
        
        test/machine/opcode.cpp
        test/machine/operation.cpp

        test/math/limits.cpp
        test/math/stealth.cpp

        test/message/address.cpp
        test/message/alert.cpp
        test/message/alert_payload.cpp
        test/message/block.cpp
        test/message/block_transactions.cpp
        test/message/compact_block.cpp
        test/message/fee_filter.cpp
        test/message/filter_add.cpp
        test/message/filter_clear.cpp
        test/message/filter_load.cpp
        test/message/get_address.cpp
        test/message/get_block_transactions.cpp
        test/message/get_blocks.cpp
        test/message/get_data.cpp
        test/message/get_headers.cpp
        test/message/header.cpp
        test/message/headers.cpp
        test/message/heading.cpp
        test/message/inventory.cpp
        test/message/inventory_vector.cpp
        test/message/memory_pool.cpp
        test/message/merkle_block.cpp
        test/message/messages.cpp
        test/message/not_found.cpp
        test/message/ping.cpp
        test/message/pong.cpp
        test/message/prefilled_transaction.cpp
        test/message/reject.cpp
        test/message/send_compact.cpp
        test/message/send_headers.cpp
        test/message/transaction.cpp
        test/message/verack.cpp
        test/message/version.cpp

        test/wallet/bitcoin_uri.cpp
        test/wallet/ec_private.cpp
        test/wallet/ec_public.cpp
        test/wallet/encrypted_keys.cpp
        test/wallet/message.cpp
        test/wallet/payment_address.cpp
        test/wallet/select_outputs.cpp
        test/wallet/stealth_address.cpp
        test/wallet/stealth_receiver.cpp
        test/wallet/stealth_sender.cpp
        test/wallet/uri_reader.cpp

        #TODO(fernando): to include 
        # test/wallet/transaction_functions.cpp
    )

    target_include_directories(kth_domain_test PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/test>)

    target_link_libraries(kth_domain_test PUBLIC kth-domain)

  if (WITH_TESTS)
    include(CTest)
    include(Catch)
    catch_discover_tests(kth_domain_test)
  endif()
  

  # _group_sources(kth_domain_test "${CMAKE_CURRENT_LIST_DIR}/test")

  # # TODO(fernando): check if there are new tests in Makefile (no Cmake)
  # _add_tests(kth_domain_test
  #   address_tests
  #   alert_payload_tests
  #   alert_tests
  #   # binary_tests
  #   bitcoin_uri_tests
  #   chain_block_tests
  #   message_block_tests
  #   block_transactions_tests
  #   # collection_tests
  #   compact_block_tests
  #   # data_tests
  #   ec_private_tests
  #   # ec_public_tests # no test cases
  #   encrypted_tests
  #   # endian_tests
  #   fee_filter_tests
  #   filter_add_tests
  #   filter_clear_tests
  #   filter_load_tests
  #   get_address_tests
  #   get_block_transactions_tests
  #   get_blocks_tests
  #   get_data_tests
  #   get_headers_tests
  #   # hash_number_tests
  #   chain_header_tests
  #   headers_tests
  #   heading_tests
  #   input_tests
  #   inventory_tests
  #   inventory_vector_tests
  #   memory_pool_tests
  #   merkle_block_tests
  #   message_tests
  #   # network_address_tests
  #   not_found_tests
  #   output_tests
  #   payment_address_tests
  #   ping_tests
  #   point_tests
  #   pong_tests
  #   prefilled_transaction_tests
  #   # pseudo_random_tests
  #   reject_tests
  #   # script_number_tests
  #   script_tests
  #   # send_compact_blocks_tests
  #   send_headers_tests
  #   # serializer_tests
  #   stealth_address_tests
  #   stealth_tests
  #   # stream_tests
  #   # thread_tests
  #   chain_transaction_tests
  #   message_transaction_tests
  #   # unicode_istream_tests
  #   # unicode_ostream_tests
  #   # unicode_tests
  #   uri_reader_tests
  #   verack_tests
  #   version_tests
  #   transaction_functions_tests
  # )

  # if (WITH_PNG)
  #   _add_tests(kth_domain_test png_tests)
  # endif()

  # if (WITH_QRENCODE)
  #   _add_tests(kth_domain_test qrcode_tests)
  # endif()
endif()

if (WITH_TESTS_NEW)
  if (WITH_KEOKEN)
    add_executable(kth_domain_test_new
            test_new/main.cpp
            test_new/create_transaction.cpp
    )
    target_link_libraries(kth_domain_test_new PUBLIC kth-domain)

    add_test(NAME kth_domain_test_new COMMAND kth_domain_test_new)
  endif()
endif()


# Examples
#==============================================================================
# local: examples/kth_domain_examples
#------------------------------------------------------------------------------
if (WITH_EXAMPLES)
  add_executable(kth_domain_examples
    examples/main.cpp)

  target_link_libraries(kth_domain_examples PUBLIC kth-domain)

  _group_sources(kth_domain_examples "${CMAKE_CURRENT_LIST_DIR}/examples")
endif()

# Install
#==============================================================================
install(TARGETS kth-domain EXPORT kth-domain
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

#install(EXPORT kth-domainTargets
#
#)

set(_kth_domain_headers
    kth/domain/multi_crypto_support.hpp
    kth/domain/compat.h
    kth/domain/compat.hpp
    kth/domain/constants.hpp
    kth/domain/define.hpp

    kth/domain/handlers.hpp
    kth/domain/version.hpp

    kth/domain/chain/block.hpp
    kth/domain/chain/chain_state.hpp
    kth/domain/chain/compact.hpp    
    kth/domain/chain/header.hpp
    kth/domain/chain/history.hpp
    kth/domain/chain/input.hpp
    kth/domain/chain/input_point.hpp
    kth/domain/chain/output.hpp
    kth/domain/chain/output_point.hpp
    kth/domain/chain/point.hpp
    kth/domain/chain/point_iterator.hpp
    kth/domain/chain/point_value.hpp
    kth/domain/chain/points_value.hpp
    kth/domain/chain/script.hpp
    kth/domain/chain/stealth.hpp
    kth/domain/chain/transaction.hpp
    kth/domain/chain/witness.hpp

    kth/domain/machine/interpreter.hpp
    kth/domain/machine/number.hpp    
    kth/domain/machine/opcode.hpp
    kth/domain/machine/operation.hpp
    kth/domain/machine/program.hpp
    kth/domain/machine/rule_fork.hpp
    
    kth/domain/config/parser.hpp
    
    kth/domain/utility/property_tree.hpp
    kth/domain/impl/utility/property_tree.ipp

    kth/domain/impl/machine/interpreter.ipp
    kth/domain/impl/machine/number.ipp
    kth/domain/impl/machine/operation.ipp
    kth/domain/impl/machine/program.ipp

    kth/domain/math/limits.hpp
    kth/domain/math/stealth.hpp
    
    kth/domain/message/address.hpp
    kth/domain/message/alert.hpp
    kth/domain/message/alert_payload.hpp
    kth/domain/message/block.hpp
    kth/domain/message/block_transactions.hpp
    kth/domain/message/compact_block.hpp
    kth/domain/message/fee_filter.hpp
    kth/domain/message/filter_add.hpp
    kth/domain/message/filter_clear.hpp
    kth/domain/message/filter_load.hpp
    kth/domain/message/get_address.hpp
    kth/domain/message/get_block_transactions.hpp
    kth/domain/message/get_blocks.hpp
    kth/domain/message/get_data.hpp
    kth/domain/message/get_headers.hpp
    kth/domain/message/header.hpp
    kth/domain/message/headers.hpp
    kth/domain/message/heading.hpp
    kth/domain/message/inventory.hpp
    kth/domain/message/inventory_vector.hpp
    kth/domain/message/memory_pool.hpp
    kth/domain/message/merkle_block.hpp
    kth/domain/message/messages.hpp

    kth/domain/message/not_found.hpp
    kth/domain/message/ping.hpp
    kth/domain/message/pong.hpp
    kth/domain/message/prefilled_transaction.hpp
    kth/domain/message/reject.hpp
    kth/domain/message/send_compact.hpp
    kth/domain/message/send_headers.hpp
    kth/domain/message/transaction.hpp
    kth/domain/message/verack.hpp
    kth/domain/message/version.hpp
    kth/domain/message/xverack.hpp
    kth/domain/message/xversion.hpp

    kth/domain/wallet/bitcoin_uri.hpp

    kth/domain/wallet/ec_private.hpp
    kth/domain/wallet/ec_public.hpp
    kth/domain/wallet/ek_private.hpp
    kth/domain/wallet/ek_public.hpp
    kth/domain/wallet/ek_token.hpp
    kth/domain/wallet/encrypted_keys.hpp
    kth/domain/wallet/hd_private.hpp
    kth/domain/wallet/hd_public.hpp
    kth/domain/wallet/message.hpp
    kth/domain/wallet/mini_keys.hpp
    kth/domain/wallet/mnemonic.hpp
    kth/domain/wallet/payment_address.hpp
    kth/domain/wallet/qrcode.hpp
    kth/domain/wallet/select_outputs.hpp
    kth/domain/wallet/stealth_address.hpp
    kth/domain/wallet/stealth_receiver.hpp
    kth/domain/wallet/stealth_sender.hpp

    kth/domain/wallet/uri_reader.hpp
    kth/domain/wallet/transaction_functions.hpp
    kth/domain.hpp
  )

foreach (_header ${_kth_domain_headers})
  get_filename_component(_directory "${_header}" DIRECTORY)
  install(FILES "include/${_header}" DESTINATION "include/${_directory}")
endforeach()

# Export
#==============================================================================
include(CMakePackageConfigHelpers)

export(EXPORT kth-domain
  FILE "${CMAKE_CURRENT_BINARY_DIR}/kth-domainTargets.cmake")
export(PACKAGE kth-domain)

if (UNIX)
  set(_config_package_location "lib/cmake")
elseif (WIN32)
  set(_config_package_location "cmake")
endif()

install(EXPORT kth-domain
  FILE kth-domainTargets.cmake
  DESTINATION ${_config_package_location})

configure_file(kth-domainConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/kth-domainConfig.cmake"
  @ONLY)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/kth-domainConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/kth-domainConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/kth-domainConfigVersion.cmake"
  DESTINATION ${_config_package_location})
