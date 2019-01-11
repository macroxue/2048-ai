#include <assert.h>

#include "board.h"

void TestMoves() {
  int layout[N][N] = {
    { 0, 0, 0, 1 },
    { 0, 2, 0, 2 },
    { 2, 2, 2, 2 },
    { 3, 1, 1, 1 },
  };
  int moved_left[N][N] = {
    { 1, 0, 0, 0 },
    { 3, 0, 0, 0 },
    { 3, 3, 0, 0 },
    { 3, 2, 1, 0 },
  };
  int moved_right[N][N] = {
    { 0, 0, 0, 1 },
    { 0, 0, 0, 3 },
    { 0, 0, 3, 3 },
    { 0, 3, 1, 2 },
  };
  int moved_up[N][N] = {
    { 2, 3, 2, 1 },
    { 3, 1, 1, 3 },
    { 0, 0, 0, 1 },
    { 0, 0, 0, 0 },
  };
  int moved_down[N][N] = {
    { 0, 0, 0, 0 },
    { 0, 0, 0, 1 },
    { 2, 3, 2, 3 },
    { 3, 1, 1, 1 },
  };
  Board left(layout);
  assert(left.MoveLeft());
  assert(left == Board(moved_left));

  Board right(layout);
  assert(right.MoveRight());
  assert(right == Board(moved_right));

  Board up(layout);
  assert(up.MoveUp());
  assert(up == Board(moved_up));

  Board down(layout);
  assert(down.MoveDown());
  assert(down == Board(moved_down));

  int layout2[N][N] = {
    { 0, 0, 0, 1 },
    { 0, 0, 3, 2 },
    { 2, 3, 4, 5 },
    { 3, 4, 5, 6 },
  };
  Board down2(layout2);
  assert(!down2.MoveDown());
  assert(down2 == Board(layout2));

  Board right2(layout2);
  assert(!right2.MoveRight());
  assert(right2 == Board(layout2));

  Board up2(layout2);
  assert(up2.MoveUp());

  Board left2(layout2);
  assert(left2.MoveLeft());

  int layout3[N][N] = {
    { 3, 4, 5, 6 },
    { 2, 3, 4, 5 },
    { 1, 2, 0, 0 },
    { 3, 0, 0, 0 },
  };
  Board up3(layout3);
  assert(!up3.MoveUp());
  assert(up3 == Board(layout3));

  Board left3(layout3);
  assert(!left3.MoveLeft());
  assert(left3 == Board(layout3));

  Board right3(layout3);
  assert(right3.MoveRight());

  Board down3(layout3);
  assert(down3.MoveDown());
}

int main() {
  Board::BuildMoveMap();
  TestMoves();
  return 0;
}
