#include "../helpers.h"
#include "experiments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
experiment_bench(struct io_uring* ring, int fd)
{
  struct io_uring_cqe* cqe;
  struct delilah_dma* dma;
  struct delilah_exec* exec;
  char *prog;
  size_t sz;

  prog = malloc(PROG_SIZE);
  sz = file_read(prog, PROG_SIZE, "programs/bench.o");

  delilah_write_prog(ring, fd, 0, prog, sz);
  io_uring_wait_cqe(ring, &cqe);
  io_uring_cqe_seen(ring, cqe);

  delilah_exec_prog(ring, fd, 0, 0, 0, 0, 0);
  io_uring_wait_cqe(ring, &cqe);
  io_uring_cqe_seen(ring, cqe);
  
  free(prog);
}