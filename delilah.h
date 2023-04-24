#ifndef DELILAH_INT_H
#define DELILAH_INT_H

#include "libfort/lib/fort.h"
#include <liburing.h>
#include <stdint.h>

#define DATA_SIZE 1024 * 1024 * 1024
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

/*
# ifndef __uint32_t_defined
# define __uint32_t_defined
*/
typedef unsigned int tud_uint32_t;
/*
# endif
# ifndef __uint64_t_defined
# define __uint64_t_defined
*/
typedef unsigned long tud_uint64_t;
/*
# endif
*/
#include <stdbool.h>

/* DB-Operator related struct BEGIN */
typedef struct {
    enum COMP_TYPE {
        EQ, NEQ, LT, LE, GT, GE, 
        BW      /*BETWEEN:                  (comp0,comp1)*/, 
        BWI     /*BETWEEN inclusive:        [comp0,comp1]*/, 
        BWLI    /*BETWEEN low inclusive:    [comp0,comp1)*/, 
        BWHI    /*BETWEEN high inclusive:   (comp0,comp1]*/
    } comp_type;
    tud_uint32_t comp0;
    tud_uint32_t comp1;
    struct delilah_file_t file;
    tud_uint64_t result;
} filter_count_op;
/* DB-Operator related struct END */


#endif
