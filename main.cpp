#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <pcap_parser/pcap_parser.hpp>
#include <simba_decoder/simba_decoder.hpp>

namespace {

std::string format_timestamp(const std::chrono::system_clock::time_point& tp)
{
    using namespace std::chrono;

    // Get time_t (truncated to seconds)
    auto tt = system_clock::to_time_t(tp);
    auto tm = *std::localtime(&tt);

    // Compute fractional part (micro/nano seconds)
    auto since_epoch = tp.time_since_epoch();
    auto seconds_part = duration_cast<seconds>(since_epoch);
    auto subsecond = duration_cast<microseconds>(since_epoch - seconds_part);

    // Format
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setw(6) << std::setfill('0') << subsecond.count();

    return oss.str();
}

} // namespace

int main(int argc, char* argv[])
{
    using namespace pcap_parser;
    using namespace simba_decoder;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <pcap_folder> [output_file.jsonl]\n";
        return 1;
    }

    std::string folder_path = argv[1];
    std::string output_file = (argc >= 3) ? argv[2] : "simba_messages.jsonl";

    if (!std::filesystem::exists(folder_path) || !std::filesystem::is_directory(folder_path)) {
        std::cerr << "Error: '" << folder_path << "' is not a valid directory.\n";
        return 1;
    }

    std::ofstream out(output_file);

    int total_packet_count = 0;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".pcap") {
            std::cout << "Parsing file: " << entry.path() << '\n';
            PcapParser parser(entry.path().string());

            int file_packet_count = 0;
            while (auto packet = parser.Next()) {
                ++file_packet_count;
                ++total_packet_count;
                out << '[';
                auto simba_messages = Decode(packet->data);
                for (auto&& simba_message : simba_messages) {
                    if (simba_message) {
                        out << simba_message->Dump();
                        if (simba_messages.size() > 1) {
                            out << ',';
                        }
                    }
                }
                out << "]\n";
            }
        }
    }
    out << "\n";
    out.close();

    std::cout << "Done parsing. Total packets: " << total_packet_count << '\n';

    return 0;
}