#ifndef BOARD_H
#define BOARD_H

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
      if (c != v) moved = true;
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
      if (c != v) moved = true;
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
      if (c != v) moved = true;
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
      if (c != v) moved = true;
      for (int y = 0; y < N; ++y) {
        board[x][y] = c % 32;
        c /= 32;
      }
    }
    return moved;
  }

  bool operator==(const Board& b) const {
    return memcmp(board, b.board, sizeof(board)) == 0;
  }

  bool operator!=(const Board& b) const {
    return memcmp(board, b.board, sizeof(board)) != 0;
  }

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
