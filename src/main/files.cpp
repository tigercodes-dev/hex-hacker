#include "files.hpp"
#include <iostream>

namespace hexhacker {
    BlockReader::BlockReader(std::string filepath, unsigned int block_size): block_size(block_size), stream(filepath), current_block(0) {
        if (!stream.is_open()) {
            throw std::runtime_error("Unable to open file: " + filepath);
        }
    }

    BlockReader::~BlockReader() {
        stream.close();
    }

    unsigned int BlockReader::next_block(char* buffer) {
        stream.read(buffer, block_size);
        std::streamsize read_size = stream.gcount(); // Save read byte count
        current_block++;
        return static_cast<unsigned int>(read_size);
    }

    size_t BlockReader::get_total_blocks() {
        std::streampos old_pos = stream.tellg(); // Save old position
        stream.seekg(0, std::ios::end);
        size_t size = static_cast<size_t>(stream.tellg());
        stream.seekg(old_pos); // Return to old position

        return size / block_size + 1; // Calculate total blocks
    }

    unsigned long BlockReader::get_current_block() {
        return current_block;
    }

    unsigned int BlockReader::get_block_size() {
        return block_size;
    }
}

