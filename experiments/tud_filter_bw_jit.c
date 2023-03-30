#include "../helpers.h"
#include "experiments.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
experiment_tud_filter_bw_jit(struct io_uring* ring, int fd)
{
  struct io_uring_cqe* cqe;
  struct delilah_dma* dma;
  struct delilah_exec* exec;
  filter_count_op *op = (filter_count_op *)malloc(sizeof(filter_count_op));
  char* prog = (char*)malloc(4 * 1024 * 1024);

  op->comp_type = BW;
  op->comp0 = 0;
  op->comp1 = 100;
  op->result = 0;

  uint64_t sizes[] = { 1024, 10240, 102400, 1048576, 10485760, 104857600, 524288000 };
  size_t sz = file_read(prog, PROG_SIZE, "programs/filter.o");

  ft_table_t* table = ft_create_table();
  /* Set "header" type for the first row */
  ft_set_cell_prop(table, 0, FT_ANY_COLUMN, FT_CPROP_ROW_TYPE, FT_ROW_HEADER);
  ft_set_cell_prop(table, FT_ANY_ROW, 0, FT_CPROP_TEXT_ALIGN, FT_ALIGNED_RIGHT);

  ft_write_ln(table, "File Size", "Write Prog", "Write State", "Exec Prog",
              "Read Result", "Total Time", "Result");

  for (int i = 0; i < 7; i++) {
    op->file.size = sizes[i];
    sprintf(op->file.filename, "/run/media/nvme0n1p1/tud/unsigned_int_%ld.dat", sizes[i]);

    struct timeval loading = clock_start();
    delilah_write_prog(ring, fd, 0, prog, sz);
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double load_time = clock_end(loading) * 1000;

    struct timeval write = clock_start();
    delilah_io(ring, fd, 0, op, sizeof(filter_count_op), 1);
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double write_time = clock_end(write) * 1000;

    struct timeval exec = clock_start();
    delilah_exec_prog_jit(ring, fd, 0, 0, 0, sizeof(filter_count_op),
                      sizeof(filter_count_op));
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double exec_time = clock_end(exec) * 1000;

    struct timeval read = clock_start();
    delilah_io(ring, fd, 0, op, sizeof(filter_count_op),
               0);
    io_uring_wait_cqe(ring, &cqe);
    io_uring_cqe_seen(ring, cqe);
    double read_time = clock_end(read) * 1000;

    char filesize[32], load_time_str[16], write_time_str[16], exec_time_str[16],
      read_time_str[16], total_time_str[16], result[32];

    add_units(filesize, sizes[i]);
    sprintf(load_time_str, "%.3f ms", load_time);
    sprintf(write_time_str, "%.3f ms", write_time);
    sprintf(exec_time_str, "%.3f ms", exec_time);
    sprintf(read_time_str, "%.3f ms", read_time);
    sprintf(total_time_str, "%.3f ms",
            load_time + write_time + exec_time + read_time);
    sprintf(result, "%ld", op->result);

    ft_write_ln(table, filesize, load_time_str, write_time_str, exec_time_str,
                read_time_str, total_time_str, result);
  }

  free(op);
  free(prog);

  printf("%s\n", ft_to_string(table));
  ft_destroy_table(table);
}