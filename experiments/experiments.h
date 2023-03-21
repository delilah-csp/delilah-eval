#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

void experiment_version(struct io_uring* ring, int fd);
void experiment_read_file(struct io_uring* ring, int fd);
void experiment_read_file_cache(struct io_uring* ring, int fd);
void experiment_cmb(struct io_uring* ring, int fd);
void experiment_dma_1(struct io_uring* ring, int fd);
void experiment_dma_4(struct io_uring* ring, int fd);

#endif
