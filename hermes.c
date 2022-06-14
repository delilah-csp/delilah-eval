/*******************************************************************************
 * Copyright 2020 Eideticom Inc
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#include "hermes_uapi.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>

static int hermes_fd;

void hermes_open() {
  hermes_fd = open("/dev/hermes0", O_RDWR);

  if (hermes_fd == -1) {
    fprintf(stderr, "Failed to open device\n");
    exit(1);
  }
}

void hermes_close() {
  close(hermes_fd);
}

void hermes_write(void *buf, size_t size){
  int rv = write(hermes_fd, buf, size);

  if(rv < 0) {
    perror("Failed to write data");
    exit(2);
  }
}

void hermes_read(void *buf, size_t size){
  int rv = read(hermes_fd, buf, size);

  if(rv < 0){
    perror("Failed to read data");
    exit(3);
  }
}

void hermes_load(void *buf, size_t size){
  struct hermes_download_prog_ioctl_argp argp = {
    .len = size * 16,
    .prog = (uint64_t) buf,
  };

  int rv = ioctl(hermes_fd, HERMES_DOWNLOAD_PROG_IOCTL, &argp);

  if (rv < 0){
    perror("Failed to write eBPF program");
    exit(4);
  }
}

void hermes_exec(){
  int rv = fsync(hermes_fd);

  if (rv < 0) {
    perror("Failed to execute program");
    exit(5);
  }
}
