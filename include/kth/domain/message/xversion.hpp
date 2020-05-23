// Copyright (c) 2016-2020 Knuth Project developers.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef KTH_DOMAIN_MESSAGE_ANNOUNCE_XVERSION_HPP_
#define KTH_DOMAIN_MESSAGE_ANNOUNCE_XVERSION_HPP_

#include <cstdint>
#include <istream>
#include <memory>
#include <string>

#include <kth/domain/constants.hpp>
#include <kth/domain/define.hpp>
#include <kth/infrastructure/message/network_address.hpp>
#include <kth/infrastructure/utility/container_sink.hpp>
#include <kth/infrastructure/utility/container_source.hpp>
#include <kth/infrastructure/utility/reader.hpp>
#include <kth/infrastructure/utility/writer.hpp>

#include <kth/domain/common.hpp>
#include <kth/domain/concepts.hpp>

namespace kth::message {

// Implementation of BU xversion and xverack messages
// https://github.com/BitcoinUnlimited/BitcoinUnlimited/blob/dev/doc/xversionmessage.md

// The checksum is ignored by the xversion command.
class BC_API xversion {
public:
    using ptr = std::shared_ptr<xversion>;
    using const_ptr = std::shared_ptr<const xversion>;

    enum level : uint32_t {
        // compact blocks protocol FIX
        bip152_fix = 70015,  //TODO(fernando): See how to name this, because there is no BIP for that...

        // compact blocks protocol
        bip152 = 70014,

        // fee_filter
        bip133 = 70013,

        // send_headers
        bip130 = 70012,

        // node_bloom service bit
        bip111 = 70011,

        // node_utxo service bit (draft)
        bip64 = 70004,

        // reject (satoshi node writes xversion.relay starting here)
        bip61 = 70002,

        // filters, merkle_block, not_found, xversion.relay
        bip37 = 70001,

        // memory_pool
        bip35 = 60002,

        // ping.nonce, pong
        bip31 = 60001,

        // Don't request blocks from nodes of xversions 32000-32400.
        no_blocks_end = 32400,

        // Don't request blocks from nodes of xversions 32000-32400.
        no_blocks_start = 32000,

    // This preceded the BIP system.
#ifdef KTH_CURRENCY_LTC
        headers = 70002,
#else
        headers = 31800,
#endif

        // We require at least this of peers, address.time fields.
        minimum = 31402,

        // We support at most this internally (bound to settings default).
        maximum = bip152_fix,

        // Used to generate canonical size required by consensus checks.
        canonical = 0
    };

    enum service : uint64_t {
        // The node exposes no services.
        none = 0,

        // The node is capable of serving the block chain (full node).
        node_network = (1U << 0),

        // Requires xversion.value >= level::bip64 (BIP64 is draft only).
        // The node is capable of responding to the getutxo protocol request.
        node_utxo = (1U << 1),

        // Requires xversion.value >= level::bip111

        // The node is capable and willing to handle bloom-filtered connections.
        node_bloom = (1U << 2),

        // Independent of network protocol level.
        // The node is capable of responding to witness inventory requests.
        node_witness = (1U << 3),

#ifdef KTH_CURRENCY_BCH
        node_network_cash = (1 << 5)  //TODO(kth): check what happens with node_network (or node_network_cash)
#endif                                //KTH_CURRENCY_BCH
    };

    static
    xversion factory_from_data(uint32_t xversion, data_chunk const& data);
    
