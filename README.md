# pcap_parser
Parsing pcap files and decode data by the SIMBA MOEX specification
## Building
```console
mkdir build && cd build && cmake .. && make -j
```

## Usage
Use it like this
```console
./pcap_main <path/to/folder_with_pcaps> <path/to/output.jsonl>
```
For example:
```console
./pcap_main ../pcaps_folder ../output.jsonl
```
