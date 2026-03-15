#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file> [options]\nUse " << argv[0] << " --help to see more detailed usage.\n";
        return 1;
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

    std::cout << "File: " << file << std::endl;

    return 0;
}
