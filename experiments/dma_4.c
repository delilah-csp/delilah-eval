#include "../helpers.h"
#include "experiments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// In this experiment we DMA to four buffers an back, and measure the time it
// takes. However, we do this in parallel, using 4 concurrent DMA operations.

void
experiment_dma_4(struct io_uring* ring, int fd)
{
  struct io_uring_cqe* cqe;
  struct delilah_dma* dma;
  char *src, *dst;

  src = malloc(DATA_SIZE);
  dst = malloc(DATA_SIZE);

  uint64_t sizes[] = { 1000,     10000,
                       100000, 1000000, 10000000, 100000000 };

  ft_table_t* table = ft_create_table();
  /* Set "header" type for the first row */
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);

  #ifdef DELILAH_TABLE_CSV
  table_csv(table);
  #endif

  ft_write_ln(table, "Buffer Size", "Read", "Read Throughput", "Write",
              "Write Throughput");

  for (int i = 0; i < 6; i++) {
    struct timeval write = clock_start();
    for (int j = 0; j < 4; j++) {
      int ret = delilah_io(ring, fd, j, src, sizes[i], 1);

      if (ret != 1) {
        printf("Experiment failed (SUBMIT, WRITE)! Error: %s\n", strerror(ret));
        return;
      }
    }
    for (int j = 0; j < 4; j++) {
      io_uring_wait_cqe(ring, &cqe);
      io_uring_cqe_seen(ring, cqe);

      if (cqe->res < 0) {
        printf("Experiment failed (COMP, WRITE)! Error: %s\n",
               strerror(cqe->res));
        return;
      }
    }
    double write_time = clock_end(write);

    struct timeval read = clock_start();
    for (int j = 0; j < 4; j++) {
      int ret = delilah_io(ring, fd, j, dst, sizes[i], 0);

      if (ret != 1) {
        printf("Experiment failed (SUBMIT, READ)! Error: %s\n", strerror(ret));
        return;
      }
    }
    for (int j = 0; j < 4; j++) {
      io_uring_wait_cqe(ring, &cqe);
      io_uring_cqe_seen(ring, cqe);

      if (cqe->res < 0) {
        printf("Experiment failed (COMP, READ)! Error: %s\n",
               strerror(cqe->res));
        return;
      }
    }
    double read_time = clock_end(read);

    char buffer_size[32], write_time_str[16], read_time_str[16],
      read_throughput_str[32], write_throughput_str[32];

    // Calculate throughput. Time is in ms, size is in bytes.
    double read_throughput = (4 * sizes[i]) / (read_time) / 1000000;
    double write_throughput = (4 * sizes[i]) / (write_time) / 1000000;

    add_units(buffer_size, sizes[i]);
    sprintf(write_time_str, "%.3f ms", write_time * 1000);
    sprintf(read_time_str, "%.3f ms", read_time * 1000);
    sprintf(read_throughput_str, "%.3lf MB/s", read_throughput);
    sprintf(write_throughput_str, "%.3lf MB/s", write_throughput);

    ft_write_ln(table, buffer_size, read_time_str, read_throughput_str,
                write_time_str, write_throughput_str);
  }

  free(src);
  free(dst);

  printf("%s\n", ft_to_string(table));
  ft_destroy_table(table);
}
