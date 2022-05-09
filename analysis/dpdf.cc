#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "tuple.h"

using Dpdf64 = Tuple<1, 1, 1, 6,   // row 0
                     1, 1, 1, 6,   // row 1
                     6, 6, 6, 6,   // row 2
                     6, 6, 6, 6>;  // row 3

template <>
const int Dpdf64::kMaxAnchorRank = 6;

template <>
bool Dpdf64::TupleBoard::IsRegular() const {
  // Rightmost column and two bottom rows are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
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
bool Dpdf64::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if ((board[2][2] == anchor_rank - 1 &&
      (board[1][2] == anchor_rank - 1 ||
       board[2][3] == anchor_rank - 1 ||
       board[3][2] == anchor_rank - 1)) ||
      (board[3][1] == anchor_rank - 1 &&
       (board[3][0] == anchor_rank - 1 ||
        board[3][2] == anchor_rank - 1)))
    return true;
  return false;
}

template <>
Dpdf64::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Dpdf128 = Tuple<1, 1, 1, 7,   // row 0
                      1, 1, 1, 7,   // row 1
                      7, 7, 7, 7,   // row 2
                      7, 7, 7, 7>;  // row 3

template <>
const int Dpdf128::kMaxAnchorRank = 7;

template <>
bool Dpdf128::TupleBoard::IsRegular() const {
  // Rightmost column and two bottom rows are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
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
bool Dpdf128::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if ((board[2][2] == anchor_rank - 1 &&
      (board[1][2] == anchor_rank - 1 ||
       board[2][3] == anchor_rank - 1 ||
       board[3][2] == anchor_rank - 1)) ||
      (board[3][1] == anchor_rank - 1 &&
       (board[3][0] == anchor_rank - 1 ||
        board[3][2] == anchor_rank - 1)))
    return true;
  return false;
}

template <>
Dpdf128::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Dpdf256 = Tuple<1, 1, 1, 8,   // row 0
                      1, 1, 1, 8,   // row 1
                      8, 8, 8, 8,   // row 2
                      8, 8, 8, 8>;  // row 3

template <>
const int Dpdf256::kMaxAnchorRank = 8;

template <>
bool Dpdf256::TupleBoard::IsRegular() const {
  // Rightmost column and two bottom rows are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
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
bool Dpdf256::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if ((board[2][2] == anchor_rank - 1 &&
      (board[1][2] == anchor_rank - 1 ||
       board[2][3] == anchor_rank - 1 ||
       board[3][2] == anchor_rank - 1)) ||
      (board[3][1] == anchor_rank - 1 &&
       (board[3][0] == anchor_rank - 1 ||
        board[3][2] == anchor_rank - 1)))
    return true;
  return false;
}

template <>
Dpdf256::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Dpdf512 = Tuple<1, 1, 1, 9,   // row 0
                      1, 1, 1, 9,   // row 1
                      9, 9, 9, 9,   // row 2
                      9, 9, 9, 9>;  // row 3

template <>
const int Dpdf512::kMaxAnchorRank = 9;

template <>
bool Dpdf512::TupleBoard::IsRegular() const {
  // Rightmost column and two bottom rows are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
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
bool Dpdf512::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if ((board[2][2] == anchor_rank - 1 &&
      (board[1][2] == anchor_rank - 1 ||
       board[2][3] == anchor_rank - 1 ||
       board[3][2] == anchor_rank - 1)) ||
      (board[3][1] == anchor_rank - 1 &&
       (board[3][0] == anchor_rank - 1 ||
        board[3][2] == anchor_rank - 1)))
    return true;
  return false;
}

template <>
Dpdf512::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

using Dpdf1024 = Tuple<1, 1, 1, 10,   // row 0
                       1, 1, 1, 10,   // row 1
                       10, 10, 10, 10,   // row 2
                       10, 10, 10, 10>;  // row 3

template <>
const int Dpdf1024::kMaxAnchorRank = 10;

