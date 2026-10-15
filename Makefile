CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23

SRC = src/main.cpp
TARGET = build/main
HEADERS = $(wildcard src/*.hpp)

$(TARGET): $(SRC) $(HEADERS)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build

.PHONY: run clean
