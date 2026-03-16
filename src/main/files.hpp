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

            inline size_t get_current_block() {
                return current_block;
            }

            inline unsigned int get_block_size() {
                return block_size;
            }

            size_t get_file_size();

        private:
            unsigned int block_size;
            std::ifstream stream;
            size_t current_block;
    };

    class BlockWriter {
        public:
            BlockWriter(std::string filepath, unsigned int block_size);

            ~BlockWriter();

            void write_next_block(char* buffer);

            void write_incomplete_block(char* buffer, unsigned int size);

            void skip_block();

            inline size_t get_current_block() {
                return current_block;
            }

            inline unsigned int get_block_size() {
                return block_size;
            }

        private:
            unsigned int block_size;
            std::ofstream stream;
            size_t current_block;
    };

    class IOException : public std::runtime_error {
        public:
            explicit IOException(const std::string& msg);
    };
}

#endif