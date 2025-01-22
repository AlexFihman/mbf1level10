# Compiler
CXX = g++

# C compiler for SFMT
CC = gcc

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O3 -DSFMT_MEXP=19937
CFLAGS = -Wall -Wextra -O3 -DSFMT_MEXP=19937

# Source files
SRCS = $(wildcard *.cpp) SFMT-src-1.5.1/SFMT.c

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = walk

# Include directories
INCLUDES = -I SFMT-src-1.5.1

# Build target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

SFMT-src-1.5.1/%.o: SFMT-src-1.5.1/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJS) $(TARGET)
