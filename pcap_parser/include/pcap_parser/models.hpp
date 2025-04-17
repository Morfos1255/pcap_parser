#pragma once

#include <chrono>
#include <cstdint>
#include <vector>

#include <common/endians.hpp>

namespace pcap_parser::models {

#pragma pack(push, 1)

// Maybe we should decide at the begging, which arch do we use
// What we're going to parse and how. And so, this marvelous code will not happen.

// Or we use library that deals with endians...

// But whatever...

// clang-format off

DEFINE_SWAPPABLE_STRUCT(PcapGlobalHeader,
    FIELD(uint32_t, magic_number)
    FIELD(uint16_t, version_major)
    FIELD(uint16_t, version_minor)
    FIELD(int32_t,  thiszone)
    FIELD(uint32_t, sigfigs)
    FIELD(uint32_t, snaplen)
    FIELD(uint32_t, network)
) {
    SWAP_FIELD(uint16_t, version_major)
    SWAP_FIELD(uint16_t, version_minor)
    SWAP_FIELD(int32_t,  thiszone)
    SWAP_FIELD(uint32_t, sigfigs)
    SWAP_FIELD(uint32_t, snaplen)
    SWAP_FIELD(uint32_t, network)
}

DEFINE_SWAPPABLE_STRUCT(PcapPacketHeader,
    FIELD(uint32_t, ts_sec)
    FIELD(uint32_t, ts_usec)
    FIELD(uint32_t, incl_len)
    FIELD(uint32_t, orig_len)
) {
    SWAP_FIELD(uint32_t, ts_sec)
    SWAP_FIELD(uint32_t, ts_usec)
    SWAP_FIELD(uint32_t, incl_len)
    SWAP_FIELD(uint32_t, orig_len)
}

// clang-format on

// I decide to not process endians actually, as there are too much
// details about what's in network endian, what is in host endian, where
// always big endian, where always little and so on

// I assume there is little endian everything, and machine that run this code
// also little endian

// So, macros above just for fun and demonstration

struct EthernetHeader
{
    uint8_t destination[6];
    uint8_t source[6];
    uint16_t etherType;
};

struct IPv4Header
{
    uint8_t version_and_header_length;
    uint8_t type_of_service;
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_and_fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t header_checksum;
    uint32_t source_address;
    uint32_t destination_address;
};

struct UDPHeader
{
    uint16_t source_port;
    uint16_t destination_port;
    uint16_t length;
    uint16_t checksum;
};

struct PcapPacket
{
    PcapPacketHeader header;
    EthernetHeader ethernet_header;
    IPv4Header ip_header;
    UDPHeader udp_header;
    std::vector<std::byte> data;
};

#pragma pack(pop)

} // namespace pcap_parser::models