#ifndef __SWF__
#define __SWF__

#include <stdint.h>
#include "swf_types.h"

typedef struct {
    uint8_t version;

    char signature[3];

    uint32_t file_length;
    float frame_rate;

    swf_RECT frame_size;
    uint16_t frame_count;

    float frame_delay;

    swf_RECORDHEADER* records;
    int record_count;
} swf;
swf read_swf(FILE* file);
void read_swf_records(FILE* file, swf* _swf);

#endif