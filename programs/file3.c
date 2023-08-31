#include "../delilah.h"
#include <stdint.h>

int
prog(void* a, int b, int mem, int mem_size)
{
  struct delilah_file_t* file = (struct delilah_file_t*)a;

  delilah_file_direct_read((mem + 4096) & ~0xFFF, file->size,
                    file->filename);

  return 0x0;
}
