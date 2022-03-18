#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#include <functional>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "node.h"
#include "plan.h"
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
      printf("%5s %.3f", Board::move_names[move], prob);
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

std::unique_ptr<Tuple10> tuple10;
std::unique_ptr<Tuple11> tuple11;
std::unique_ptr<BlockPlan> block_plan;
std::unique_ptr<LinePlan> line_plan;

struct Valuation {
  float prob(int max_rank) const {
    if (type == kTuple10) return max_rank <= 14 ? value / 0.1166666 : value;
    else if (type == kTuple11) return value;
    else return value / 1000;
  }

  void Show(int max_rank) const {
    if (type == kSearch)
      printf("from search, h-score %.3f\n", prob(max_rank));
    else if (type == kTuple10)
      printf("from lookup-10, prob %.3f\n", prob(max_rank));
    else
      printf("from lookup-11, prob %.3f\n", prob(max_rank));
  }

  enum Type {kInvalid, kTuple11, kTuple10, kLinePlan, kBlockPlan, kSearch};
  Type type;
  float value;
};

Valuation SuggestMove(Node& n, int* m) {
  float prob = 0;
  if (prob == 0 && tuple11) {
    prob = tuple11->SuggestMove(n, m);
    if (prob < 0.9) prob = 0;
    else return {Valuation::kTuple11, prob};
  }
  if (prob == 0 && tuple10) {
    prob = tuple10->SuggestMove(n, m);
    if (prob > 0) return {Valuation::kTuple10, prob};
  }
  if (prob == 0 && line_plan) {
    auto suggestion = line_plan->SuggestMove(n);
    *m = suggestion.move;
    prob = suggestion.prob;
    if (prob > 0) return {Valuation::kLinePlan, prob};
  }
  if (prob == 0 && block_plan) {
    auto suggestion = block_plan->SuggestMove(n);
    *m = suggestion.move;
    prob = suggestion.prob;
    if (prob > 0) return {Valuation::kBlockPlan, prob};
  }
  return {Valuation::kSearch, (float)n.Search(options.max_depth, m)};
}

float LookupTuple(Node &n, Valuation::Type type) {
  int ignore;
  if (type == Valuation::kTuple11)
    return tuple11->SuggestMove(n, &ignore);
  else
    return tuple10->SuggestMove(n, &ignore);
}

float RollOutWithTuple(Node &n, Valuation::Type type) {
  int empty_tiles = n.CountEmptyTiles();
  float tile2_prob = 1.0 / empty_tiles * 0.9;
  float tile4_prob = 1.0 / empty_tiles * 0.1;
  float total_prob = 0;
  for (int y = 0; y < N; ++y) {
    for (int x = 0; x < N; ++x) {
      if (n[x][y]) continue;

      n[x][y] = 1;
      total_prob += LookupTuple(n, type) * tile2_prob;
      n[x][y] = 2;
      total_prob += LookupTuple(n, type) * tile4_prob;
      n[x][y] = 0;
    }
  }
  return total_prob;
}

void AnalyzeMove(Node& n, int move) {
  int best_move;
  auto best_valuation = SuggestMove(n, &best_move);
  if (move == best_move) return;

  Node n1 = n;
  if (!(n1.*Node::moves[move])()) return;

  Valuation valuation = {best_valuation.type, 0};
  if (best_valuation.type == Valuation::kTuple11 ||
      best_valuation.type == Valuation::kTuple10) {
    valuation.value = RollOutWithTuple(n1, best_valuation.type);
  } else {
    valuation.value = n1.TryAllTiles(options.max_depth - 1, 1);
  }
  bool suboptimal = (best_valuation.value >= 0)
    ? (valuation.value < options.optimality * best_valuation.value)
    : (valuation.value < 1 / options.optimality * best_valuation.value);
  if (suboptimal) {
    n.Show();
    printf("***** %s %.3f < %s %.3f *****\n",
           Board::move_names[move], valuation.prob(n.MaxRank()),
           Board::move_names[best_move], best_valuation.prob(n.MaxRank()));
  }
}

int CharToRank(char ch) {
  if ('0' <= ch && ch <= '9') return ch - '0';
  if ('A' <= ch && ch <= 'G') return ch - 'A' + 10;
  if ('a' <= ch && ch <= 'g') return ch - 'a' + 10;
  return 0;
}

