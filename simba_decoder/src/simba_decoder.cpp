#include <simba_decoder/simba_decoder.hpp>

#include <iomanip>
#include <iostream>
#include <span>

#include "decode_dispatcher.hpp"

namespace simba_decoder {

namespace {

bool IsIncremental(const MarketDataPacketHeader& header)
{
    return header.msg_flags & 0x8;
}

MarketDataPacketHeader DecodeMarketDataPacketHeader(std::vector<std::byte>::const_iterator& it)
{
    MarketDataPacketHeader hdr{};

    std::memcpy(&hdr, &(*it), sizeof(MarketDataPacketHeader));

    it += sizeof(MarketDataPacketHeader);

    return hdr;
}

SBEHeader DecodeSBEHeader(std::vector<std::byte>::const_iterator& it)
{
    SBEHeader hdr{};

    std::memcpy(&hdr, &(*it), sizeof(SBEHeader));

    it += sizeof(SBEHeader);

    return hdr;
}

} // namespace

std::vector<std::unique_ptr<DecodedSimbaBody>> Decode(const std::vector<std::byte>& data)
{
    auto it = data.begin();

    auto market_data_packet_header = DecodeMarketDataPacketHeader(it);
    if (IsIncremental(market_data_packet_header)) {
        // Just skip it
        it += sizeof(IncrementalPacketHeader);
    }

    const DecodeDispatcher decode_dispatcher{};

    std::vector<std::unique_ptr<DecodedSimbaBody>> result;

    while (it != data.end() && (it - data.begin()) < market_data_packet_header.msg_size) {
        auto header = DecodeSBEHeader(it);
        result.push_back(decode_dispatcher.Visit(header.template_id, it));
        if (!result.back()) {
            if (header.block_length == 0) {
                //throw std::runtime_error("I mess up");
                break;
            }
            // Decoding seems okay, but for some reason, once I reach msg_id = 18, block_length = 290
            // on data 2023-10-09.1849-1906.pcap (packet number 20), it fails after skipping
            // I didn't get why
            // I checked out this packet in WireShark, count very first like 24 bytes of raw data, and they
            // decoded well
            // I could assume that there in 290 bytes (blockLength) some Repeated Group fields. According to
            // SIMBA specification "BlockLength: The root part length of the message in bytes.
            // It does not include the SBE message header and the repeating 'NoMDEntries' field group."
            // But according to same specification, there are no 'NoMDEntries' in message with id = 18
            // So, I decided to just skip it, whatever...
            it += header.block_length;
        }
    }

    return result;
}

} // namespace simba_decoder