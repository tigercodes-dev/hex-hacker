export CXX := g++
export LD := g++
export BUILD_DIR := $(abspath build)

EXEC ?= $(BUILD_DIR)/hexhacker

.PHONY: all executable clean
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
