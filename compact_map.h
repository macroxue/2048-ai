#ifndef COMPACT_MAP_H
#define COMPACT_MAP_H

#include <memory>
#include <vector>

struct Entry {
  int Move() const { return move; }
  float Prob() const { return prob * float(1.0 / 16000); }

  unsigned long long key : 48;
  unsigned long long move : 2;
  unsigned long long prob : 14;
};

class CompactMap {
 public:
  CompactMap() { Clear(); }

  void Clear() {
    num_entries = 0;
    for (int i = 0; i < kNumBuckets; ++i) buckets[i] = nullptr;
    free_holders = nullptr;
    num_free_holders = 0;
    holder_memory.clear();
  }

  const Entry* Find(unsigned long long key) const {
    auto index = Hash(key) % kNumBuckets;
    for (auto holder = buckets[index]; holder; holder = holder->next) {
      if (holder->entry.key == key) return &holder->entry;
    }
    return nullptr;
  }

  void Insert(unsigned long long key, int move, float prob) {
    auto index = Hash(key) % kNumBuckets;
    auto holder = AllocateHolder();
    holder->entry.key = key;
    holder->entry.move = move;
    holder->entry.prob = prob * float(16000.0);
    holder->next = buckets[index];
    buckets[index] = holder;
    ++num_entries;
  }

  size_t size() const { return num_entries; }
  bool empty() const { return num_entries == 0; }

 private:
  struct Holder {
    Entry entry;
    Holder* next;
  };

  Holder* AllocateHolder() {
    if (num_free_holders == 0) {
      free_holders = new Holder[kHolderChunkSize];
      holder_memory.emplace_back(free_holders);
      num_free_holders = kHolderChunkSize;
    }
    return &free_holders[--num_free_holders];
  }

  unsigned long long Hash(unsigned long long key) const {
    constexpr auto A = (1ULL << 24) - 3;
    constexpr auto B = (1ULL << 19) - 1;
    constexpr auto P = (1ULL << 31) - 19;
    return (A * key + B) % P;
  }

  static constexpr int kNumBuckets = 64 << 20;
  static constexpr int kHolderChunkSize = 8 << 20;

  size_t num_entries = 0;
  Holder* buckets[kNumBuckets];
  Holder* free_holders = nullptr;
  int num_free_holders = 0;
  std::vector<std::unique_ptr<Holder[]>> holder_memory;
};

#endif
