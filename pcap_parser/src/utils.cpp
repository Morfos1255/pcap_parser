#include "utils.hpp"

namespace pcap_parser {

std::chrono::system_clock::time_point get_timestamp(uint32_t ts_sec, uint32_t ts_subsec, bool is_nano) {
    using namespace std::chrono;

    auto secs = seconds(ts_sec);
    if (is_nano) {
        auto nanos = nanoseconds(ts_subsec);
        return system_clock::time_point(duration_cast<system_clock::duration>(secs + nanos));
    } else {
        auto micros = microseconds(ts_subsec);
        return system_clock::time_point(duration_cast<system_clock::duration>(secs + micros));
    }
}

}

