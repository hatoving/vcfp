#include "swf.h"
#include "swf_types.h"

#include <stdlib.h>
#include <psp2/kernel/clib.h>

swf read_swf(FILE* file) {
    swf dummy_swf; 

    fread(&dummy_swf.compression_type, 1, 1, file);
    fread(&dummy_swf.signature_1, 1, 1, file);
    fread(&dummy_swf.signature_2, 1, 1, file);
    fread(&dummy_swf.version, 1, 1, file);

    sceClibPrintf("[vcfp] swf version: %i\n", dummy_swf.version);

    fread(&dummy_swf.file_length, 4, 1, file);

    dummy_swf.frame_size = read_swf_RECT(file);

    sceClibPrintf("[vcfp] swf size: %i, %i\n", dummy_swf.frame_size.w, dummy_swf.frame_size.h);

    uint16_t frame_rate;
    fread(&frame_rate, 2, 1, file);
    dummy_swf.frame_rate = (frame_rate / 256);

    sceClibPrintf("[vcfp] swf fps: %f\n", dummy_swf.frame_rate);
    fread(&dummy_swf.frame_count, 2, 1, file);
    sceClibPrintf("[vcfp] pos (%li) swf count: %i\n", ftell(file) - 2, dummy_swf.frame_count);

    dummy_swf.frame_delay = 1000 / dummy_swf.frame_rate;

    read_swf_records(file, &dummy_swf);
    read_swf_tags(file, &dummy_swf);

    return dummy_swf;
}

void read_swf_tags(FILE* file, swf* _swf) {
    for (int i = 0; i < _swf->record_count; i++)
    {
        fseek(file, _swf->records[i].offset, SEEK_SET);

        switch(_swf->records[i].tag_type) {
            case 9: {
                swf_RGB rgb = read_swf_RGB(file);
                _swf->records->collection.background_tag.rgb = rgb;
                break;
            }
        }
    }
}

void read_swf_records(FILE* file, swf* _swf) {
    _swf->records = NULL;
    int numRecords = 0;
    int size = 0;

    while (1) {
        swf_RECORDHEADER record = read_swf_RECORDHEADER(file);
        if (record.tag_type == 0 && record.length == 0) {
            break;
        }

        size++;
        _swf->records = realloc(_swf->records, size * sizeof(swf_RECORDHEADER));
        if (_swf->records == NULL) {
            perror("Failed to allocate memory");
            exit(1);
        }

        _swf->records[size - 1] = record;
        fseek(file, record.length, SEEK_CUR);
    }

    _swf->record_count = size;
    sceClibPrintf("[vcfp] swf amount of tags: %i\n", size);
}