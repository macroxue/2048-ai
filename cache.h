#ifndef CASHE_H
#define CASHE_H

template <class T>
int BitSize(T v) {
  return sizeof(v) * 8;
}

class Cache {
 public:
  struct Entry {
    unsigned long long board;
    float prob;
    int depth : 4;
    int score : 28;
  };

  Cache() { Clear(); }

  void ShowStats() const {
    int loaded_count = 0;
    for (int i = 0; i < kSize; ++i) loaded_count += (entries[i].board != 0);

    printf("lookups: %10lld         hits: %10lld (%4.1f%%)\n", lookup_count,
           hit_count, hit_count * 100.0 / lookup_count);
    printf("updates: %10lld   collisions: %10lld (%4.1f%%)\n", update_count,
           collision_count, collision_count * 100.0 / update_count);
    printf("entries: %10d       loaded: %10d (%4.1f%%)\n", kSize,
           loaded_count, loaded_count * 100.0 / kSize);
  }

  void Clear() {
    for (int i = 0; i < kSize; ++i) entries[i].board = 0;
  }

  bool Lookup(unsigned long long board, float prob, int depth,
              int* score) const {
    ++lookup_count;
    unsigned long long hash = Hash(board);
    unsigned long long index = hash & (kSize - 1);
    const Entry& entry = entries[index];
    if (entry.board == board && entry.prob >= prob && entry.depth >= depth) {
      ++hit_count;
      *score = entry.score;
      return true;
    }
    return false;
  }

  void Update(unsigned long long board, float prob, int depth, int score) {
    ++update_count;
    unsigned long long hash = Hash(board);
    unsigned long long index = hash & (kSize - 1);
    Entry& entry = entries[index];
    collision_count += entry.board != 0;
    entry.board = board;
    entry.prob = prob;
    entry.depth = depth;
    entry.score = score;
  }

 private:
  unsigned long long Hash(unsigned long long board) const {
    return board + (board >> kBits) + (board >> (kBits * 2));
  }

  static constexpr int kBits = 22;
  static constexpr int kSize = 1 << kBits;
  Entry entries[kSize];

  mutable long long lookup_count = 0;
  mutable long long hit_count = 0;
  mutable long long update_count = 0;
  mutable long long collision_count = 0;
};

#endif
