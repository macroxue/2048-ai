#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>

#include "tuple.h"

using Snake11 = Tuple<1, 1, 1, 1,   // row 0
                      1, 10, 9, 8,  // row 1
                      7, 7, 7, 7,   // row 2
                      6, 6, 6, 6>;  // row 3

template <>
const int Snake11::kMaxAnchorRank = 9;
template <>
const int Snake11::kAnchorCol = 1;
template <>
const int Snake11::kAnchorRow = 1;
template <>
const char Snake11::kTupleFile[] = "snake_moves.11";

template <>
bool Snake11::TupleBoard::IsRegular(bool transposed) const {
  if (transposed) {
    if (board[0][0] == board[0][1] || board[0][1] == board[0][2] ||
        board[0][2] == board[0][3])
      return false;
    if (board[1][0] <= board[1][1]) return false;
    if (board[0][0] == board[1][0]) return false;
    if (board[0][1] <= board[1][1]) return false;
    if (board[0][2] <= board[1][2]) return false;
    if (board[0][3] <= board[1][3]) return false;
    if (board[0][3] <= board[1][0]) return false;

    // Two bottom rows are smaller than anchor rank.
    int anchor_rank = std::min(kMaxAnchorRank, board[1][0]);
    if (board[1][1] > anchor_rank - 0) return false;
    if (board[1][2] > anchor_rank - 1) return false;
    if (board[1][3] > anchor_rank - 2) return false;
    if (board[2][0] > anchor_rank - 3) return false;
    if (board[2][1] > anchor_rank - 3) return false;
    if (board[2][2] > anchor_rank - 3) return false;
    if (board[2][3] > anchor_rank - 3) return false;
    if (board[3][0] > anchor_rank - 4) return false;
    if (board[3][1] > anchor_rank - 4) return false;
    if (board[3][2] > anchor_rank - 4) return false;
    if (board[3][3] > anchor_rank - 4) return false;
  } else {
    // Upper 4x2 configuration:
    // (0,0) != (1,0) != (2,0) != (3,0)
    //  !=        V        V        V
    // (0,1) >  (1,1)    (2,1)    (3,1)
    // plus (3,0) > (0,1)
    if (board[0][0] == board[1][0] || board[1][0] == board[2][0] ||
        board[2][0] == board[3][0])
      return false;
    if (board[0][1] <= board[1][1]) return false;
    if (board[0][0] == board[0][1]) return false;
    if (board[1][0] <= board[1][1]) return false;
    if (board[2][0] <= board[2][1]) return false;
    if (board[3][0] <= board[3][1]) return false;
    if (board[3][0] <= board[0][1]) return false;

    // Two bottom rows are smaller than anchor rank.
    int anchor_rank = std::min(kMaxAnchorRank, board[0][1]);
    if (board[1][1] > anchor_rank - 0) return false;
    if (board[2][1] > anchor_rank - 1) return false;
    if (board[3][1] > anchor_rank - 2) return false;
    if (board[0][2] > anchor_rank - 3) return false;
    if (board[1][2] > anchor_rank - 3) return false;
    if (board[2][2] > anchor_rank - 3) return false;
    if (board[3][2] > anchor_rank - 3) return false;
    if (board[0][3] > anchor_rank - 4) return false;
    if (board[1][3] > anchor_rank - 4) return false;
    if (board[2][3] > anchor_rank - 4) return false;
    if (board[3][3] > anchor_rank - 4) return false;
  }
  return true;
}

template <>
bool Snake11::TupleBoard::IsGoal(bool transposed) const {
  if (transposed) {
    int anchor_rank = std::min(kMaxAnchorRank, board[1][0] - 1);
    return board[1][1] == anchor_rank && board[1][2] == anchor_rank - 1 &&
           (board[1][3] == anchor_rank - 1 || board[1][3] == anchor_rank - 2 ||
            (board[1][3] == anchor_rank - 3 && board[2][3] == anchor_rank - 3));
  } else {
    int anchor_rank = std::min(kMaxAnchorRank, board[0][1] - 1);
    return board[1][1] == anchor_rank && board[2][1] == anchor_rank - 1 &&
           (board[3][1] == anchor_rank - 1 || board[3][1] == anchor_rank - 2 ||
            (board[3][1] == anchor_rank - 3 && board[3][2] == anchor_rank - 3));
  }
}

