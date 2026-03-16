#include "files.hpp"
#include <iostream>

namespace hexhacker {
    // BlockReader

    BlockReader::BlockReader(std::string filepath, unsigned int block_size): block_size(block_size), stream(filepath), current_block(0) {
        if (!stream.is_open()) {
            throw IOException("Unable to open file: " + filepath);
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

    size_t BlockReader::get_current_block() {
        return current_block;
    }

    unsigned int BlockReader::get_block_size() {
        return block_size;
    }

    // Block Writer

    BlockWriter::BlockWriter(std::string filepath, unsigned int block_size): block_size(block_size), stream(filepath), current_block(0) {
        if (!stream.is_open()) {
            throw IOException("Unable to open file: " + filepath);
        }
    }

    BlockWriter::~BlockWriter() {
        stream.close();
    }

    void BlockWriter::write_next_block(char* buffer) {
        stream.write(buffer, block_size);
        current_block++;
        if (!stream) {
            throw IOException("Unable to write to the file.");
        }
    }

    void BlockWriter::skip_block() {
        current_block++;
        stream.seekp(block_size, std::ios::cur); // Seek the pointer forward.
    }

    size_t BlockWriter::get_current_block() {
        return current_block;
    }

    unsigned int BlockWriter::get_block_size() {
        return block_size;
    }

    void BlockWriter::write_incomplete_block(char* buffer, unsigned int size) {
        if (size > block_size) {
            throw std::invalid_argument("Incomplete block size of " + std::to_string(size) + " bytes is greater than the block size.");
        }
        stream.write(buffer, size);
        current_block++;
        stream.seekp(block_size - size, std::ios::cur);
    }

    // IOException

    IOException::IOException(const std::string& msg): std::runtime_error(msg) {}
}

