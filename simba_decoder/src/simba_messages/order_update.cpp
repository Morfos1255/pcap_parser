#include <simba_decoder/simba_messages/order_update.hpp>

#include <sstream>

namespace simba_decoder {

std::unique_ptr<DecodedSimbaBody> OrderUpdateMessage::Decode(std::vector<std::byte>::const_iterator& it)
{
    auto message = std::make_unique<OrderUpdateMessage>();

    std::memcpy(&message->order_update, &(*it), sizeof(OrderUpdate));

    it += sizeof(OrderUpdate);

    return message;
}

std::string OrderUpdateMessage::Dump() const
{
    std::ostringstream oss;

    oss << "{"
        << "\"message_type\":" << "\"OrderUpdate\"" << ","
        << "\"md_entry_id\":" << order_update.md_entry_id << ","
        << "\"md_entry_px\":" << ConvertDecimalToString(order_update.md_entry_px) << ","
        << "\"md_entry_size\":" << order_update.md_entry_size << ","
        << "\"md_flags\":" << static_cast<uint64_t>(order_update.md_flags) << ","
        << "\"md_flags2\":" << order_update.md_flags2 << ","
        << "\"security_id\":" << order_update.security_id << ","
        << "\"rpt_seq\":" << order_update.rpt_seq << ","
        << "\"md_update_action\":" << static_cast<int>(order_update.md_update_action) << ","
        << "\"md_entry_type\":\"" << static_cast<char>(order_update.md_entry_type) << "\""
        << "}";

    return oss.str();
}

} // namespace simba_decoder