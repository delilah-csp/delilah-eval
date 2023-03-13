#include <stdint.h>

struct delilah_file_t {
  uint64_t size;
  char filename[128];
};

struct delilah_mount_t {
  char nvme_handle[32];
  char mount_point[128];
  char fs_type[32];
};
