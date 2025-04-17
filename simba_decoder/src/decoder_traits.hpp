#pragma once

#include <simba_decoder/include_simba_messages.hpp>

namespace simba_decoder {

// I like to use snake case for things like traits

template <typename T>
struct decoder_traits;

template <>
struct decoder_traits<OrderUpdateMessage> {
    static constexpr int id = OrderUpdateMessage::message_id;
    static std::unique_ptr<DecodedSimbaBody> decode(std::vector<std::byte>::const_iterator& it) {
        return OrderUpdateMessage::Decode(it);
    }
};

template <>
struct decoder_traits<OrderBookSnapshotMessage> {
    static constexpr int id = OrderBookSnapshotMessage::message_id;
    static std::unique_ptr<DecodedSimbaBody> decode(std::vector<std::byte>::const_iterator& it) {
        return OrderBookSnapshotMessage::Decode(it);
    }
};

template <>
struct decoder_traits<OrderExecutionMessage> {
    static constexpr int id = OrderExecutionMessage::message_id;
    static std::unique_ptr<DecodedSimbaBody> decode(std::vector<std::byte>::const_iterator& it) {
        return OrderExecutionMessage::Decode(it);
    }
};

} // namespace simba_decoder