#include <stdint.h>

struct delilah_file_t {
  uint64_t size;
  char filename[128];
};
