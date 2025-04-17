#include "decode_dispatcher.hpp"

#include <iostream>

#include "decoder_traits.hpp"

namespace simba_decoder {

DecodeDispatcher::DecodeDispatcher() {
    RegisterDecoder<decoder_traits<OrderUpdateMessage>>();
    RegisterDecoder<decoder_traits<OrderBookSnapshotMessage>>();
    RegisterDecoder<decoder_traits<OrderExecutionMessage>>();
}

DecodeDispatcher::SimbaMessagePtr DecodeDispatcher::Visit(uint16_t message_id, Iterator& it) const {
    auto func_it = table.find(message_id);
    if (func_it != table.end()) {
        return func_it->second(it);
    }
    return nullptr;
}

} // simba_decoder