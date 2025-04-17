#pragma once

#include <fstream>

#include <pcap_parser/models.hpp>

namespace pcap_parser {

class PcapParser
{
public:
    using Packet = models::PcapPacket;
    using GlobalHeader = models::PcapGlobalHeader;
    using PacketHeader = models::PcapPacketHeader;
    using EthernetHeader = models::EthernetHeader;
    using IPv4Header = models::IPv4Header;
    using UDPHeader = models::UDPHeader;

    explicit PcapParser(const std::string& filename);

    std::optional<Packet> Next() const;
    [[nodiscard]] const GlobalHeader& GetGlobalHeader() const;
    bool NeedSwap() const;

protected:
    enum class Endian
    {
        Little,
        Big
    };

    void ParseMagicByteProperly();

    template <typename T>
    std::optional<T> ParseHeader() const
    {
        T header;
        file_.read(reinterpret_cast<char*>(&header), sizeof(T));
        if (file_.gcount() != sizeof(header)) {
            return std::nullopt;
        }
        return header;
    }

    std::optional<models::IPv4Header> ParseIPv4Header() const;

private:
    mutable std::ifstream file_;
    GlobalHeader global_header_{};
    Endian endian_ = Endian::Little;
    bool is_nanosecond_{};
    bool need_swap_{};
};

} // namespace pcap_parser
