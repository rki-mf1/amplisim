TARGET = amplisim
BUILD_DIR = ./build
SRC_DIR = ./src

SRCS := $(shell find $(SRC_DIR) -type f -name *.cpp)
OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.cpp=.o))

# Compiler
CXX = g++ -std=c++11
CC = $(CXX)

# Additional libraries for compilation of all builds types
override CXXFLAGS+=-I lib/htslib/

# Libraries for the linker (HTSlib)
override LDLIBS+=lib/htslib/libhts.a
override LDLIBS+=-largp
override LDLIBS+=-lz -lpthread
# Libraries for the linker (HTSlib - required for network support and cloud storage)
override LDLIBS+=-lcurl
# Libraries for the linker (HTSlib - CRAM support)
override LDLIBS+=-llzma -lbz2

# Date and version number from git
DATE := on $(shell git log --pretty=format:"%cd" --date=iso | cut -f 1,2 -d " " | head -n 1)
VERSION := 0.1.0
override CXXFLAGS+=-DDATE=\""$(DATE)"\" -DVERSION=\""$(VERSION)"\"

# Compiler flags
override CXXFLAGS+=-W -Wall -pedantic -O3 -DNDEBUG
#override CXXFLAGS+=-march=native

# DEBUG   build
#override CXXFLAGS+=-g -pg -O0 -DDEBUG

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDLIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) $(TOOLS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)