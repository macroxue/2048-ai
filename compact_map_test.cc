#include "compact_map.h"
#include "assert.h"

CompactMap map;

int main() {
  const int kTotalEntries = 100 << 20;
  for (unsigned key = 0; key < kTotalEntries; ++key) {
    auto entry = map.Find(key);
    assert(!entry);
    map.Insert(key, 0, 1);
  }
  assert(map.size() == kTotalEntries);
  for (unsigned key = 0; key < kTotalEntries; ++key) {
    auto entry = map.Find(key);
    assert(entry);
  }
}
