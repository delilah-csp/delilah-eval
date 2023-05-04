#include "delilah.h"
#include "experiments/experiments.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int
main()
{
  struct io_uring ring;
  struct io_uring_params p = {};

  int fd, ret;

  /* Open Delilah device */
  fd = open("/dev/delilah0", O_RDWR);
  if (fd < 0) {
    perror("Cannot open Delilah device");
    return 3;
  }

  /* Initialize io_uring queue with four entries */
  p.flags = IORING_SETUP_SQE128;
  ret = io_uring_queue_init(1024, &ring, p.flags);

  if (ret) {
    fprintf(stderr, "Cannot init queue\n");
    return 2;
  }

  experiment_version(&ring, fd);
  printf("\n");

  printf("Calling Bench on Delilah. No output on Host.\n\n");
  experiment_bench(&ring, fd);

  printf("Reading files:\n");
  experiment_read_file(&ring, fd);

  printf("Reading files with strategic cache coherency:\n");
  experiment_read_file_cache(&ring, fd);

  printf("CMB latency/troughput:\n");
  experiment_cmb(&ring, fd);

  printf("DMA latency/troughput:\n");
  experiment_dma_1(&ring, fd);

  printf("Multi DMA (4) latency/troughput:\n");
  experiment_dma_4(&ring, fd);

  printf("TUD Filtering (BW, JIT):\n");
  experiment_tud_filter_bw_jit(&ring, fd);

  printf("TUD Filtering (EQ, JIT):\n");
  experiment_tud_filter_eq_jit(&ring, fd);

  io_uring_queue_exit(&ring);
  close(fd);

  return 0;
}
