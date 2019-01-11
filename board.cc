#include "board.h"

// static
const Board::Move Board::moves[] = {&Board::MoveUp, &Board::MoveLeft,
                                    &Board::MoveRight, &Board::MoveDown};
const char* Board::move_names[] = {"up", "left", "right", "down"};
unsigned int Board::move_map[1 << 20];

// static
void Board::BuildMoveMap() {
  int line[N];
  for (int i = 0; i < 1 << 20; ++i) {
    for (int j = 0; j < N; ++j) line[j] = (i >> (j * 5)) & 0x1f;

    int new_x = N - 1;
    bool merged = false;
    for (int x = N - 1; x >= 0; --x) {
      if (!line[x]) continue;
      if (!merged && new_x < N - 1 && line[x] == line[new_x + 1]) {
        line[new_x + 1] += 1;
        line[x] = 0;
        merged = true;
      } else if (new_x == x) {
        --new_x;
      } else {
        line[new_x--] = line[x];
        line[x] = 0;
        merged = false;
      }
    }

    move_map[i] = 0;
    for (int j = 0; j < N; ++j) move_map[i] += line[j] << (j * 5);
  }
}
