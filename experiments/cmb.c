#include "../helpers.h"
#include "experiments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

// In this experiment we transfer data to and from the CMB. We do this by
// mapping the CMB into our address space, and then using memcpy to transfer
// data to and from it. We do this to compare the CMB speed to the DMA speed.
// Note that we cannot do the experiment for 100 MB buffers, since we are
// borrowing BAR0 for the CMB experiment, and only ~31 MB is free.
// Furthermore, there are alignment restrictions on the CMB, so we cannot
// write any arbitrary data size.

void
experiment_cmb(struct io_uring* ring, int fd)
{
  char *src, *dst;
  src = malloc(DATA_SIZE);
  dst = malloc(DATA_SIZE);

  uint64_t sizes[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };

  ft_table_t* table = ft_create_table();
  /* Set "header" type for the first row */
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);

  ft_write_ln(table, "Buffer Size", "Read", "Read Throughput", "Write",
              "Write Throughput");

  int fd_cmb =
    open("/sys/bus/pci/devices/0000:01:00.0/resource0", O_RDWR | O_SYNC);
  void* ptr = mmap(NULL, 31 * 1024 * 1024, PROT_READ | PROT_WRITE, MAP_SHARED,
                   fd_cmb, 1024 * 1024);

  for (int i = 0; i < 8; i++) {
    struct timeval read = clock_start();
    memcpy(dst, ptr, sizes[i]);
    double read_time = clock_end(read);

    struct timeval write = clock_start();
    memcpy(ptr, src, sizes[i]);
    double write_time = clock_end(write);

    char buffer_size[32], write_time_str[16], read_time_str[16],
      read_throughput_str[32], write_throughput_str[32];

    // Calculate throughput. Time is in ms, size is in bytes.
    double read_throughput = sizes[i] / (read_time) / 1000000;
    double write_throughput = sizes[i] / (write_time) / 1000000;

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

  munmap(ptr, 31 * 1024 * 1024);
  close(fd_cmb);

  printf("%s\n", ft_to_string(table));
  ft_destroy_table(table);
}