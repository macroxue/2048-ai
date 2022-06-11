#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <algorithm>
#include "string.h"

const int N = 4;

class Board {
 public:
  Board() { memset(board, 0, sizeof(board)); }

  Board(int layout[N][N]) {
    for (int y = 0; y < N; ++y)
      for (int x = 0; x < N; ++x) board[x][y] = layout[y][x];
  }

  bool MoveLeft() {
    bool moved = false;
    for (int y = 0; y < N; ++y) {
      int v = 0;
      for (int x = 0; x < N; ++x) v = v * 32 + board[x][y];
      int c = move_map[v];
      if (c == v) continue;
      moved = true;
      for (int x = N - 1; x >= 0; --x) {
        board[x][y] = c % 32;
        c /= 32;
      }
    }
    return moved;
  }

  bool MoveRight() {
    bool moved = false;
    for (int y = 0; y < N; ++y) {
      int v = 0;
      for (int x = N - 1; x >= 0; --x) v = v * 32 + board[x][y];
      int c = move_map[v];
      if (c == v) continue;
      moved = true;
      for (int x = 0; x < N; ++x) {
        board[x][y] = c % 32;
        c /= 32;
      }
    }
    return moved;
  }

  bool MoveUp() {
    bool moved = false;
    for (int x = 0; x < N; ++x) {
      int v = 0;
      for (int y = 0; y < N; ++y) v = v * 32 + board[x][y];
      int c = move_map[v];
      if (c == v) continue;
      moved = true;
      for (int y = N - 1; y >= 0; --y) {
        board[x][y] = c % 32;
        c /= 32;
      }
    }
    return moved;
  }

  bool MoveDown() {
    bool moved = false;
    for (int x = 0; x < N; ++x) {
      int v = 0;
      for (int y = N - 1; y >= 0; --y) v = v * 32 + board[x][y];
      int c = move_map[v];
      if (c == v) continue;
      moved = true;
      for (int y = 0; y < N; ++y) {
        board[x][y] = c % 32;
        c /= 32;
      }
    }
    return moved;
  }

  unsigned long long Compact() const {
    unsigned long long compact_board = 0;
    for (int y = 0; y < N; ++y)
      for (int x = 0; x < N; ++x)
        compact_board = compact_board * 16 + board[x][y];
    return compact_board;
  }

  int MaxRank() const {
    int max_rank = 0;
    for (int y = 0; y < N; ++y)
      for (int x = 0; x < N; ++x) max_rank = std::max(max_rank, board[x][y]);
    return max_rank;
  }

  void Show() const {
    for (int y = 0; y < N; ++y) {
      for (int x = 0; x < N; ++x)
        printf("|%5d", board[x][y] ? 1 << board[x][y] : 0);
      printf("|\n");
    }
  }

  bool operator==(const Board& b) const {
    return memcmp(board, b.board, sizeof(board)) == 0;
  }

  bool operator!=(const Board& b) const {
    return memcmp(board, b.board, sizeof(board)) != 0;
  }

  const int* operator[](int col) const { return board[col]; }
  int* operator[](int col) { return board[col]; }

  typedef bool (Board::*Move)();
  static const Move moves[];
  static const char* move_names[];

  static void BuildMoveMap();

 protected:
  int board[N][N];

 private:
  static unsigned int move_map[1 << 20];
};

#endif
