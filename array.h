#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <functional>
#include <memory>

template <class T, size_t kArraySize>
class Array {
  public:
    Array(int memset_value = 0) : memset_value(memset_value) {
      memset(chunks, 0, sizeof(chunks));
    }

    T* locate(size_t i) const {
      auto chunk_index = i / kChunkSize;
      return chunks[chunk_index] ? &chunks[chunk_index][i % kChunkSize] : nullptr;
    }

    T& operator[](size_t i) {
      auto chunk_index = i / kChunkSize;
      auto chunk = chunks[chunk_index];
      if (chunk) {
        if (mapped_data <= chunk &&
            chunk < mapped_data + header.num_used_chunks * kChunkSize) {
          // Copy on write.
          chunks[chunk_index] = new T[kChunkSize];
          memcpy(chunks[chunk_index], chunk, kChunkSize * sizeof(T));
        }
      } else {
        chunks[chunk_index] = new T[kChunkSize];
        memset(chunks[chunk_index], memset_value, kChunkSize * sizeof(T));
      }
      return chunks[chunk_index][i % kChunkSize];
    }

    const T& operator[](size_t i) const {
      auto chunk_index = i / kChunkSize;
      return chunks[chunk_index][i % kChunkSize];
    }

    bool Load(const char* file) {
      printf("Loading array from %s ... ", file);
      fflush(stdout);
      auto fd = open(file, O_RDONLY);
      if (fd == -1) {
        perror("");
        return false;
      }

      // Read the header and see if it's correct.
      auto header_size = read(fd, &header, sizeof(header));
      if (header_size == -1 || header_size != sizeof(header)) {
        perror("Failed to read the file header");
        exit(1);
      }
      if (header.element_size != sizeof(T) || header.array_size != kArraySize ||
          header.num_chunks != kNumChunks || header.num_used_chunks > kNumChunks) {
        fprintf(stderr, "The file header is corrupted.\n");
        exit(1);
      }

      // Map a bitmap indicating what chunks are used.
      auto chunk_bitmap = (uint8_t*) mmap(nullptr, kChunkBitmapBytes,
                                          PROT_READ, MAP_PRIVATE, fd, header_size);
      if (!chunk_bitmap) {
        perror("Failed to mmap the bitmap of used chunks");
        exit(1);
      }

      // Map data in the used chunks.
      mapped_data = (T*) mmap(nullptr, header.num_used_chunks * kChunkSize * sizeof(T),
                              PROT_READ, MAP_PRIVATE, fd, header_size + kChunkBitmapBytes);
      if (!mapped_data) {
        perror("Failed to mmap data of used chunks");
        exit(1);
      }

      // Setup the pointers of used chunks.
      size_t num_used_chunks = 0;
      for (size_t i = 0; i < kNumChunks; ++i) {
        if ((chunk_bitmap[i / 8] & (1 << (i % 8))) == 0) continue;
        chunks[i] = &mapped_data[num_used_chunks * kChunkSize];
        ++num_used_chunks;
      }
      assert(num_used_chunks == header.num_used_chunks);

      if (munmap(chunk_bitmap, kChunkBitmapBytes) == -1) {
        perror("Fail to munmap the bitmap of used chunks");
        exit(1);
      }

      printf("done\n");
      return true;
    }

    bool Save(const char* file, std::function<bool(const T&)> is_valid,
              double threshold = 0) {
      auto fd = open(file, O_CREAT|O_EXCL|O_WRONLY, S_IRUSR|S_IWUSR);
      if (fd == -1) {
        if (errno != EEXIST) perror("Failed to create file");
        return false;
      }
      printf("Saving array to %s ... ", file);
      fflush(stdout);

      // Prepare the header and chunk bitmap.
      header.element_size = sizeof(T);
      header.array_size = kArraySize;
      header.num_chunks = kNumChunks;
      header.num_used_chunks = 0;
      std::unique_ptr<uint8_t[]> chunk_bitmap(new uint8_t[kChunkBitmapBytes]);
      memset(chunk_bitmap.get(), 0, kChunkBitmapBytes);
      for (size_t i = 0; i < kNumChunks; ++i) {
        if (!chunks[i]) continue;

        auto num_valid = std::count_if(chunks[i], chunks[i] + kChunkSize, is_valid);
        if ((double)num_valid / kChunkSize >= threshold) {
          ++header.num_used_chunks;
          chunk_bitmap[i / 8] |= 1 << (i % 8);
        }
      }

      // Write the header.
      auto header_size = write(fd, &header, sizeof(header));
      if (header_size == -1) {
        perror("Failed to write the header");
        return false;
      }

      // Write the bitmap of used chunks.
      auto bitmap_size = write(fd, chunk_bitmap.get(), kChunkBitmapBytes);
      if (bitmap_size == -1) {
        perror("Failed to write the bitmap of used chunks");
        return false;
      }

      // Write the data of used chunks.
      for (size_t i = 0; i < kNumChunks; ++i) {
        if (!chunks[i]) continue;
        if ((chunk_bitmap[i / 8] & (1 << (i % 8))) == 0) continue;
        auto data_size = write(fd, chunks[i], kChunkSize * sizeof(T));
        if (data_size == -1) {
          perror("Failed to write used chunks");
          return false;
        }
      }
      close(fd);
      printf("done\n");
      return true;
    }

  private:
    static constexpr size_t kPageSize = 4096;
    static constexpr size_t kChunkSize = 512;
    static constexpr size_t kNumChunks = (kArraySize + kChunkSize - 1) / kChunkSize;
    static constexpr size_t kChunkBitmapBytes =
      ((kNumChunks + 7) / 8 + kPageSize - 1) / kPageSize * kPageSize;

    struct Header {
      size_t element_size = 0;
      size_t array_size = 0;
      size_t num_chunks = 0;
      size_t num_used_chunks = 0;
      char pad[kPageSize - sizeof(size_t) * 4] = {'\0'};
    };
    static_assert(sizeof(Header) == kPageSize, "Wrong header size");

    const int memset_value;

    Header header;
    T* chunks[kNumChunks];
    T* mapped_data;
};

#endif
