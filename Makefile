export CXX := g++
export LD := g++
export BUILD_DIR := $(abspath build)

VERSION = 0.1.0

EXEC ?= $(BUILD_DIR)/hexhacker

.PHONY: all executable debug always clean version help
.SILENT:

all: executable

executable: $(EXEC)

$(EXEC): always
	$(MAKE) -C src/main EXEC=$(EXEC)
	echo "Executable file: $@ is ready."

debug: always
	$(MAKE) -C src/main EXEC=$(EXEC) debug
	echo "Executable file with debug: $(EXEC) is ready."

always:
	mkdir -p build

clean:
	$(MAKE) -C src/main clean EXEC=$(EXEC)
	rm -rf $(BUILD_DIR)/*

version:
	echo "HexHacker Version $(VERSION)"
	echo "See the GitHub repository for more info: https://github.com/tigercodes-dev/hex-hacker"

help:
	echo "HexHacker by TigerCodes <https://github.com/tigercodes-dev>"
	echo "Version $(VERSION)"
	echo "See the GitHub repository for more info: https://github.com/tigercodes-dev/hex-hacker"
	echo
	echo "Makefile Commands:"
	echo "  help: show this help message"
	echo "  version: show a version message"
	echo
	echo "  all, executable (default): build the executable"
	echo "  debug: build the executable with debug information"
	echo
	echo "  clean: delete all files in the build directory"
