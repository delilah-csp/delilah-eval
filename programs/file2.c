#include "../delilah.h"
#include <stdint.h>

int
prog(void* mem, int mem_size)
{
  struct delilah_file_t* file = (struct delilah_file_t*)mem;

  delilah_file_indirect_read(mem, file->size,
                    file->filename, 1);

  return 0;
}
