#pragma once

#include <simba_decoder/simba_message.hpp>

#include <vector>

namespace simba_decoder {

template <typename T>
struct decoder_traits;

class OrderExecutionMessage final: public DecodedSimbaBody
{
public:
    struct __attribute__((packed)) OrderExecution
    {
        int64_t md_entry_id;
        Decimal5NULL md_entry_px;
        int64_t md_entry_size;
        Decimal5 last_px;
        int64_t last_qty;
        int64_t trade_id;
        MDFlagsSet md_flags;
        uint64_t md_flags2;
        int32_t security_id;
        uint32_t rpt_seq;
        MDUpdateAction md_update_action;
        MDEntryType md_entry_type;
    } order_execution;

    static std::unique_ptr<DecodedSimbaBody> Decode(std::vector<std::byte>::const_iterator& data);

    [[nodiscard]] std::string Dump() const override;

private:
    static constexpr uint16_t message_id = 16;
    friend struct decoder_traits<OrderExecutionMessage>;
};

} // namespace simba_decoder