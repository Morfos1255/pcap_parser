#include <pcap_parser/pcap_parser.hpp>

#include <cassert>
#include <iostream>

#include "utils.hpp"

namespace pcap_parser {

PcapParser::PcapParser(const std::string& filename)
    : file_(std::ifstream(filename, std::ios::binary))
{
    if (!file_) {
        throw std::runtime_error("Cannot open file");
    }

    // Working with endians are kinda strange
    ParseMagicByteProperly();

    std::cout << std::showbase << std::hex << "Magic Number: " << global_header_.magic_number << std::dec << "\n";

    switch (global_header_.magic_number) {
    case 0xa1b2c3d4: // Big-endian, microsecond
        endian_ = Endian::Big;
        is_nanosecond_ = false;
        std::cout << "Big Endian arch with microseconds" << '\n';
        break;
    case 0xd4c3b2a1: // Little-endian, microsecond
        endian_ = Endian::Little;
        is_nanosecond_ = false;
        std::cout << "Little Endian arch with microseconds" << '\n';
        break;
    case 0xa1b23c4d: // Big-endian, nanosecond
        endian_ = Endian::Big;
        is_nanosecond_ = true;
        std::cout << "Big Endian arch with nanoseconds" << '\n';
        break;
    case 0x4d3cb2a1: // Little-endian, nanosecond
        endian_ = Endian::Little;
        is_nanosecond_ = true;
        std::cout << "Little Endian arch with nanoseconds" << '\n';
        break;
    default:
        throw std::runtime_error("Unknown PCAP magic number");
    }

    file_.read(
        reinterpret_cast<char*>(&global_header_.version_major),
        sizeof(GlobalHeader) - sizeof(global_header_.magic_number));

    // Don't use it actually
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    if (endian_ == Endian::Big) {
        std::cout << "NEED SWAP" << '\n';
        need_swap_ = true;
        throw;
    }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    if (endian == Endian::Little) {
        std::cout << "NEED SWAP" << '\n';
        need_swap_ = true;
        throw;
    }
#endif

    std::cout << std::showbase << std::hex << "Version Major: " << global_header_.version_major << "\n";
    std::cout << "Version Minor: " << global_header_.version_minor << "\n";
    std::cout << "Thiszone: " << global_header_.thiszone << "\n";
    std::cout << "Snaplen: " << global_header_.snaplen << "\n";
    std::cout << "Network: " << global_header_.network << "\n";
    std::cout << "Sigfigs: " << global_header_.sigfigs << std::dec << "\n";
}

void PcapParser::ParseMagicByteProperly()
{
    // I don't get why, but if I read it straight into 4 bytes, it's gonna looks like BE
    // But it actually LE, and to get it, I need to parse byte one by one, in the natural order
    // it appears in binary
    unsigned char first_byte;
    unsigned char second_byte;
    unsigned char third_byte;
    unsigned char fourth_byte;

    file_.read(reinterpret_cast<char*>(&first_byte), sizeof(first_byte));

    file_.read(reinterpret_cast<char*>(&second_byte), sizeof(second_byte));

    file_.read(reinterpret_cast<char*>(&third_byte), sizeof(third_byte));

    file_.read(reinterpret_cast<char*>(&fourth_byte), sizeof(fourth_byte));

    global_header_.magic_number = static_cast<uint32_t>(first_byte) << 24 | static_cast<uint32_t>(second_byte) << 16 |
                                  static_cast<uint32_t>(third_byte) << 8 | static_cast<uint32_t>(fourth_byte);
}

std::optional<PcapParser::Packet> PcapParser::Next() const
{

    if (file_.eof()) {
        std::cout << "Reach end of file" << '\n';
        return std::nullopt;
    }

    Packet packet;

    try {
        packet.header = ParseHeader<PacketHeader>().value();
        packet.ethernet_header = ParseHeader<EthernetHeader>().value();
        packet.ip_header = ParseHeader<IPv4Header>().value();
        packet.udp_header = ParseHeader<UDPHeader>().value();
    } catch (...) {
        std::cout << "Couldn't read headers, assume end of file" << '\n';
        return std::nullopt;
    }

    size_t ip_header_size = (packet.ip_header.version_and_header_length & 0x0F) * 4;
    size_t headers_length = sizeof(EthernetHeader) + ip_header_size + sizeof(UDPHeader);

    size_t payload_size = packet.header.incl_len - headers_length;
    packet.data.resize(payload_size);

    file_.read(reinterpret_cast<char*>(packet.data.data()), payload_size);

    assert(packet.data.size() == payload_size);

    return packet;
}

std::optional<models::IPv4Header> PcapParser::ParseIPv4Header() const
{
    static constexpr std::size_t kBaseIPv4HeaderSize = 20;
    IPv4Header ip_header;
    file_.read(reinterpret_cast<char*>(&ip_header), kBaseIPv4HeaderSize);

    uint8_t ihl = ip_header.version_and_header_length & 0x0F;
    std::size_t ip_header_size = ihl * 4;

    // Read any additional options in the extended header
    if (ip_header_size > kBaseIPv4HeaderSize) {
        file_.read(reinterpret_cast<char*>(&ip_header) + kBaseIPv4HeaderSize, ip_header_size - kBaseIPv4HeaderSize);
        if (file_.gcount() != ip_header_size - kBaseIPv4HeaderSize) {
            return std::nullopt;
        }
    }
    return ip_header;
}

const PcapParser::GlobalHeader& PcapParser::GetGlobalHeader() const
{
    return global_header_;
}
bool PcapParser::NeedSwap() const
{
    return need_swap_;
}

} // namespace pcap_parser