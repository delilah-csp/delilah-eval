#include "../delilah.h"
#include <stdint.h>

int prog (void *mem, int mem_size){
  struct delilah_file_t *file = (struct delilah_file_t *) mem;

  delilah_file_read(mem + sizeof(struct delilah_file_t), file->size, delilah_map(file->filename));

  return 0x0;
}
