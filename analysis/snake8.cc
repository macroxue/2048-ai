#include <stdio.h>

#include "tuple.h"

using Snake64 = Tuple<1, 1, 1, 1,   // row 0
                      1, 1, 1, 1,   // row 1
                      6, 6, 6, 6,   // row 2
                      6, 6, 6, 6>;  // row 3

template <>
const int Snake64::kMaxAnchorRank = 6;

template <>
bool Snake64::TupleBoard::IsRegular() const {
  // Rightmost column and bottom row are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[0][2] > anchor_rank - 1) return false;
  if (board[1][2] > anchor_rank - 1) return false;
  if (board[2][2] > anchor_rank - 1) return false;
  if (board[3][2] > anchor_rank - 1) return false;
  if (board[0][3] > anchor_rank - 1) return false;
  if (board[1][3] > anchor_rank - 1) return false;
  if (board[2][3] > anchor_rank - 1) return false;
  if (board[3][3] > anchor_rank - 1) return false;
  return true;
}

template <>
bool Snake64::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if (board[3][2] == anchor_rank - 1 &&
      (board[2][2] == anchor_rank - 1 || board[3][3] == anchor_rank - 1))
    return true;
  return false;
}

template <>
Snake64::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Snake128 = Tuple<1, 1, 1, 1,   // row 0
                       1, 1, 1, 1,   // row 1
                       7, 7, 7, 7,   // row 2
                       7, 7, 7, 7>;  // row 3

template <>
const int Snake128::kMaxAnchorRank = 7;

template <>
bool Snake128::TupleBoard::IsRegular() const {
  // Rightmost column and bottom row are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[0][2] > anchor_rank - 1) return false;
  if (board[1][2] > anchor_rank - 1) return false;
  if (board[2][2] > anchor_rank - 1) return false;
  if (board[3][2] > anchor_rank - 1) return false;
  if (board[0][3] > anchor_rank - 1) return false;
  if (board[1][3] > anchor_rank - 1) return false;
  if (board[2][3] > anchor_rank - 1) return false;
  if (board[3][3] > anchor_rank - 1) return false;
  return true;
}

template <>
bool Snake128::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if (board[3][2] == anchor_rank - 1 &&
      (board[2][2] == anchor_rank - 1 || board[3][3] == anchor_rank - 1))
    return true;
  return false;
}

template <>
Snake128::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Snake256 = Tuple<1, 1, 1, 1,   // row 0
                       1, 1, 1, 1,   // row 1
                       8, 8, 8, 8,   // row 2
                       8, 8, 8, 8>;  // row 3

template <>
const int Snake256::kMaxAnchorRank = 8;

template <>
bool Snake256::TupleBoard::IsRegular() const {
  // Rightmost column and bottom row are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[0][2] > anchor_rank - 1) return false;
  if (board[1][2] > anchor_rank - 1) return false;
  if (board[2][2] > anchor_rank - 1) return false;
  if (board[3][2] > anchor_rank - 1) return false;
  if (board[0][3] > anchor_rank - 1) return false;
  if (board[1][3] > anchor_rank - 1) return false;
  if (board[2][3] > anchor_rank - 1) return false;
  if (board[3][3] > anchor_rank - 1) return false;
  return true;
}

template <>
bool Snake256::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if (board[3][2] == anchor_rank - 1 &&
      (board[2][2] == anchor_rank - 1 || board[3][3] == anchor_rank - 1))
    return true;
  return false;
}

template <>
Snake256::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

int main(int argc, const char *argv[]) {
  Board::BuildMoveMap();
  int tile = 64;
  if (argc > 1) tile = atoi(argv[1]);
  if (tile != 64 && tile != 128 && tile != 256) {
    puts("Specify one of 64, 128 and 256 tile.");
    exit(-1);
  }

  char message[512];
  sprintf(message,
          "Below are moves and probabilities for getting a new %d tile at "
          "position (4)\ngiven tiles at positions (1) (2) (3) (4) (5) (6) (7) "
          "(8) as specified above.",
          tile);

  if (tile == 64) {
    Snake64 snake;
    puts("   1024  2048  4096  8192");
    puts("    512   256   128    64");
    puts("    (1)   (2)   (3)   (4)");
    puts("    (5)   (6)   (7)   (8)");
    puts(message);
    snake.Show();
  }
  if (tile == 128) {
    Snake128 snake;
    puts("   2048  4096  8192 16384");
    puts("   1024   512   256   128");
    puts("    (1)   (2)   (3)   (4)");
    puts("    (5)   (6)   (7)   (8)");
    puts(message);
    snake.Show();
  }
  if (tile == 256) {
    Snake256 snake;
    puts("   4096  8192 16384 32768");
    puts("   2048  1024   512   256");
    puts("    (1)   (2)   (3)   (4)");
    puts("    (5)   (6)   (7)   (8)");
    puts(message);
    snake.Show();
  }
  return 0;
}
