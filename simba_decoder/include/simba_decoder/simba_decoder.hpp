#pragma once

#include <simba_decoder/include_simba_messages.hpp>
#include <simba_decoder/simba_message.hpp>

namespace simba_decoder {

std::vector<std::unique_ptr<DecodedSimbaBody>> Decode(const std::vector<std::byte>& data);

} // namespace simba_decoder