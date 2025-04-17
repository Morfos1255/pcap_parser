#pragma once

#include <simba_decoder/simba_message.hpp>

#include <vector>

namespace simba_decoder {

template <typename T>
struct decoder_traits;

class OrderBookSnapshotMessage final: public DecodedSimbaBody
{
public:
    struct __attribute__((packed)) OrderBookSnapshot
    {
        int32_t security_id;
        uint32_t last_msg_seq_num_processed;
        uint32_t rpt_seq;
        uint32_t exchange_trading_session_id;
        RepGroupDim no_md_entries;

        struct __attribute__((packed)) Entry
        {
            int64_t md_entry_id;
            uint64_t transact_time;
            Decimal5NULL md_entry_px;
            int64_t md_entry_size;
            int64_t trade_id;
            MDFlagsSet md_flags;
            uint64_t md_flags2;
            MDEntryType md_entry_type;
        };

        std::vector<Entry> md_entries;
    } order_book_snapshot;

    static std::unique_ptr<DecodedSimbaBody> Decode(std::vector<std::byte>::const_iterator& data);

    [[nodiscard]] std::string Dump() const override;

private:
    static constexpr uint16_t message_id = 17;
    static constexpr size_t order_book_size =
        sizeof(order_book_snapshot.security_id) + sizeof(order_book_snapshot.last_msg_seq_num_processed) +
        sizeof(order_book_snapshot.rpt_seq) + sizeof(order_book_snapshot.exchange_trading_session_id) +
        sizeof(RepGroupDim);
    friend struct decoder_traits<OrderBookSnapshotMessage>;
};

} // namespace simba_decoder