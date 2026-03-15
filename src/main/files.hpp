#ifndef HEXHACKER_FILES_H
#define HEXHACKER_FILES_H

#include <fstream>
#include <string>

namespace hexhacker {
    class BlockReader {
        public:
            BlockReader(std::string filepath, unsigned int block_size);

            ~BlockReader();

            unsigned int next_block(char* buffer);

            size_t get_total_blocks();

            unsigned long get_current_block();

            unsigned int get_block_size();

        private:
            unsigned int block_size;
            std::ifstream stream;
            unsigned long current_block;
    };
}

#endif