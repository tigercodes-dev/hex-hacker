#include <iostream>
#include <string>
#include <iomanip>
#include "files.hpp"

using namespace hexhacker;

#define BLOCK_SIZE 4096
#define BYTES_PER_LINE 16

#define CALCULATE_LINE_OFFSET(buffer_offset, block) ((block) * BLOCK_SIZE + (buffer_offset))

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file> [options]\nUse " << argv[0] << " --help to see more detailed usage.\n";
        return 2;
    }

    std::string file;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i]; // Convert to std::string so we can use == operator.

        if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: " << argv[0] << " <file> [options]\n"
            "file: the file to open\n"
            "options:\n"
            "\t-h, --help - display this help message and exit.\n"
            "\t-v, --version - display the version and exit.\n";
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "HexHacker\n"
            "(c) 2026 tigercodes-dev <https://github.com/tigercodes-dev>\n"
            "Version 0.1.0\n";
            return 0;
        } else {
            // Check if it is an option or a file.
            if (arg[0] == '-') {
                std::cerr << "\e[31;1mError: Invalid option: \e[0m'" << arg << "'\n";
                return 2;
            } else {
                file = arg;
            }
        }
    }

    // Check if a file was specified
    if (file.empty()) {
        std::cerr << "\e[31;1mError: No file was specified.\e[0m\n";
        return 2;
    }

    try {
        BlockReader reader(file, BLOCK_SIZE); // Must catch std::runtime_error if file does not exist.

        unsigned char buffer[BLOCK_SIZE] = {0};
        unsigned int size;

        std::cout << "Offset (x) │ 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"
                     "───────────┼────────────────────────────────────────────────";

        do {
            size = reader.next_block(reinterpret_cast<char*>(buffer));
            for (int i = 0; i < size; i++) {
                // Print newline and offset on each line
                if (i % BYTES_PER_LINE == 0) {
                    std::cout << "\n0x" << std::hex << std::uppercase
                    << std::setfill('0') << std::setw(8) << CALCULATE_LINE_OFFSET(i, reader.get_current_block() - 1)
                    << std::dec << std::nouppercase << std::setfill(' ') << " │ ";
                }

                // Print hex byte
                unsigned int byte = static_cast<unsigned int>(buffer[i]);
                bool bolded = byte != 0;

                if (bolded) std::cout << "\e[1m"; // Bold non-zero bytes

                std::cout << std::hex << std::setfill('0') << std::setw(2) << std::uppercase
                << byte << std::dec << std::setfill(' ') << std::uppercase << ' ';

                if (bolded) std::cout << "\e[0m"; // Unbold the non zero bytes after
            }
        } while (size == BLOCK_SIZE); // Check if we have read final block after reading so we can read at least one block.

        std::cout << '\n';
    } catch (const std::runtime_error& e) {
        // Catch errors thrown by the file operations
        std::cerr << "\e[31;1mError: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
