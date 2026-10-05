CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++23 -Iheaders

RUN_TARGET = build/main
BENCH_TARGET = build/bench

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)

BENCH_SRC = bench/benchmark.cpp
BENCH_OBJ = build/benchmark.o


$(RUN_TARGET): $(OBJ)
	@mkdir -p build
	$(CXX) $(OBJ) -o $@

$(BENCH_TARGET): $(BENCH_OBJ)
	@mkdir -p build
	$(CXX) $(BENCH_OBJ) -o $@

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/benchmark.o: bench/benchmark.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(RUN_TARGET)
	./$(RUN_TARGET)

bench: $(BENCH_TARGET)
	./$(BENCH_TARGET)

clean:
	rm -r build

.PHONY: run bench clean