    static
    xversion factory_from_data(uint32_t xversion, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    static
    xversion factory_from_data(uint32_t xversion, R& source) {
        message::xversion instance;
        instance.from_data(xversion, source);
        return instance;
    }

    xversion() = default;
    xversion(uint32_t value, uint64_t services, uint64_t timestamp, network_address const& address_receiver, network_address const& address_sender, uint64_t nonce, std::string const& user_agent, uint32_t start_height, bool relay);
    xversion(uint32_t value, uint64_t services, uint64_t timestamp, network_address const& address_receiver, network_address const& address_sender, uint64_t nonce, std::string&& user_agent, uint32_t start_height, bool relay);

    bool operator==(xversion const& x) const;
    bool operator!=(xversion const& x) const;

    [[nodiscard]] 
    uint32_t value() const;

    void set_value(uint32_t value);

    [[nodiscard]] 
    uint64_t services() const;
    
    void set_services(uint64_t services);

    [[nodiscard]] 
    uint64_t timestamp() const;
    
    void set_timestamp(uint64_t timestamp);
    network_address& address_receiver();
    
    [[nodiscard]] 
    network_address const& address_receiver() const;

    void set_address_receiver(network_address const& address);
    network_address& address_sender();
    
    [[nodiscard]] 
    network_address const& address_sender() const;

    void set_address_sender(network_address const& address);

    [[nodiscard]] 
    uint64_t nonce() const;
    
    void set_nonce(uint64_t nonce);

    std::string& user_agent();
    
    [[nodiscard]] 
    std::string const& user_agent() const;
    
    void set_user_agent(std::string const& agent);
    void set_user_agent(std::string&& agent);

    [[nodiscard]] 
    uint32_t start_height() const;
    
    void set_start_height(uint32_t height);

    // xversion >= 70001
    [[nodiscard]] 
    bool relay() const;
    
    void set_relay(bool relay);

    bool from_data(uint32_t xversion, data_chunk const& data);
    bool from_data(uint32_t xversion, std::istream& stream);

    template <typename R, KTH_IS_READER(R)>
    bool from_data(uint32_t xversion, R& source) {
        reset();

        value_ = source.read_4_bytes_little_endian();
        services_ = source.read_8_bytes_little_endian();
        timestamp_ = source.read_8_bytes_little_endian();
        address_receiver_.from_data(xversion, source, false);
        address_sender_.from_data(xversion, source, false);
        nonce_ = source.read_8_bytes_little_endian();
        user_agent_ = source.read_string();
        start_height_ = source.read_4_bytes_little_endian();

        // HACK: disabled check due to inconsistent node implementation.
        // The protocol expects duplication of the sender's services.
        ////if (services_ != address_sender_.services())
        ////    source.invalidate();

        auto const peer_bip37 = (value_ >= level::bip37);
        auto const self_bip37 = (xversion >= level::bip37);

        // The relay field is optional at or above xversion 70001.
        // But the peer doesn't know our xversion when it sends its xversion.
        // This is a bug in the BIP37 design as it forces older peers to adapt to
        // the expansion of the xversion message, which is a clear compat break.
        // So relay is eabled if either peer is below 70001, it is not set, or
        // peers are at/above 70001 and the field is set.
        relay_ = (peer_bip37 != self_bip37) || source.is_exhausted() ||
                 (self_bip37 && source.read_byte() != 0);

        if ( ! source) {
            reset();
        }

        return source;
    }

    //bool from_data(uint32_t xversion, reader& source);
    [[nodiscard]] 
    data_chunk to_data(uint32_t xversion) const;
    
    void to_data(uint32_t xversion, data_sink& stream) const;

    template <typename W>
    void to_data(uint32_t xversion, W& sink) const {
        sink.write_4_bytes_little_endian(value_);
        auto const effective_xversion = std::min(xversion, value_);
        sink.write_8_bytes_little_endian(services_);
        sink.write_8_bytes_little_endian(timestamp_);
        address_receiver_.to_data(xversion, sink, false);
        address_sender_.to_data(xversion, sink, false);
        sink.write_8_bytes_little_endian(nonce_);
        sink.write_string(user_agent_);
        sink.write_4_bytes_little_endian(start_height_);

        if (effective_xversion >= level::bip37) {
            sink.write_byte(relay_ ? 1 : 0);
        }
    }

    //void to_data(uint32_t xversion, writer& sink) const;
    [[nodiscard]] 
    bool is_valid() const;
    
    void reset();
    
    [[nodiscard]] 
    size_t serialized_size(uint32_t xversion) const;


    static
    std::string const command;

    //static
    //const bounds xversion;
    
    static
    uint32_t const xversion_minimum;
    
    staticuint32_t const xversion_maximum;

private:
    uint32_t value_{0};
    uint64_t services_{0};
    uint64_t timestamp_{0};
    network_address address_receiver_;
    network_address address_sender_;
    uint64_t nonce_{0};
    std::string user_agent_;
    uint32_t start_height_{0};

    // xversion >= 70001
    bool relay_{false};
};

}  // namespace kth::message

#endif
