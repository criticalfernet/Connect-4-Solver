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

bench: $(TARGET)
	./$(TARGET) bench

benchT: $(TARGET)
	./$(TARGET) benchTT

clean:
	rm -rf build

.PHONY: run clean bench benchT
