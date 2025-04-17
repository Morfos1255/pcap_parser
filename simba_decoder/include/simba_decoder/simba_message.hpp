#pragma once

#include <string>

namespace simba_decoder {

class DecodedSimbaBody {
public:
    virtual ~DecodedSimbaBody() = default;
    [[nodiscard]] virtual std::string Dump() const = 0;
};

#pragma pack(push, 1)

struct MarketDataPacketHeader {
    uint32_t msg_seq_num;
    uint16_t msg_size;
    uint16_t msg_flags;
    uint64_t sending_time;
};

struct IncrementalPacketHeader {
    uint64_t transact_time;
    uint32_t exchange_trading_session_id;
};

struct SBEHeader {
    uint16_t block_length;
    uint16_t template_id;
    uint16_t schema_id;
    uint16_t version;
};

struct RepGroupDim {
    uint16_t block_length;
    uint8_t num_in_group;
};

struct Decimal5 {
    int64_t mantissa;
};

struct Decimal5NULL {
    int64_t mantissa;
};

enum class MDFlagsSet : uint64_t
{
    Day = 0x1,
    IOC = 0x2,
    NonQuote = 0x4,
    EndOfTransaction = 0x1000,
    SecondLeg = 0x4000,
    FOK = 0x80000,
    Replace = 0x100000,
    Cancel = 0x200000,
    MassCancel = 0x400000,
    Negotiated = 0x4000000,
    MultiLeg = 0x8000000,
    CrossTrade = 0x20000000,
    COD = 0x100000000,
    ActiveSide = 0x20000000000,
    PassiveSide = 0x40000000000,
    Synthetic = 0x200000000000,
    RFS = 0x400000000000,
    SyntheticPassive = 0x200000000000000,
};

enum class MDUpdateAction : uint8_t
{
    New = 0,
    Change = 1,
    Delete = 2,
};

enum class MDEntryType : char
{
    Bid = '0',
    Offer = '1',
    EmptyBook = 'J',
};

#pragma pack(pop)

template <typename T>
struct simba_primitive_traits;

template <>
struct simba_primitive_traits<Decimal5> {
    static constexpr int8_t exponent = -5;

    static constexpr Decimal5 min() {
        return Decimal5{std::numeric_limits<int64_t>::min()};
    }

    static constexpr Decimal5 max() {
        return Decimal5{std::numeric_limits<int64_t>::max()};
    }
};

template <>
struct simba_primitive_traits<Decimal5NULL> {
    static constexpr int8_t exponent = -5;

    static constexpr Decimal5NULL min() {
        return Decimal5NULL{std::numeric_limits<int64_t>::min()};
    }

    static constexpr Decimal5NULL max() {
        return Decimal5NULL{std::numeric_limits<int64_t>::max() - 1};
    }

    static constexpr Decimal5NULL null() {
        return Decimal5NULL{std::numeric_limits<int64_t>::max()};
    }
};

template <typename DecimalType>
std::string ConvertDecimalToString(const DecimalType& decimal) {
    if constexpr (std::is_same_v<DecimalType, Decimal5NULL>) {
        if (decimal.mantissa == simba_primitive_traits<Decimal5NULL>::null().mantissa) {
            return "null";
        }
    }
    return std::to_string(decimal.mantissa * std::pow(10, simba_primitive_traits<DecimalType>::exponent));
}

} // namespace simba_decoder