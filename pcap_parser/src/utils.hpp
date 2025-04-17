#pragma once

#include <chrono>

namespace pcap_parser {

std::chrono::system_clock::time_point get_timestamp(uint32_t ts_sec, uint32_t ts_subsec, bool is_nano);

}