using Snake9 = Tuple<1, 1, 1, 1,   // row 0
                     1, 1, 1, 9,   // row 1
                     8, 8, 8, 8,   // row 2
                     7, 7, 7, 7>;  // row 3

template <>
const int Snake9::kMaxAnchorRank = 8;
template <>
const int Snake9::kAnchorCol = 3;
template <>
const int Snake9::kAnchorRow = 1;
template <>
const char Snake9::kTupleFile[] = "snake_moves.9";

template <>
bool Snake9::TupleBoard::IsRegular(bool transposed) const {
  if (transposed) {
    if (board[0][0] == board[0][1] || board[0][1] == board[0][2] ||
        board[0][2] == board[0][3])
      return false;
    if (board[1][0] <= board[1][1] || board[1][1] <= board[1][2] ||
        board[1][2] <= board[1][3])
      return false;
    if (board[0][0] == board[1][0]) return false;
    if (board[0][1] == board[1][1]) return false;
    if (board[0][2] == board[1][2]) return false;
    if (board[0][3] <= board[1][3]) return false;
    if (board[0][3] <= board[1][0]) return false;

    int anchor_rank =
        std::min(kMaxAnchorRank, std::min(board[1][2], board[0][3]));
    if (board[2][0] > anchor_rank - 1) return false;
    if (board[2][1] > anchor_rank - 1) return false;
    if (board[2][2] > anchor_rank - 1) return false;
    if (board[2][3] > anchor_rank - 1) return false;
    if (board[3][0] > anchor_rank - 2) return false;
    if (board[3][1] > anchor_rank - 2) return false;
    if (board[3][2] > anchor_rank - 2) return false;
    if (board[3][3] > anchor_rank - 2) return false;
  } else {
    // Upper 4x2 configuration:
    // (0,0) != (1,0) != (2,0) != (3,0)
    //  !=       !=       !=        V
    // (0,1) >  (1,1) >  (2,1) >  (3,1)
    // plus (3,0) > (0,1)
    if (board[0][0] == board[1][0] || board[1][0] == board[2][0] ||
        board[2][0] == board[3][0])
      return false;
    if (board[0][1] <= board[1][1] || board[1][1] <= board[2][1] ||
        board[2][1] <= board[3][1])
      return false;
    if (board[0][0] == board[0][1]) return false;
    if (board[1][0] == board[1][1]) return false;
    if (board[2][0] == board[2][1]) return false;
    if (board[3][0] <= board[3][1]) return false;
    if (board[3][0] <= board[0][1]) return false;

    // Two bottom rows are smaller than anchor rank.
    int anchor_rank =
        std::min(kMaxAnchorRank, std::min(board[2][1], board[3][0]));
    if (board[0][2] > anchor_rank - 1) return false;
    if (board[1][2] > anchor_rank - 1) return false;
    if (board[2][2] > anchor_rank - 1) return false;
    if (board[3][2] > anchor_rank - 1) return false;
    if (board[0][3] > anchor_rank - 2) return false;
    if (board[1][3] > anchor_rank - 2) return false;
    if (board[2][3] > anchor_rank - 2) return false;
    if (board[3][3] > anchor_rank - 2) return false;
  }
  return true;
}

template <>
bool Snake9::TupleBoard::IsGoal(bool transposed) const {
  if (transposed) {
    int anchor_rank =
        std::min(kMaxAnchorRank, std::min(board[1][2], board[0][3]) - 1);
    return board[1][3] == anchor_rank && board[2][3] == anchor_rank - 1 &&
           (board[2][2] == anchor_rank - 1 ||
            (board[3][3] == anchor_rank - 2 && board[3][2] == anchor_rank - 2));
  } else {
    int anchor_rank =
        std::min(kMaxAnchorRank, std::min(board[2][1], board[3][0]) - 1);
    return board[3][1] == anchor_rank && board[3][2] == anchor_rank - 1 &&
           (board[2][2] == anchor_rank - 1 ||
            (board[3][3] == anchor_rank - 2 && board[2][3] == anchor_rank - 2));
  }
}

#endif
