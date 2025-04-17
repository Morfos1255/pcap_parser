#pragma once

#include <simba_decoder/include_simba_messages.hpp>

#include <unordered_map>

namespace simba_decoder {

class DecodeDispatcher {
public:
    using SimbaMessagePtr = std::unique_ptr<DecodedSimbaBody>;
    using Iterator = std::vector<std::byte>::const_iterator;
    using DecodeFunc = SimbaMessagePtr(*)(Iterator&);

    DecodeDispatcher();

    SimbaMessagePtr Visit(uint16_t message_id, Iterator& it) const;

private:
    std::unordered_map<uint16_t, DecodeFunc> table;

    template <typename decoder_trait>
    void RegisterDecoder() {
        table[decoder_trait::id] = &decoder_trait::decode;
    }
};

} // simba_decoder