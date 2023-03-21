#include "../helpers.h"
#include "experiments.h"

// This experiment reads the version of Delilah from the device.
// While this is not a very useful experiment, it is a good example and a
// good way to check if the device is working.

void
experiment_version(struct io_uring* ring, int fd)
{
  struct io_uring_cqe* cqe;
  struct io_uring_params p = {};
  struct delilah_dma* dma;
  struct delilah_exec* exec;
  char *src, *dst, *prog;
  size_t sz;

  src = malloc(DATA_SIZE);
  dst = malloc(DATA_SIZE);
  prog = malloc(PROG_SIZE);

  // Write versioning program to Delilah
  sz = file_read(prog, PROG_SIZE, "programs/version.o");
  delilah_write_prog(ring, fd, 0, prog, sz);
  io_uring_wait_cqe(ring, &cqe);
  io_uring_cqe_seen(ring, cqe);

  // Execute versioning program on Delilah
  delilah_exec_prog(ring, fd, 0, 0, 0, 1, sizeof(uint64_t));
  io_uring_wait_cqe(ring, &cqe);
  io_uring_cqe_seen(ring, cqe);

  // Read versioning result from Delilah
  delilah_io(ring, fd, 0, dst, 8, 0);
  io_uring_wait_cqe(ring, &cqe);
  io_uring_cqe_seen(ring, cqe);

  printf("Delilah reported itself to be of version: %ld\n", *((uint64_t*)dst));

  free(src);
  free(dst);
  free(prog);
}