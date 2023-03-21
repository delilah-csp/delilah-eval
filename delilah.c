#include "delilah.h"
#include "delilah_uapi.h"
#include <liburing.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define DATA_SIZE 1024 * 1024 * 128
#define PROG_SIZE 1024 * 1024

struct delilah_mount_t fs0 = { .nvme_handle = "nvme0n1p1",
                               .mount_point = "/media/nvme0n1p1/",
                               .fs_type = "ext4" };

int
delilah_write_prog(struct io_uring* ring, int fd, void* buf, size_t len)
{
  struct io_uring_sqe* sqe;
  struct delilah_dma* dma;
  sqe = io_uring_get_sqe(ring);

  sqe->opcode = IORING_OP_URING_CMD;
  sqe->fd = fd;
  sqe->cmd_op = DELILAH_OP_PROG_WRITE;

  dma = (struct delilah_dma*)&sqe->cmd;
  dma->slot = 0, dma->buf = (uint64_t)buf, dma->len = len;

  return io_uring_submit(ring);
}

int
delilah_exec_prog(struct io_uring* ring, int fd, int prog, int data, int eng,
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

  exec->invalidation_size = invalidation_size;
  exec->invalidation_offset = 0;
  exec->flush_size = flush_size;
  exec->flush_offset = 0;

  return io_uring_submit(ring);
}

int
delilah_io(struct io_uring* ring, int fd, void* buf, size_t len, bool write)
{
  struct io_uring_sqe* sqe;
  struct delilah_dma* dma;

  sqe = io_uring_get_sqe(ring);

  sqe->opcode = IORING_OP_URING_CMD;
  sqe->fd = fd;
  sqe->cmd_op = write ? DELILAH_OP_DATA_WRITE : DELILAH_OP_DATA_READ;

  dma = (struct delilah_dma*)&sqe->cmd;
  dma->slot = 0, dma->buf = (uint64_t)buf, dma->len = len;

  return io_uring_submit(ring);
}

size_t
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

int
main()
{
  struct io_uring ring;
  struct io_uring_cqe* cqe;
  struct io_uring_params p = {};
  struct delilah_dma* dma;
  struct delilah_exec* exec;

  char *src, *dst, *prog;
  size_t sz;
  int fd, ret;

  src = malloc(DATA_SIZE);
  dst = malloc(DATA_SIZE);
  prog = malloc(PROG_SIZE);

  /* Open Delilah device */
  fd = open("/dev/delilah0", O_RDWR);
  if (fd < 0) {
    perror("Cannot open Delilah device");
    return 3;
  }

  /* Initialize io_uring queue with four entries */
  p.flags = IORING_SETUP_SQE128;
  ret = io_uring_queue_init(4, &ring, p.flags);

  if (ret) {
    fprintf(stderr, "Cannot init queue\n");
    return 2;
  }

  // Write versioning program to Delilah
  sz = file_read(prog, PROG_SIZE, "programs/version.o");
  delilah_write_prog(&ring, fd, prog, sz);
  io_uring_wait_cqe(&ring, &cqe);
  io_uring_cqe_seen(&ring, cqe);

  // Execute versioning program on Delilah
  delilah_exec_prog(&ring, fd, 0, 0, 0, 0, sizeof(uint64_t));
  io_uring_wait_cqe(&ring, &cqe);
  io_uring_cqe_seen(&ring, cqe);

  // Read versioning result from Delilah
  delilah_io(&ring, fd, dst, 64, 0);
  io_uring_wait_cqe(&ring, &cqe);
  io_uring_cqe_seen(&ring, cqe);

  printf("Delilah reported itself to be of version: %ld\n\n",
         *((uint64_t*)dst));

  printf("Begin table with results: \n");
  printf("bytes, load, write, exec, read, v1, v2\n");

  uint64_t sizes[] = { 100, 1000, 10000, 100000, 1000000, 10000000 };
  struct delilah_file_t* file = (struct delilah_file_t*)src;
  struct timeval loading, writeing, executing, reading, done;
  struct timeval sub_load, sub_write, sub_exec, sub_read;

  sz = file_read(prog, PROG_SIZE, "programs/file.o");
  for (int i = 0; i < 6; i++) {
    file->size = sizes[i] * 10; // the size denotes the number of strings
    sprintf(file->filename, "/run/media/nvme0n1p1/%ld.txt", sizes[i]);

    gettimeofday(&loading, NULL);
    delilah_write_prog(&ring, fd, prog, sz);
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

    gettimeofday(&writeing, NULL);
    delilah_io(&ring, fd, src, sizeof(struct delilah_file_t), 1);
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

    gettimeofday(&executing, NULL);
    delilah_exec_prog(&ring, fd, 0, 0, 0, sizeof(struct delilah_file_t),
                      sizeof(struct delilah_file_t) + sizes[i] * 10);
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

    gettimeofday(&reading, NULL);
    delilah_io(&ring, fd, dst, sizeof(struct delilah_file_t) + sizes[i] * 10,
               0);
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

    gettimeofday(&done, NULL);

    timersub(&writeing, &loading, &sub_load);
    timersub(&executing, &writeing, &sub_write);
    timersub(&reading, &executing, &sub_exec);
    timersub(&done, &reading, &sub_read);

    printf("%lu, ", sizes[i] * 10);
    printf("%ld.%06ld, ", (long int)sub_load.tv_sec,
           (long int)sub_load.tv_usec);
    printf("%ld.%06ld, ", (long int)sub_write.tv_sec,
           (long int)sub_write.tv_usec);
    printf("%ld.%06ld, ", (long int)sub_exec.tv_sec,
           (long int)sub_exec.tv_usec);
    printf("%ld.%06ld, ", (long int)sub_read.tv_sec,
           (long int)sub_read.tv_usec);

    char first[10], last[10];
    first[9] = '\0';
    last[9] = '\0';

    memcpy(first, dst + sizeof(struct delilah_file_t), 9);
    memcpy(last, dst + sizeof(struct delilah_file_t) + (sizes[i] * 10) - 10, 9);

    printf("%s, %s\n", first, last);
  }

  return 0;
}
