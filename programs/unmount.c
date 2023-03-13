#include "../delilah.h"
#include <stdint.h>

int prog(void *mem, int mem_size) {
  struct delilah_mount_t *fs = (struct delilah_mount_t *)mem;

  return delilah_fs_unmount(&fs->mount_point);
}
