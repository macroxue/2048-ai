#include <stdio.h>

#include "../tuple.h"

using Pdf64 = Tuple<1, 1, 1, 6,   // row 0
                    1, 1, 1, 6,   // row 1
                    1, 1, 6, 6,   // row 2
                    6, 6, 6, 6>;  // row 3

template <>
const int Pdf64::kMaxAnchorRank = 6;

template <>
bool Pdf64::TupleBoard::IsRegular() const {
  // Rightmost column and bottom row are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
  if (board[2][2] > anchor_rank - 1) return false;
  if (board[3][2] > anchor_rank - 1) return false;
  if (board[0][3] > anchor_rank - 1) return false;
  if (board[1][3] > anchor_rank - 1) return false;
  if (board[2][3] > anchor_rank - 1) return false;
  if (board[3][3] > anchor_rank - 1) return false;
  return true;
}

template <>
bool Pdf64::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if (board[2][2] == anchor_rank - 1 &&
      (board[2][3] == anchor_rank - 1 || board[3][2] == anchor_rank - 1))
    return true;
  return false;
}

template <>
Pdf64::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Pdf128 = Tuple<1, 1, 1, 7,   // row 0
                     1, 1, 1, 7,   // row 1
                     1, 1, 7, 7,   // row 2
                     7, 7, 7, 7>;  // row 3

template <>
const int Pdf128::kMaxAnchorRank = 7;

template <>
bool Pdf128::TupleBoard::IsRegular() const {
  // Rightmost column and bottom row are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
  if (board[2][2] > anchor_rank - 1) return false;
  if (board[3][2] > anchor_rank - 1) return false;
  if (board[0][3] > anchor_rank - 1) return false;
  if (board[1][3] > anchor_rank - 1) return false;
  if (board[2][3] > anchor_rank - 1) return false;
  if (board[3][3] > anchor_rank - 1) return false;
  return true;
}

template <>
bool Pdf128::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if (board[2][2] == anchor_rank - 1 &&
      (board[2][3] == anchor_rank - 1 || board[3][2] == anchor_rank - 1))
    return true;
  return false;
}

template <>
Pdf128::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Pdf256 = Tuple<1, 1, 1, 8,   // row 0
                     1, 1, 1, 8,   // row 1
                     1, 1, 8, 8,   // row 2
                     8, 8, 8, 8>;  // row 3

template <>
const int Pdf256::kMaxAnchorRank = 8;

template <>
bool Pdf256::TupleBoard::IsRegular() const {
  // Rightmost column and bottom row are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
  if (board[2][2] > anchor_rank - 1) return false;
  if (board[3][2] > anchor_rank - 1) return false;
  if (board[0][3] > anchor_rank - 1) return false;
  if (board[1][3] > anchor_rank - 1) return false;
  if (board[2][3] > anchor_rank - 1) return false;
  if (board[3][3] > anchor_rank - 1) return false;
  return true;
}

template <>
bool Pdf256::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if (board[2][2] == anchor_rank - 1 &&
      (board[2][3] == anchor_rank - 1 || board[3][2] == anchor_rank - 1))
    return true;
  return false;
}

template <>
Pdf256::Tuple() {
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
          "position (3)\ngiven tiles at positions (1) (2) (3) (4) (5) (6) (7) "
          "(8) as specified above.",
          tile);

  if (tile == 64) {
    Pdf64 pdf;
    puts("   8192  2048   512  (1)");
    puts("   4096  1024   256  (2)");
    puts("    128    64   (3)  (4)");
    puts("    (5)   (6)   (7)  (8)");
    puts(message);
    pdf.Show();
  }
  if (tile == 128) {
    Pdf128 pdf;
    puts("  16384  4096  1024  (1)");
    puts("   8192  2048   512  (2)");
    puts("    256   128   (3)  (4)");
    puts("    (5)   (6)   (7)  (8)");
    puts(message);
    pdf.Show();
  }
  if (tile == 256) {
    Pdf256 pdf;
    puts("  32768  8192  2048  (1)");
    puts("  16384  4096  1024  (2)");
    puts("    512   256   (3)  (4)");
    puts("    (5)   (6)   (7)  (8)");
    puts(message);
    pdf.Show();
  }
  return 0;
}