void RunAgent(int client_socket) {
  printf("Accepted client socket %d\n", client_socket);
  char buffer[1 << 16] = {0};
  while (true) {
    buffer[0] = '\0';
    if (read(client_socket, buffer, sizeof(buffer)) < 0) break;
    buffer[sizeof(buffer) - 1] = '\0';

    char* get = strstr(buffer, "GET /move?board=");
    char* post = strstr(buffer, "POST /analyze?board=");
    if (!get && !post) break;
    char* board = get ? strchr(get, '=') + 1 : strchr(post, '=') + 1;
    if (strlen(board) < N * N) break;

    int layout[N][N] = {0};
    for (int y = 0; y < N; ++y)
      for (int x = 0; x < N; ++x) layout[y][x] = CharToRank(*board++);

    Node n(layout);
    if (post) {
      char* direction = strchr(board, '=') + 1;
      int m = -1;
      switch (*direction) {
        case 'u': m = 0; break;
        case 'l': m = 1; break;
        case 'r': m = 2; break;
        case 'd': m = 3; break;
      }
      if (m == -1) break;

      AnalyzeMove(n, m);
      std::string reply(
                        "HTTP/1.1 200 OK\n"
                        "Access-Control-Allow-Origin: *\n"
                        "Content-Type: text/plain\n"
                        "Content-Length: 0\n"
                        "\n");

      if (write(client_socket, reply.c_str(), reply.size()) < 0) break;
    } else {
      int m;
      auto valuation = SuggestMove(n, &m);
      if (options.verbose) {
        n.Show();
        if (m >= 0) {
          printf("%5s ", Board::move_names[m]);
          valuation.Show(n.MaxRank());
        }
      }

      std::string reply(
                        "HTTP/1.1 200 OK\n"
                        "Access-Control-Allow-Origin: *\n"
                        "Content-Type: text/plain\n"
                        "Content-Length: 1\n"
                        "\n");
      reply += m >= 0 ? Board::move_names[m][0] : 'g';

      if (write(client_socket, reply.c_str(), reply.size()) < 0) break;
    }
  }
  close(client_socket);
  printf("Closed client socket %d\n", client_socket);
}

void RunServer(int server_port) {
  // Creating socket file descriptor
  int server_socket;
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("In socket");
    exit(EXIT_FAILURE);
  }
  int enable = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable,
                 sizeof(enable)) < 0) {
    perror("In setsockopt");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(server_port);

  memset(address.sin_zero, '\0', sizeof address.sin_zero);

  if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
    perror("In bind");
    exit(EXIT_FAILURE);
  }
  if (listen(server_socket, 10) < 0) {
    perror("In listen");
    exit(EXIT_FAILURE);
  }

  puts("Server ready");
  while (true) {
    const int addrlen = sizeof(address);
    int client_socket;
    if ((client_socket = accept(server_socket, (struct sockaddr*)&address,
                                (socklen_t*)&addrlen)) < 0) {
      perror("In accept");
      exit(EXIT_FAILURE);
    }
    new std::thread(std::bind(RunAgent, client_socket));
  }

  close(server_socket);
}

int main(int argc, char* argv[]) {
  options.seed = time(nullptr);
  options.UpdateMinProbFromDepth();
  int server_port = 0;
  int c;
  while ((c = getopt(argc, argv, "d:i:p:vIO:P:R:S:T")) != -1) {
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
      case 'O':
        options.optimality = atof(optarg);
        break;
      case 'P':
        options.prefill_rank = atoi(optarg);
        break;
      case 'R':
        options.max_rank = atoi(optarg);
        break;
      case 'S':
        server_port = atoi(optarg);
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
  if (options.tuple_moves) {
    tuple10.reset(new Tuple10);
    tuple11.reset(new Tuple11);
  } else {
    block_plan.reset(new BlockPlan);
    // line_plan.reset(new LinePlan);
  }

  if (server_port) {
    RunServer(server_port);
    return 0;
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
    int layout[N][N] = {{14, 13, 12, 11},  // row 0
                        {1, 0, 0, 0},      // row 1
                        {0, 0, 0, 0},      // row 2
                        {0, 0, 0, 0}};     // row 3
    Node n(layout);
#endif

    timeval start, finish;
    gettimeofday(&start, NULL);

    int num_moves = 0;
    int prev_max_rank = 0;
    do {
      if (options.verbose || options.interactive) n.Show();

      int max_rank = n.MaxRank();
      if (max_rank == options.max_rank) break;

      int m;
      auto valuation = SuggestMove(n, &m);
      auto prob = valuation.prob(max_rank);

      if (options.interactive) num_moves -= InteractivePlay(&n, &m, prob);

      if (m < 0) break;

      bool moved = (n.*Node::moves[m])();
      assert(moved);
      ++num_moves;

      if (options.interactive) {
        printf("#%d: %s\n", num_moves, Board::move_names[m]);
      } else if (options.verbose) {
        printf("#%d: %5s ", num_moves, Board::move_names[m]);
        valuation.Show(max_rank);
      } else if (max_rank != prev_max_rank) {
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
