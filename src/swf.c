#include "swf.h"
#include "swf_types.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>
#include <psp2/kernel/clib.h>

#include "tag_types.h"

swf read_swf(FILE* file) {
    swf dummy_swf; 

    fread(&dummy_swf.signature, 1, 3, file);

    sceClibPrintf("[vcfp] swf signature: %c%c%c\n", dummy_swf.signature[0], dummy_swf.signature[1], dummy_swf.signature[2]);

    fread(&dummy_swf.version, 1, 1, file);

    sceClibPrintf("[vcfp] swf version: %i\n", dummy_swf.version);

    fread(&dummy_swf.file_length, 4, 1, file);

    if (dummy_swf.signature[0] == 'C') {
        sceClibPrintf("[vcfp] swf is compressed; compressing now.\n");

        z_stream zs;
        const int MAX_BUFFER = 128 * 1024;

        uint8_t *input_buffer = (uint8_t *)malloc(MAX_BUFFER);
        uint8_t *uncompressed_file = (uint8_t *)malloc(dummy_swf.file_length);

        memset(&zs, 0, sizeof(z_stream));
        //fread(compressed_file, 1, dummy_swf.file_length - ftell(file), file);

        zs.avail_in = 0;
        zs.avail_out = dummy_swf.file_length;
        zs.next_in = Z_NULL;
        zs.next_out = uncompressed_file;
        zs.zalloc = Z_NULL;
		zs.zfree = Z_NULL;
		zs.opaque = Z_NULL;

        int status = inflateInit(&zs);

        if (status != Z_OK) {
            sceClibPrintf("inflateInit failed\n");
            free(input_buffer);
            free(uncompressed_file);
            return;
        }

        sceClibPrintf("[vcfp] ");

        do {
            sceClibPrintf("decompressing... ");
            if (zs.avail_in == 0) {
                zs.next_in = input_buffer;
                zs.avail_in = fread(input_buffer, 1, MAX_BUFFER, file);
            }
            if (zs.avail_in == 0)
                break;
            status = inflate(&zs, Z_SYNC_FLUSH);
        } while(status != Z_STREAM_END);

        sceClibPrintf("\n");
        inflateEnd(&zs);

        if (status != Z_STREAM_END) {
            sceClibPrintf("decompress failed\n");
            free(input_buffer);
            free(uncompressed_file);
            return;
        }
        
        sceClibPrintf("[vcfp] swf decompression done\n");

        fclose(file);
        file = fmemopen((void*)uncompressed_file, dummy_swf.file_length, "rb");

        free(input_buffer);
        free(uncompressed_file);
    }

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
            case TAG_DefineShape: {
                tag_DefineShapeTag* tag = tag_read_DefineShape(file, _swf->records[i]);
                _swf->records[i].tag = tag;
                break;
            }
            case TAG_SetBackgroundColor: {
                swf_RGBA rgb = read_swf_RGB(file);
                _swf->records[i].tag = (tag_BackgroundColorTag*)malloc(sizeof(tag_BackgroundColorTag));
                ((tag_BackgroundColorTag*)_swf->records[i].tag)->rgb = rgb;
                //_swf->records[i]->tag->background_tag.rgb = rgb;
                break;
            }
            case TAG_PlaceObject2: {

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
            sceClibPrintf("Failed to allocate memory\n");
            exit(1);
        }

        _swf->records[size - 1] = record;
        fseek(file, record.length, SEEK_CUR);
    }

    _swf->record_count = size;
    sceClibPrintf("[vcfp] swf amount of tags: %i\n", size);
}