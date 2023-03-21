#include "../helpers.h"
#include "experiments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// In this experiment we read a file from an NVMe device and read it to a buffer
// on the host. We experiment with different file sizes. We measure the time it
// takes to load the program, write state to the device, execute the program,
// and read the result. In this particular experiment, we only invalidate and
// flush the used parts of the cache.

void
experiment_read_file_cache(struct io_uring* ring, int fd)
{
  struct io_uring_cqe* cqe;
  struct delilah_dma* dma;
  struct delilah_exec* exec;
  char *src, *dst, *prog;
  size_t sz;

  src = malloc(DATA_SIZE);
  dst = malloc(DATA_SIZE);
  prog = malloc(PROG_SIZE);

  uint64_t sizes[] = { 100, 1000, 10000, 100000, 1000000, 10000000 };
  struct delilah_file_t* file = (struct delilah_file_t*)src;

  sz = file_read(prog, PROG_SIZE, "programs/file.o");

  ft_table_t* table = ft_create_table();
  /* Set "header" type for the first row */
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);

  ft_write_ln(table, "File Size", "Write Prog", "Write State", "Exec Prog",
              "Read Result", "Total Time", "V1", "V2");

  for (int i = 0; i < 6; i++) {
    file->size = sizes[i] * 10; // the size denotes the number of strings
    sprintf(file->filename, "/run/media/nvme0n1p1/%ld.txt", sizes[i]);

    struct timeval loading = clock_start();
    delilah_write_prog(ring, fd, 0, prog, sz);
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double load_time = clock_end(loading) * 1000;

    struct timeval write = clock_start();
    delilah_io(ring, fd, 0, src, sizeof(struct delilah_file_t), 1);
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double write_time = clock_end(write) * 1000;

    struct timeval exec = clock_start();
    delilah_exec_prog(ring, fd, 0, 0, 0, sizeof(struct delilah_file_t),
                      sizeof(struct delilah_file_t) + sizes[i] * 10);
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double exec_time = clock_end(exec) * 1000;

    struct timeval read = clock_start();
    delilah_io(ring, fd, 0, dst, sizeof(struct delilah_file_t) + sizes[i] * 10,
               0);
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double read_time = clock_end(read) * 1000;

    char first[10], last[10];
    first[9] = '\0';
    last[9] = '\0';

    memcpy(first, dst + sizeof(struct delilah_file_t), 9);
    memcpy(last, dst + sizeof(struct delilah_file_t) + (sizes[i] * 10) - 10, 9);

    char filesize[32], load_time_str[16], write_time_str[16], exec_time_str[16],
      read_time_str[16], total_time_str[16];

    add_units(filesize, sizes[i] * 10);
    sprintf(load_time_str, "%.3f ms", load_time);
    sprintf(write_time_str, "%.3f ms", write_time);
    sprintf(exec_time_str, "%.3f ms", exec_time);
    sprintf(read_time_str, "%.3f ms", read_time);
    sprintf(total_time_str, "%.3f ms",
            load_time + write_time + exec_time + read_time);

    ft_write_ln(table, filesize, load_time_str, write_time_str, exec_time_str,
                read_time_str, total_time_str, first, last);
  }

  free(src);
  free(dst);
  free(prog);

  printf("%s\n", ft_to_string(table));
  ft_destroy_table(table);
}