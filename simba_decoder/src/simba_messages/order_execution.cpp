#include <simba_decoder/simba_messages/order_execution.hpp>

#include <iomanip>
#include <sstream>

namespace simba_decoder {

std::unique_ptr<DecodedSimbaBody> OrderExecutionMessage::Decode(std::vector<std::byte>::const_iterator& it)
{
    auto message = std::make_unique<OrderExecutionMessage>();

    std::memcpy(&message->order_execution, &(*it), sizeof(OrderExecution));

    it += sizeof(OrderExecution);

    return message;
}

std::string OrderExecutionMessage::Dump() const
{
    std::ostringstream oss;

    oss << "{"
        << "\"message_type\":" << "\"OrderExecution\"" << ","
        << "\"md_entry_id\":" << order_execution.md_entry_id << ","
        << "\"md_entry_px\":" << ConvertDecimalToString(order_execution.md_entry_px) << ","
        << "\"md_entry_size\":" << order_execution.md_entry_size << ","
        << "\"last_px\":" << ConvertDecimalToString(order_execution.last_px) << ","
        << "\"last_qty\":" << order_execution.last_qty << ","
        << "\"trade_id\":" << order_execution.trade_id << ","
        << "\"md_flags\":" << static_cast<uint64_t>(order_execution.md_flags) << ","
        << "\"md_flags2\":" << order_execution.md_flags2 << ","
        << "\"security_id\":" << order_execution.security_id << ","
        << "\"rpt_seq\":" << order_execution.rpt_seq << ","
        << "\"md_update_action\":" << static_cast<int>(order_execution.md_update_action) << ","
        << "\"md_entry_type\":\"" << static_cast<char>(order_execution.md_entry_type) << "\""
        << "}";

    return oss.str();
}

} // namespace simba_decoder