template <>
bool Dpdf1024::TupleBoard::IsRegular() const {
  // Rightmost column and two bottom rows are smaller than anchor rank.
  int anchor_rank = kMaxAnchorRank;
  if (board[3][0] > anchor_rank - 1) return false;
  if (board[3][1] > anchor_rank - 1) return false;
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
bool Dpdf1024::TupleBoard::IsGoal() const {
  int anchor_rank = kMaxAnchorRank;
  if ((board[2][2] == anchor_rank - 1 &&
      (board[1][2] == anchor_rank - 1 ||
       board[2][3] == anchor_rank - 1 ||
       board[3][2] == anchor_rank - 1)) ||
      (board[3][1] == anchor_rank - 1 &&
       (board[3][0] == anchor_rank - 1 ||
        board[3][2] == anchor_rank - 1)))
    return true;
  return false;
}

template <>
Dpdf1024::Tuple() {
  printf("Computing tuple_moves for %ld tuples ...     ", kNumTuples);
  fflush(stdout);
  Compute();
  printf("done\n");
}

int* ReadIndex(int max_tile) {
  auto is_valid_tile = [&](int tile) {
    return tile != 1 && 0 <= tile && tile <= max_tile && (tile & (tile - 1)) == 0;
  };
  auto tile_to_rank = [](int tile) {
    if (tile == 0) return 0;
    int rank;
    for (rank = 1, tile >>= 2; tile; ++rank, tile >>= 1);
    return rank;
  };
  static int index[10];
  for (char* line; line = readline("> "); free(line)) {
    if (strlen(line) > 0) add_history(line);
    int num_tiles = sscanf(line, "%d %d %d %d %d %d %d %d %d %d",
                           &index[0], &index[1], &index[2], &index[3], &index[4],
                           &index[5], &index[6], &index[7], &index[8], &index[9]);
    if (num_tiles != 10) {
      puts("Please enter 10 tiles as the index.");
      continue;
    }
    if (!std::all_of(index, index + 10, is_valid_tile)) {
      printf("Each tile must be either 0 or a power of 2 between 2 and %d.\n",
             max_tile);
      continue;
    }
    free(line);
    for (auto& tile : index) tile = tile_to_rank(tile);
    return index;
  }
  exit(0);
}

int main(int argc, const char *argv[]) {
  Board::BuildMoveMap();
  int tile = 64;
  if (argc > 1) tile = atoi(argv[1]);
  if (tile != 64 && tile != 128 && tile != 256 && tile != 512 && tile != 1024) {
    puts("Specify one of 64, 128, 256, 512 and 1024 tiles.");
    exit(-1);
  }

  char message[512];
  sprintf(message,
          "To query the probability of getting a new %d tile, "
          "input the small tiles at positions (1) (2) (3) (4) (5) (6) (7) "
          "(8) (9) (10) as indicated above.",
          tile);

  if (tile == 64) {
    Dpdf64 dpdf;
    puts("   2048  1024   512  (1)");
    puts("    256   128    64  (2)");
    puts("    (3)   (4)   (5)  (6)");
    puts("    (7)   (8)   (9) (10)");
    puts(message);
    for (;;) dpdf.Query(ReadIndex(tile / 2));
  }
  if (tile == 128) {
    Dpdf128 dpdf;
    puts("   4096  2048  1024  (1)");
    puts("    512   256   128  (2)");
    puts("    (3)   (4)   (5)  (6)");
    puts("    (7)   (8)   (9) (10)");
    puts(message);
    for (;;) dpdf.Query(ReadIndex(tile / 2));
  }
  if (tile == 256) {
    Dpdf256 dpdf;
    puts("   8192  4096  2048  (1)");
    puts("   1024   512   256  (2)");
    puts("    (3)   (4)   (5)  (6)");
    puts("    (7)   (8)   (9) (10)");
    puts(message);
    for (;;) dpdf.Query(ReadIndex(tile / 2));
  }
  if (tile == 512) {
    Dpdf512 dpdf;
    puts("  16384  8192  4096  (1)");
    puts("   2048  1024   512  (2)");
    puts("    (3)   (4)   (5)  (6)");
    puts("    (7)   (8)   (9) (10)");
    puts(message);
    for (;;) dpdf.Query(ReadIndex(tile / 2));
  }
  if (tile == 1024) {
    Dpdf1024 dpdf;
    puts("  32768 16384  8192  (1)");
    puts("   4096  2048  1024  (2)");
    puts("    (3)   (4)   (5)  (6)");
    puts("    (7)   (8)   (9) (10)");
    puts(message);
    for (;;) dpdf.Query(ReadIndex(tile / 2));
  }
  return 0;
}
