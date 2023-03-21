#ifndef HELPERS_H
#define HELPERS_H

#include "delilah.h"
#include "delilah_uapi.h"
#include <liburing.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

static int
delilah_write_prog(struct io_uring* ring, int fd, uint8_t slot, void* buf,
                   size_t len)
{
  struct io_uring_sqe* sqe;
  struct delilah_dma* dma;

  // Get a submission queue entry
  sqe = io_uring_get_sqe(ring);

  sqe->opcode = IORING_OP_URING_CMD;
  sqe->fd = fd;
  sqe->cmd_op = DELILAH_OP_PROG_WRITE;

  dma = (struct delilah_dma*)&sqe->cmd;
  dma->slot = slot, dma->buf = (uint64_t)buf, dma->len = len;

  // Submit the SQE to the device async
  return io_uring_submit(ring);
}

static int
delilah_exec_prog(struct io_uring* ring, int fd, uint8_t prog, uint8_t data, uint8_t eng,
                  uint32_t invalidation_size, uint32_t flush_size)
{
  struct io_uring_sqe* sqe;
  struct delilah_exec* exec;

  sqe = io_uring_get_sqe(ring);

  sqe->opcode = IORING_OP_URING_CMD;
  sqe->fd = fd;
  sqe->cmd_op = DELILAH_OP_PROG_EXEC;

  exec = (struct delilah_exec*)&sqe->cmd;
  exec->prog_slot = prog;
  exec->data_slot = data;
  exec->eng = eng;

  exec->invalidation_size =
    invalidation_size; // How many bytes to invalidate from the CPU cache
  exec->invalidation_offset = 0; // Invalidation offset
  exec->flush_size =
    flush_size; // How many bytes to flush from the CPU cache to the DMA buffer
  exec->flush_offset = 0; // Flush offset

  return io_uring_submit(ring);
}

static int
delilah_io(struct io_uring* ring, int fd, uint8_t slot, void* buf, size_t len,
           bool write)
{
  struct io_uring_sqe* sqe;
  struct delilah_dma* dma;

  sqe = io_uring_get_sqe(ring);

  sqe->opcode = IORING_OP_URING_CMD;
  sqe->fd = fd;
  sqe->cmd_op = write ? DELILAH_OP_DATA_WRITE : DELILAH_OP_DATA_READ;

  dma = (struct delilah_dma*)&sqe->cmd;
  dma->slot = slot, dma->buf = (uint64_t)buf, dma->len = len;

  return io_uring_submit(ring);
}

static size_t
file_read(void* buffer, size_t size, char* filename)
{
  size_t i = 0;
  size_t fsize, rsize;
  char* buf = (char*)buffer;

  FILE* f = fopen(filename, "rb");

  fseek(f, 0, SEEK_END);
  fsize = ftell(f);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  fread(buf, 1, (fsize < size - 1) ? fsize : size - 1, f);
  fclose(f);

  buf[(fsize < size - 1) ? fsize : size] = 0;

  rsize = (fsize < size - 1) ? fsize : size;

  return rsize;
}

static inline void add_units(char* buf, double val)
{
  if (val < 1000) {
    sprintf(buf, "%.2f  B", val);
  } else if (val < 1000000) {
    sprintf(buf, "%.2f KB", val / 1000);
  } else if (val < 1000000000) {
    sprintf(buf, "%.2f MB", val / 1000000);
  } else {
    sprintf(buf, "%.2f GB", val / 1000000000);
  }
}

static inline struct timeval
clock_start()
{
  struct timeval start;
  gettimeofday(&start, NULL);
  return start;
}

static inline double
clock_end(struct timeval start)
{
  struct timeval end;
  gettimeofday(&end, NULL);

  return (end.tv_sec - start.tv_sec) +
         (end.tv_usec - start.tv_usec) / 1000000.0;
}

#endif