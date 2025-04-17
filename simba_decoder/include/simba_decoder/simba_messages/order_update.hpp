#pragma once

#include <simba_decoder/simba_message.hpp>

#include <vector>

namespace simba_decoder {

template <typename T>
struct decoder_traits;

class OrderUpdateMessage final: public DecodedSimbaBody
{
public:
    struct __attribute__((packed)) OrderUpdate
    {
        int64_t md_entry_id;
        Decimal5 md_entry_px;
        int64_t md_entry_size;
        MDFlagsSet md_flags;
        uint64_t md_flags2;
        int32_t security_id;
        uint32_t rpt_seq;
        MDUpdateAction md_update_action;
        MDEntryType md_entry_type;
    } order_update;

    static std::unique_ptr<DecodedSimbaBody> Decode(std::vector<std::byte>::const_iterator& it);

    [[nodiscard]] std::string Dump() const override;

private:
    static constexpr uint16_t message_id = 15;
    friend struct decoder_traits<OrderUpdateMessage>;
};

} // namespace simba_decoder