EXEC ?= $(BUILD_DIR)/hexhacker

SOURCES := $(shell find . -name "*.cpp")
OBJECTS := $(patsubst %.cpp, $(BUILD_DIR)/obj/%.o, $(SOURCES))
HEADERS := $(shell find . -name "*.hpp")

.PHONY: all clean
.SILENT:

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(LD) $(OBJECTS) -o $@
	echo "Linked object files."

$(BUILD_DIR)/obj/%.o: %.cpp $(HEADERS)
	mkdir -p $(@D)
	$(CXX) -c $< -o $@
	echo "Compiled C++ : $<"

clean:
	rm -f $(EXEC)
