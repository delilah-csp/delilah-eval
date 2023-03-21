#include <stdint.h>

int
prog(void* mem, int mem_size)
{
  uint64_t* version = (uint64_t*)mem;
  *version = delilah_version_major() * 10000 + delilah_version_minor() * 100 +
             delilah_version_build();

  return 0;
}
