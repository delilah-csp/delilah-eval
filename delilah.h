#ifndef DELILAH_INT_H
#define DELILAH_INT_H

#include "libfort/lib/fort.h"
#include <liburing.h>
#include <stdint.h>

#define DATA_SIZE 1024 * 1024 * 128
#define PROG_SIZE 1024 * 1024

struct delilah_file_t
{
  uint64_t size;
  char filename[128];
};

struct delilah_mount_t
{
  char nvme_handle[32];
  char mount_point[128];
  char fs_type[32];
};

#endif