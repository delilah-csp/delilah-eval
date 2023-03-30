#include "../delilah.h"

int prog(void *ctx, int mem_size) {
    filter_count_op * op = (filter_count_op *)ctx;
    
    delilah_file_read(ctx + sizeof(filter_count_op), op->file.size, op->file.filename);

    tud_uint64_t element_count = op->file.size / sizeof(tud_uint32_t);
    tud_uint64_t result = 0;
    tud_uint32_t const * ptr = ctx + sizeof(filter_count_op);
    tud_uint32_t const * const end = ptr + element_count;

    // Assuming that branching is expensive, we only branch in the beginning and manually roll out everything else.
    if(op->comp_type == EQ) {
        tud_uint32_t const comp0 = op->comp0;
        for(; ptr != end; ++ptr) {
            result += (*ptr == comp0) ? 1:0;
        }
    } else if(op->comp_type == NEQ) {
        tud_uint32_t const comp0 = op->comp0;
        for(; ptr != end; ++ptr) {
            result += (*ptr == comp0) ? 0:1;
        }
    } else if(op->comp_type == LT) {
        tud_uint32_t const comp0 = op->comp0;
        for(; ptr != end; ++ptr) {
            result += (*ptr < comp0) ? 1:0;
        }
    } else if(op->comp_type == LE) {
        tud_uint32_t const comp0 = op->comp0;
        for(; ptr != end; ++ptr) {
            result += (*ptr <= comp0) ? 1:0;
        }
    } else if(op->comp_type == GT) {
        tud_uint32_t const comp0 = op->comp0;
        for(; ptr != end; ++ptr) {
            result += (*ptr > comp0) ? 1:0;
        }
    } else if(op->comp_type == GE) {
        tud_uint32_t const comp0 = op->comp0;
        for(; ptr != end; ++ptr) {
            result += (*ptr >= comp0) ? 1:0;
        }
    } else if(op->comp_type == BW) {
        tud_uint32_t const comp0 = op->comp0;
        tud_uint32_t const comp1 = op->comp1;
        for(; ptr != end; ++ptr) {
            result += ((*ptr > comp0) && (*ptr < comp1)) ? 1:0;
        }
    } else if(op->comp_type == BWI) {
        tud_uint32_t const comp0 = op->comp0;
        tud_uint32_t const comp1 = op->comp1;
        for(; ptr != end; ++ptr) {
            result += ((*ptr >= comp0) && (*ptr <= comp1)) ? 1:0;
        }
    } else if(op->comp_type == BWLI) {
        tud_uint32_t const comp0 = op->comp0;
        tud_uint32_t const comp1 = op->comp1;
        for(; ptr != end; ++ptr) {
            result += ((*ptr >= comp0) && (*ptr < comp1)) ? 1:0;
        }
    } else if(op->comp_type == BWHI) {
        tud_uint32_t const comp0 = op->comp0;
        tud_uint32_t const comp1 = op->comp1;
        for(; ptr != end; ++ptr) {
            result += ((*ptr > comp0) && (*ptr <= comp1)) ? 1:0;
        }
    }
        
    op->result = result;
    return 0x0;
}


 