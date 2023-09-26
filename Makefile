TARGET = amplisim
BUILD_DIR = ./build
SRC_DIR = ./src

SRCS := $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.cpp=.o))

# Compiler
CXX = g++ -std=c++11
CC = $(CXX)

# Compiler flags
override CXXFLAGS+=-W -Wall -pedantic

# Compiler libraries
override CXXFLAGS+=-I lib/htslib/

# Linker libraries
override LDLIBS+=lib/htslib/libhts.a
override LDLIBS+=-lz -lpthread -lcurl -llzma -lbz2

# Linker libraries (macOS Darwin kernel)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
override LDLIBS+=-largp
endif

# Date and version
DATE := on $(shell git log --pretty=format:"%cd" --date=iso | cut -f 1,2 -d " " | head -n 1)
VERSION := 0.2.0
override CXXFLAGS+=-DDATE=\""$(DATE)"\" -DVERSION=\""$(VERSION)"\"

.PHONY: all
all: CXXFLAGS+=-O3 -DNDEBUG -Wno-missing-field-initializers -Wno-unused-function
all: $(TARGET)

.PHONY: debug
debug: CXXFLAGS+=-g -pg -O0 -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(TOOLS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)