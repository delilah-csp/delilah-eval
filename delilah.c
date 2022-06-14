#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <string.h>
#include "delilah.h"

#define DATA_SIZE 1024 * 1024 * 128
#define PROG_SIZE 1024 * 1024

int hermes_open();
int hermes_close();
int hermes_write(void *buf, size_t size);
int hermes_read(void *buf, size_t size);
int hermes_load(void *buf, size_t size);
int hermes_exec();

size_t file_read(void *buffer, size_t size, char *filename) {
  size_t i = 0;
  size_t fsize, rsize;
  char *buf = (char *)buffer;

  FILE *f = fopen(filename, "rb");

  fseek(f, 0, SEEK_END);
  fsize = ftell(f);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  fread(buf, 1, (fsize < size - 1) ? fsize : size - 1, f);
  fclose(f);

  buf[(fsize < size - 1) ? fsize : size] = 0;

  rsize = (fsize < size - 1) ? fsize : size;

  return rsize;
}

int main(){
  hermes_open();

  char *src, *dst, *prog;
  size_t sz;

  src = malloc(DATA_SIZE);
  dst = malloc(DATA_SIZE);
  prog = malloc(PROG_SIZE);

  sz = file_read(prog, PROG_SIZE, "programs/version.o");
  hermes_load(prog, sz);
  hermes_write(src, 4096);
  hermes_exec();
  hermes_read(dst, 4096);

  printf("Delilah reported itself to be of version: %ld\n\n", *((uint64_t *) dst));

  uint64_t sizes[] = { 100, 1000, 10000, 100000, 1000000, 10000000 };
  struct delilah_file_t *file = (struct delilah_file_t *) src;
  struct timeval loading, writeing, executing, reading, done;
  struct timeval sub_load, sub_write, sub_exec, sub_read;

  sz = file_read(prog, PROG_SIZE, "programs/file.o");
  for(int i = 0; i < 6; i++){
    file->size = sizes[i] * 10; // the size denotes the number of strings
    sprintf(file->filename, "/media/nvme/%ld.txt", sizes[i]);

    gettimeofday(&loading, NULL);
    hermes_load(prog, sz);

    gettimeofday(&writeing, NULL);
    hermes_write(src, sizeof(struct delilah_file_t));

    gettimeofday(&executing, NULL);
    hermes_exec();

    gettimeofday(&reading, NULL);
    hermes_read(dst, sizeof(struct delilah_file_t) + sizes[i] * 10);

    gettimeofday(&done, NULL);

    printf("Completed evaluation of %ld strings, %ld bytes\n", sizes[i], sizes[i] * 10);
    timersub(&writeing, &loading, &sub_load);
    timersub(&executing, &writeing, &sub_write);
    timersub(&reading, &executing, &sub_exec);
    timersub(&done, &reading, &sub_read);

    printf("Load  : %ld.%06ld s\n", (long int)sub_load.tv_sec, (long int)sub_load.tv_usec);
    printf("Write : %ld.%06ld s\n", (long int)sub_write.tv_sec, (long int)sub_write.tv_usec);
    printf("Exec  : %ld.%06ld s\n", (long int)sub_exec.tv_sec, (long int)sub_exec.tv_usec);
    printf("Read  : %ld.%06ld s\n", (long int)sub_read.tv_sec, (long int)sub_read.tv_usec);

    char first[10], last[10];
    first[9] = '\0';
    last[9] = '\0';

    memcpy(first, dst + sizeof(struct delilah_file_t), 9);
    memcpy(last, dst + sizeof(struct delilah_file_t) + (sizes[i] * 10) - 10, 9);

    printf("Control strings: %s, %s\n\n",
     first,
     last
    );
  }


  return 0;
}

