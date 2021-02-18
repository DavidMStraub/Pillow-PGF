/*
 * The Python Imaging Library.
 * $Id$
 *
 * decoder for PGF image data.
 *
 * history:
 * 2021-02-08 cs  Created
 *
 * Copyright (c) 2021 Christoph Stamm
 *
 * See the README file for details on usage and redistribution.
 */

#include "Imaging.h"
//#define HAVE_PGF
#ifdef HAVE_PGF

#include <stdlib.h>
#include "PGF.h"

enum {
    PGF_STATE_START = 0,
    PGF_STATE_DECODING = 1,
    PGF_STATE_DONE = 2,
    PGF_STATE_FAILED = 3,
};

static void
unpackBitmap(uint32_t width, uint32_t height, uint8_t *buffer) {
    const int pitch = (width + 7) / 8;
    const int lastByteLen = width + 8 - pitch * 8;

    // iterate rows
	for (uint32_t i = 0; i < height; i++) {
        int len = lastByteLen;
        int bytePos = pitch - 1;
        int val = buffer[bytePos];
        int bit = 1 << (8 - len);

        // iterate from right to left in a row and spread packed bits
        for (uint32_t j = width - 1; j >= 0; j--) {
            buffer[j] = (val & bit);
            bit <<= 1;
            len--;
            if (len == 0) {
                len = 8;
                bytePos--;
                val = buffer[bytePos];
                bit = 1;
            }
        }
        buffer += width;
    }
}

/* -------------------------------------------------------------------- */
/* Decoder                                                              */
/* -------------------------------------------------------------------- */

int ImagingPgfDecode(Imaging im, ImagingCodecState state, UINT8 *buf, Py_ssize_t bytes) {
    state->state = PGF_STATE_START;

    PGFDECODESTATE *context = (PGFDECODESTATE *)state->context;
    context->error_msg = NULL;

    context->pgf = newPGFImage();
    if (context->pgf == NULL) {
        state->errcode = IMAGING_CODEC_BROKEN;
        state->state = PGF_STATE_FAILED;
        goto quick_exit;
    }
    context->stream = newPGFFileStream(context->fd);
    if (context->stream == NULL) {
        state->errcode = IMAGING_CODEC_BROKEN;
        state->state = PGF_STATE_FAILED;
        goto quick_exit;
    }

	// optional PGF encoder configuration
    configureDecoder(context->pgf, true);  // true: use openMP (if codec is compiled with openMP)

    // open pgf image
    setPos(context->stream, 0);
    if (checkPGFError(&context->error_msg)) {
        state->errcode = IMAGING_CODEC_MEMORY;
        state->state = PGF_STATE_FAILED;
        goto quick_exit;
    }

    //uint64_t startpos = getPos(context->stream);
    openPGFImage(context->pgf, context->stream);
    if (checkPGFError(&context->error_msg)) {
        state->errcode = IMAGING_CODEC_BROKEN;
        state->state = PGF_STATE_FAILED;
        goto quick_exit;
    }

    // read entire file down to level 0
    state->state = PGF_STATE_DECODING;
    readPGFImage(context->pgf);
    if (checkPGFError(&context->error_msg)) {
        state->errcode = IMAGING_CODEC_BROKEN;
        state->state = PGF_STATE_FAILED;
        goto quick_exit;
    }

    // uint64_t sourceSize = getPos(context->stream) - startpos;

    //return -1;

    size_t stride = 0;
    uint32_t width = getWidth(context->pgf);
    uint32_t height = getHeight(context->pgf);
    uint8_t mode = getMode(context->pgf);
    int map[4] = {0, 1, 2, 3};

    /*
    0: ("1", 1),    # needs unpacking to 1 bit per byte
    1: ("L", 1),    # 8 bit grayscale
    2: ("P", 1),    # 8 bit indexed (palette needed)
    3: ("RGB", 3),  # 24 bit true color, BGR->RGB mapping necessary
    4: ("CMYK", 4),
    9: ("LAB", 3),
    17: ("RGBA", 4) # channel mapping necessary
    */
    switch (mode) {
        case 0:
        case 1:
        case 2:
            stride = width;
            break;
        case 3:
        case 9:
            stride = 3 * width;
            break;
        case 4:
        case 17:
            stride = 4 * width;
            break;
    }
    if (stride == 0 || height == 0) {
        state->errcode = IMAGING_CODEC_BROKEN;
        state->state = PGF_STATE_FAILED;
        goto quick_exit;
    }

    // allocate image buffer
    uint8_t *buffer = malloc(stride * height);

    map[0] = 2;
    map[2] = 0;
    copyBitmap(context->pgf, buffer, stride, 24, map);

    // copy im->block to im->image
    int pos = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            im->image[i][4*j+0] = buffer[pos++];
            im->image[i][4*j+1] = buffer[pos++];
            im->image[i][4*j+2] = buffer[pos++];
        }
    }
    free(buffer);
    state->state = PGF_STATE_DONE;
    return -1;

    // copy image to im->block
    switch (mode) {
        case 0: {
            copyBitmap(context->pgf, im->block, stride, 1, map); // bpp must be 1
            unpackBitmap(width, height, im->block); // now bpp is 8
            break;
        }
        case 1:
            copyBitmap(context->pgf, im->block, stride, 8, map);
            break;
        case 2: {
            copyBitmap(context->pgf, im->block, stride, 8, map);
            // TODO set color table
            // image->SetColorTable(0, 1 << context->pgf.BPP(), pgf.GetColorTable());
            break;
        }
        case 3:
            map[0] = 2;
            map[2] = 0;
            copyBitmap(context->pgf, im->block, stride, 24, map);

            // copy im->block to im->image
            int pos = 0;
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    IMAGING_PIXEL_RGB(im, j, i+0) = im->block[pos++];
                    IMAGING_PIXEL_RGB(im, j, i+1) = im->block[pos++];
                    IMAGING_PIXEL_RGB(im, j, i+2) = im->block[pos++];
                }
            }
            break;
        case 4:
            copyBitmap(context->pgf, im->block, stride, 32, map);
            break;
        case 9:
            copyBitmap(context->pgf, im->block, stride, 24, map);
            break;
        case 17:
            map[0] = 2;
            map[2] = 0;
            copyBitmap(context->pgf, im->block, stride, 32, map);
            break;
    }
    if (checkPGFError(&context->error_msg)) {
        state->errcode = IMAGING_CODEC_BROKEN;
        state->state = PGF_STATE_FAILED;
        goto quick_exit;
    }



    state->state = PGF_STATE_DONE;

quick_exit:
    free(im->block);
    return -1;
}

/* -------------------------------------------------------------------- */
/* Cleanup                                                              */
/* -------------------------------------------------------------------- */

int ImagingPgfDecodeCleanup(ImagingCodecState state) {
    PGFDECODESTATE *context = (PGFDECODESTATE *)state->context;

    if (context->error_msg) {
        free((void *)context->error_msg);
    }
    context->error_msg = NULL;

    if (context->pgf) {
        deletePGFImage(context->pgf);
    }
    context->pgf = NULL;

    if (context->stream) {
        deletePGFFileStream(context->stream);
    }
    context->stream = NULL;

    return -1;
}

const char *ImagingPgfVersion(void) {
    return "7.21.2";
}

#endif /* HAVE_PGF */

