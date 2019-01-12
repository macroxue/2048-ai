#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#include <vector>

#include "node.h"
#include "tuple.h"

struct Snapshot {
  Node node;
  int move;
  float prob;
};

double Elapse(const timeval& from, const timeval& to) {
  return (to.tv_sec - from.tv_sec) + (double(to.tv_usec) - from.tv_usec) * 1e-6;
}

char GetKey() {
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0) perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
  return (buf);
}

int InteractivePlay(Node* n, int* move, float prob) {
  auto show_move = [](int move, float prob) {
    if (move >= 0)
      printf("p(%s)=%f", Node::move_names[move], prob);
    else
      printf("deadend");
    printf(
        ", Space=accept, A|H=left, S|J=down, W|K=up, D|L=right, U=undo, "
        "Q=quit\n");
  };

  static std::vector<Snapshot> history;
  history.push_back({*n, *move, prob});
  show_move(*move, prob);

  Node next = *n;
  int num_undos = 0;
  bool moved = false, quit = false;
  do {
    switch (toupper(GetKey())) {
      case ' ':
        *move = history.back().move;
        moved = true;
        break;
      case 'W':
      case 'K':
        *move = 0;
        moved = (next.*Node::moves[0])();
        break;
      case 'A':
      case 'H':
        *move = 1;
        moved = (next.*Node::moves[1])();
        break;
      case 'D':
      case 'L':
        *move = 2;
        moved = (next.*Node::moves[2])();
        break;
      case 'S':
      case 'J':
        *move = 3;
        moved = (next.*Node::moves[3])();
        break;
      case 'U':
        if (history.size() > 1) {
          ++num_undos;
          history.pop_back();
          *n = history.back().node;
          *move = history.back().move;
          prob = history.back().prob;
          n->Show();
          show_move(*move, prob);
          next = *n;
        }
        break;
      case 'Q':
        quit = true;
        *move = -1;
        break;
    }
  } while (!moved && !quit);
  return num_undos;
}

int main(int argc, char* argv[]) {
  options.seed = time(nullptr);
  options.UpdateMinProbFromDepth();
  int c;
  while ((c = getopt(argc, argv, "d:i:p:vIP:R:T")) != -1) {
    switch (c) {
      case 'd':
        options.max_depth = atoi(optarg);
        options.UpdateMinProbFromDepth();
        break;
      case 'i':
        options.iterations = atoi(optarg);
        break;
      case 'p':
        options.min_prob = atof(optarg);
        break;
      case 'v':
        options.verbose = true;
        break;
      case 'I':
        options.interactive = true;
        break;
      case 'P':
        options.prefill_rank = atoi(optarg);
        break;
      case 'R':
        options.max_rank = atoi(optarg);
        break;
      case 'T':
        options.tuple_moves = false;
        break;
    }
  }
  if (optind < argc) options.seed = atoi(argv[optind]);
  srand(options.seed);

  Node::BuildMoveMap();
  Node::BuildScoreMap();

  std::unique_ptr<Tuple10> tuple10;
  std::unique_ptr<Tuple11> tuple11;
  if (options.tuple_moves) {
    tuple10.reset(new Tuple10);
    tuple11.reset(new Tuple11);
  }

  long sum_game_scores = 0;
  int max_rank_freq[N * N + 1];
  memset(max_rank_freq, 0, sizeof(max_rank_freq));

  for (int i = 0; i < options.iterations; ++i) {
    srand(options.seed + i);
    Node::cache.Clear();
#if 1
    Node n;
    if (options.prefill_rank) {
      n.Prefill(options.prefill_rank);
    } else {
      n.GenerateRandomTile();
      n.GenerateRandomTile();
    }
#else
    int layout[N][N] = {{1, 7, 3, 0},     // row 0
                        {9, 7, 2, 1},     // row 1
                        {11, 10, 1, 1},   // row 2
                        {13, 12, 0, 0}};  // row 3
    Node n(layout);
#endif

    timeval start, finish;
    gettimeofday(&start, NULL);

    int num_moves = 0;
    int prev_max_rank = 0;
    do {
      if (options.verbose || options.interactive) n.Show();

      int m;
      float prob = 0;
      if (tuple11) {
        prob = tuple11->SuggestMove(n, &m);
        if (prob < 0.9) prob = 0;
      }
      if (prob == 0 && tuple10) {
        prob = tuple10->SuggestMove(n, &m);
      }
      if (prob == 0) n.Search(options.max_depth, &m);

      if (options.interactive) num_moves -= InteractivePlay(&n, &m, prob);

      if (m < 0) break;

      bool moved = (n.*Node::moves[m])();
      assert(moved);
      ++num_moves;

      int max_rank = n.MaxRank();
      if (max_rank == options.max_rank) break;

      if (options.interactive)
        printf("#%d: %s\n", num_moves, Node::move_names[m]);
      else if (options.verbose)
        printf("#%d: %s from %s\n", num_moves, Node::move_names[m],
               prob > 0 ? "lookup" : "search");
      else if (max_rank != prev_max_rank) {
        printf("\r%7d", Node::Tile(max_rank));
        fflush(stdout);
      }
      prev_max_rank = max_rank;
    } while (n.GenerateRandomTile());

    gettimeofday(&finish, NULL);

    if (!options.verbose) {
      putchar('\r');
      n.Show();
    }
    auto seconds = Elapse(start, finish);
    printf("game# %d moves %d seconds %.1f moves/s %.1f\n", options.seed + i,
           num_moves, seconds, num_moves / seconds);

    sum_game_scores += n.GameScore();
    ++max_rank_freq[n.MaxRank()];
    if (options.iterations > 1) {
      printf("%d-game average %ld max ", i + 1, sum_game_scores / (i + 1));
      for (int r = 0; r <= N * N; ++r)
        if (max_rank_freq[r])
          printf("%d*%d(%.1f%%) ", Node::Tile(r), max_rank_freq[r],
                 100.0 * max_rank_freq[r] / (i + 1));
      puts("");
    }
    if (options.verbose) Node::cache.ShowStats();
    fflush(stdout);
  }
  return 0;
}
