#include "array.h"

int main() {
  constexpr size_t kArraySize = 1234567;
  const char* kFile = "test_array";
  unlink(kFile);

  Array<int, kArraySize> a1;
  for (int i = 2; i <= 10; ++i) a1[kArraySize / i] = i;
  assert(a1.Save(kFile, [](const int&) { return true; }));

  Array<int, kArraySize> a2;
  assert(a2.Load(kFile));
  for (int i = 2; i <= 10; ++i) a2[kArraySize / i + 1] = i + 1;
  for (int i = 2; i <= 10; ++i) {
    assert(a2[kArraySize / i] == i);
    assert(a2[kArraySize / i + 1] == i + 1);
  }

  unlink(kFile);
  return 0;
}
