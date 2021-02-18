/*
 * The Python Imaging Library
 * $Id$
 *
 * declarations for the PGF codec interface.
 *
 * Copyright (c) 2021 by Christoph Stamm
 */

#include "libpgf.h"

/* -------------------------------------------------------------------- */
/* Decoder                                                              */
/* -------------------------------------------------------------------- */

typedef struct {
    /* CONFIGURATION */

    char *mode;
    int fd;

    /* PRIVATE CONTEXT (set by decoder) */
    CPGFImage *pgf;
    CPGFFileStream *stream;
    const char *error_msg;

} PGFDECODESTATE;

/* -------------------------------------------------------------------- */
/* Encoder                                                              */
/* -------------------------------------------------------------------- */

typedef struct {
    /* CONFIGURATION */

    char *mode;
    int fd;

    /* PRIVATE CONTEXT (set by decoder) */
    CPGFImage *pgf;
    CPGFFileStream *stream;
    const char *error_msg;

} PGFENCODESTATE;
