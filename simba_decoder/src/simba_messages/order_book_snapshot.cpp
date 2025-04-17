#include <simba_decoder/simba_messages/order_book_snapshot.hpp>

#include <iomanip>
#include <sstream>

namespace simba_decoder {

std::unique_ptr<DecodedSimbaBody> OrderBookSnapshotMessage::Decode(std::vector<std::byte>::const_iterator& it)
{
    auto message = std::make_unique<OrderBookSnapshotMessage>();

    auto& snapshot = message->order_book_snapshot;

    std::memcpy(&snapshot, &(*it), order_book_size);
    it += order_book_size;

    snapshot.md_entries.resize(snapshot.no_md_entries.num_in_group);

    for (auto& md_entry : snapshot.md_entries) {
        std::memcpy(&md_entry, &(*it), sizeof(OrderBookSnapshot::Entry));
        it += sizeof(OrderBookSnapshot::Entry);
    }

    return message;
}

std::string OrderBookSnapshotMessage::Dump() const
{
    std::ostringstream oss;

    // LLM generated snippet

    oss << "{"
        << "\"message_type\":" << "\"OrderBookSnapshot\"" << ","
        << "\"security_id\":" << order_book_snapshot.security_id << ","
        << "\"last_msg_seq_num_processed\":" << order_book_snapshot.last_msg_seq_num_processed << ","
        << "\"rpt_seq\":" << order_book_snapshot.rpt_seq << ","
        << "\"exchange_trading_session_id\":" << order_book_snapshot.exchange_trading_session_id << ",";

    oss << "\"no_md_entries\":{"
        << "\"block_length\":" << order_book_snapshot.no_md_entries.block_length << ","
        << "\"num_in_group\":" << static_cast<uint32_t>(order_book_snapshot.no_md_entries.num_in_group) << "},";

    oss << "\"md_entries\":[";
    for (size_t i = 0; i < order_book_snapshot.md_entries.size(); ++i) {
        const auto& entry = order_book_snapshot.md_entries[i];
        oss << "{"
            << "\"md_entry_id\":" << entry.md_entry_id << ","
            << "\"transact_time\":" << entry.transact_time << ","
            << "\"md_entry_px\":" << ConvertDecimalToString(entry.md_entry_px) << ","
            << "\"md_entry_size\":" << entry.md_entry_size << ","
            << "\"trade_id\":" << entry.trade_id << ","
            << "\"md_flags\":" << static_cast<uint64_t>(entry.md_flags) << ","
            << "\"md_flags2\":" << entry.md_flags2 << ","
            << "\"md_entry_type\":\"" << static_cast<char>(entry.md_entry_type) << "\""
            << "}";

        if (i + 1 < order_book_snapshot.md_entries.size()) {
            oss << ",";
        }
    }

    oss << "]}";

    return oss.str();
}

} // namespace simba_decoder