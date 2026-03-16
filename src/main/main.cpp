#include <iostream>
#include <string>
#include <iomanip>
#include <ncurses.h>
#include <curses.h>
#include <cctype>
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

        initscr();
        scrollok(stdscr, true);

        bool colors = has_colors();

        if (colors) {
            start_color();
            init_pair(1, COLOR_GREEN, COLOR_BLACK); // Title
            init_color(8, 750, 750, 750);
            init_pair(2, 8, COLOR_BLACK); // Grayed Out Bytes
            init_color(9, 1000, 500, 500);
            init_pair(3, 9, COLOR_BLACK); // Changed Bytes
        }

        cbreak();
        noecho();

        if (colors) attron(COLOR_PAIR(1));
        printw("File: %s | %lu bytes | %lu blocks of %u\n", file.c_str(), reader.get_file_size(), reader.get_total_blocks(), BLOCK_SIZE);
        if (colors) attroff(COLOR_PAIR(1));
        printw("Offset (x) | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n"
               "-----------+------------------------------------------------");

        do {
            size = reader.next_block(reinterpret_cast<char*>(buffer));
            for (int i = 0; i < size; i++) {
                // Print newline and offset on each line
                if (i % BYTES_PER_LINE == 0) {
                    printw("\n0x%.8lX |", CALCULATE_LINE_OFFSET(i, reader.get_current_block() - 1));
                }

                // Print hex byte
                bool bolded = buffer[i] != 0;

                if (bolded) attron(A_BOLD); // Bold non-zero bytes
                else if (colors) attron(COLOR_PAIR(2)); // Gray out zero bytes.

                printw(" %.2X", buffer[i]);

                if (bolded) attroff(A_BOLD); // Unbold the non zero bytes after
                else if (colors) attroff(COLOR_PAIR(2)); // Return to normal color.
            }
        } while (size == BLOCK_SIZE); // Check if we have read final block after reading so we can read at least one block.

        printw("\n");
        refresh();
    } catch (const IOException& e) {
        // Catch errors thrown by the file operations
        endwin();
        std::cerr << "\e[31;1m" << e.what() << "\e[0m\n";
        return 1;
    }

    keypad(stdscr, true);

    bool halfbyte = false; // Set if cursor is halfway through byte

    int y, x;
    getyx(stdscr, y, x);

    y--;
    x += 13;
    move(y, x);

    refresh();

    int chr;
    while ((chr = getch()) != 'q') {
        if (std::isxdigit(chr)) {
            char upper = std::toupper(chr);
            bool change = ((inch() & A_CHARTEXT) != upper) || (PAIR_NUMBER(mvinch(x - 1, y) & A_COLOR) == 3);

            if (change) attron(COLOR_PAIR(3)); // Color it red
            addch(upper);
            if (change) attroff(COLOR_PAIR(3)); // Stop coloring red
            
            x += (halfbyte ? 2 : 1);
            
            if (change) {
                if (halfbyte) {
                    mvchgat(y, x - 3, 1, A_NORMAL, 3, NULL);
                } else {
                    mvchgat(y, x, 1, A_NORMAL, 3, NULL);
                }
            }

            if (x >= 10 + 3 *BYTES_PER_LINE) {
                x = 13;
                y++;
            }
            move(y, x);
            halfbyte = !halfbyte;
        }
        switch (chr) {
            case KEY_UP:
                if (y > 0) y--;
                if (halfbyte) x--;
                move(y, x);
                halfbyte = false;
                break;
            case KEY_DOWN:
                y++;
                if (halfbyte) x--;
                move(y, x);
                halfbyte = false;
                break;
            case KEY_LEFT:
                if (x > 13) x -= (halfbyte ? 1 : 3);
                else {
                    // Go backwards one line
                    x = 10 + 3 * BYTES_PER_LINE;
                    y--;
                }
                move(y, x);
                halfbyte = false;
                break;
            case KEY_RIGHT:
                if (x < 10 + 3 * BYTES_PER_LINE) x += (halfbyte ? 2 : 3);
                else {
                    // Go forwards one line
                    x = 13;
                    y++;
                }
                move(y, x);
                halfbyte = false;
                break;
        }

        refresh();
    }
    endwin();

    return 0;
}
