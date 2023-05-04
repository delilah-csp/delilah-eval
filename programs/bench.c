#include "../delilah.h"
#include <stdint.h>

int
prog(void* mem, int mem_size, void* shared, int shared_size)
{
  delilah_debug_bench(mem, mem_size, shared, shared_size);
  return 0x0;
}
