#pragma once
#include <cstdint>
#include "Board.hpp"

class MoveSorter {
public:
    void add(uint64_t move, int score) {
      assert(size < Board::WIDTH);
      int pos = size++;
      for(; pos && entries[pos-1].score > score; --pos) 
        entries[pos] = entries[pos-1];
      entries[pos].move = move;
      entries[pos].score = score;
    }

    uint64_t getNext(){
      if(size) return entries[--size].move;
      else return 0;
    }

    void reset() {
      size = 0;
    }

    MoveSorter(): size{0} {}
private:
    unsigned int size;

    struct {
        uint64_t move;
        int score;
    } entries[Board::WIDTH];
};