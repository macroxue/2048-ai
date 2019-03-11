#ifndef PLAN_H
#define PLAN_H

#include "board.h"
#include "compact_map.h"

class Plan {
 public:
  struct Suggestion {
    int move;
    float prob;
  };

  Suggestion SuggestMove(const Board& board) {
    if (!IsApplicable(board) || IsGoal(board)) return Suggestion{-1, 0};

    if (plan.empty()) {
      max_rank = board.MaxRank();
      auto prob = TryMoves(board, 1);
      printf("Training success rate: %3.1f%%\n", prob * 100);
      board.Show();
    }

    auto key = CompactBoard(board);
    auto entry = plan.Find(key);
    if (entry) return {entry->Move(), entry->Prob()};
    return {0, 0};
  }

  float TryMoves(const Board& board, int depth) {
    auto key = CompactBoard(board);
    auto entry = plan.Find(key);
    if (entry) return entry->Prob();

    int max_move = 0;
    float max_prob = 0;
    for (int m = 0; m < 4; ++m) {
      float prob = 0;
      Board new_board = board;
      bool moved = (new_board.*(Board::moves[m]))();
      if (moved && IsValidChange(board, new_board)) {
        if (IsGoal(new_board))
          prob = 1;
        else if (IsApplicable(new_board))
          prob = TryTiles(new_board, depth + 1);
      }
      if (max_prob < prob) {
        max_prob = prob;
        max_move = m;
      }
    }
    plan.Insert(key, max_move, max_prob);
    if (plan.size() % 1000000 == 0) {
      printf("key %012llx move %5s prob %f depth %3d size %ld\n", key,
             Board::move_names[max_move], max_prob, depth, plan.size());
    }
    return max_prob;
  }

  float TryTiles(Board& board, int depth) {
    int empty_tiles = 0;
    float tile2_prob = 0, tile4_prob = 0;
    for (int y = 0; y < N; ++y) {
      for (int x = 0; x < N; ++x) {
        if (board[x][y]) continue;
        ++empty_tiles;
        board[x][y] = 1;
        tile2_prob += TryMoves(board, depth);
        board[x][y] = 2;
        tile4_prob += TryMoves(board, depth);
        board[x][y] = 0;
      }
    }
    return (tile2_prob * 0.9 + tile4_prob * 0.1) / empty_tiles;
  }

 protected:
  virtual bool IsApplicable(const Board& board) = 0;
  virtual bool IsGoal(const Board& board) = 0;
  virtual bool IsValidChange(const Board& old_board,
                             const Board& new_board) const = 0;
  virtual unsigned long long CompactBoard(const Board& board) const = 0;

  int max_rank = 0;

 private:
  CompactMap plan;
};

class BlockPlan : public Plan {
 private:
  bool IsApplicable(const Board& board) override {
    auto anchor_rank = board[2][1];
    if (board[1][2] >= anchor_rank || board[1][3] >= anchor_rank - 1)
      return false;
    if (board[2][2] >= anchor_rank || board[2][3] >= anchor_rank - 1)
      return false;

    int rank_count[17] = {0};
    for (int y = 0; y < N; ++y)
      for (int x = 0; x < N; ++x)
        if (board[x][y] >= anchor_rank - 1) ++rank_count[board[x][y]];
    if (rank_count[max_rank + 1] != 0) return false;
    for (int rank = max_rank; rank >= anchor_rank; --rank) {
      if (rank_count[rank] != 1) return false;
    }
    return true;
  }

  bool IsGoal(const Board& board) override {
    auto anchor_rank = board[2][1] - 1;
    // x 13 12  x
    // x  9  8  x
    // x (7) 7 (7)
    // x  x (7) x
    if (board[2][2] == anchor_rank &&
        (board[1][2] == anchor_rank || board[3][2] == anchor_rank ||
         board[2][3] == anchor_rank))
      return true;
    // x 13 12 (7)
    // x  9  8  7
    // x  x  x (7)
    // x  x  x  x
    if (board[3][1] == anchor_rank &&
        (board[3][0] == anchor_rank || board[3][2] == anchor_rank))
      return true;

    return false;
  }

  bool IsValidChange(const Board& old_board,
                     const Board& new_board) const override {
    return
#if 0
        old_board[0][0] == new_board[0][0] &&
        old_board[0][1] == new_board[0][1] &&
#endif
        old_board[1][0] == new_board[1][0] &&
        old_board[1][1] == new_board[1][1] &&
        old_board[2][0] == new_board[2][0] &&
        old_board[2][1] == new_board[2][1];
  }

  unsigned long long CompactBoard(const Board& board) const override {
    return (static_cast<unsigned long long>(board[0][0]) << 44) +
           (static_cast<unsigned long long>(board[3][0]) << 40) +
           (static_cast<unsigned long long>(board[0][1]) << 36) +
           (static_cast<unsigned long long>(board[3][1]) << 32) +
           (board[0][2] << 28) + (board[1][2] << 24) + (board[2][2] << 20) +
           (board[3][2] << 16) + (board[0][3] << 12) + (board[1][3] << 8) +
           (board[2][3] << 4) + (board[3][3] << 0);
  }
};

class LinePlan : public Plan {
 private:
  bool IsApplicable(const Board& board) override {
    auto anchor_rank = board[3][0] - 1;
    if (board[0][0] <= board[1][0] || board[1][0] <= board[2][0] ||
        board[2][0] <= board[3][0])
      return false;
    if (board[1][1] == anchor_rank || board[2][1] == anchor_rank ||
        board[3][1] == anchor_rank)
      return false;
    if (board[0][1] >= anchor_rank - 4) {
      if (board[0][1] < board[1][1] || board[0][1] < board[2][1] ||
          board[0][1] < board[3][1])
        return false;
      if (board[1][1] < board[2][1] && board[2][1] > board[3][1]) return false;
    }

    for (int x = 0; x < N; ++x)
      if (board[x][1] > anchor_rank) return false;
    for (int x = 0; x < N; ++x)
      if (board[x][2] > anchor_rank - 4) return false;
    for (int x = 0; x < N; ++x)
      if (board[x][3] > anchor_rank - 5) return false;
    return true;
  }

  bool IsGoal(const Board& board) override {
    auto anchor_rank = board[3][0] - 1;
    // 14 13 12 11
    // 10  9  8  x
    //  x  x  x  x
    //  x  x  x  x
    if (board[0][1] == anchor_rank && board[1][1] == anchor_rank - 1 &&
        board[2][1] == anchor_rank - 2 && board[3][1] <= anchor_rank - 3)
      return true;

    return false;
  }

  bool IsValidChange(const Board& old_board,
                     const Board& new_board) const override {
    return old_board[0][0] == new_board[0][0] &&
           old_board[1][0] == new_board[1][0] &&
           old_board[2][0] == new_board[2][0] &&
           old_board[3][0] == new_board[3][0];
  }

  unsigned long long CompactBoard(const Board& board) const override {
    return (static_cast<unsigned long long>(board[0][1]) << 44) +
           (static_cast<unsigned long long>(board[1][1]) << 40) +
           (static_cast<unsigned long long>(board[2][1]) << 36) +
           (static_cast<unsigned long long>(board[3][1]) << 32) +
           (board[0][2] << 28) + (board[1][2] << 24) + (board[2][2] << 20) +
           (board[3][2] << 16) + (board[0][3] << 12) + (board[1][3] << 8) +
           (board[2][3] << 4) + (board[3][3] << 0);
  }
};

#endif
