#include "Python.h"
#include "../include/libpgf.h"
#include "Imaging.h"

static PyObject* decode(PyObject* self, PyObject* args)
{
    printf("Hello World\n");
    return Py_None;
}


static PyMethodDef pgfMethods[] = {
    { "decode", decode, METH_NOARGS, "Prints Hello World" },
    { NULL, NULL, 0, NULL }
};


static struct PyModuleDef _decode = {
    PyModuleDef_HEAD_INIT,
    "pgf_decode",
    "PGF Decode",
    -1,
    pgfMethods
};


PyMODINIT_FUNC PyInit__decode(void)
{
    return PyModule_Create(&_decode);
}

/*
* https://github.com/python-pillow/Pillow/blob/master/src/libImaging/Imaging.h
typedef struct ImagingCodecStateInstance *ImagingCodecState;
struct ImagingCodecStateInstance {
	int count;
	int state;
	int errcode;
	int x, y;
	int ystep;
	int xsize, ysize, xoff, yoff;
	ImagingShuffler shuffle;
	int bits, bytes;
	UINT8 *buffer;
	void *context;
	PyObject *fd;
};

*/
int ImagingPgfDecode(Imaging im, ImagingCodecState state, UINT8* buf, Py_ssize_t bytes) {
	struct CPGFImage *pgf = newPGFImage();		// needs Wrapper.lib
	struct CPGFFileStream *stream(state->fd);	// needs Wrapper.lib

	// optional PGF encoder configuration
	configureDecoder(pgf, true); // true: use openMP (if codec is compiled with openMP)

	try {
		// open pgf image
		unit64_t startpos = getPos(stream);
		openPGFImage(pgf, stream);

		// read entire file down to level 0
		readPGFImage(pgf);
		unit64_t sourceSize = getPos(stream) - startpos;

	} catch (IOException& e) {
		deletePGFFileStream(stream);
		deletePGFImage(pgf);
		return -1;
	}

	uint8_t mode = getMode(pgf);

	// copy image to buffer
	if (mode == ImageModeRGB) {
		int map[] = { 2, 1, 0 };
		copyBitmap(pgf, buf, bytes, stride, map);
	}

	// update color table if image is indexed or bitmap
	if (mode == ImageModeIndexedColor) {
		// cannot get number of color table entries directly, so use 2^bitdepth
		//image->SetColorTable(0, 1 << pgf.BPP(), pgf.GetColorTable());
	} else if (mode == ImageModeBitmap) {
		//RGBQUAD bw[2];
		//bw[0].rgbRed = 255;
		//bw[0].rgbGreen = 255;
		//bw[0].rgbBlue = 255;
		//bw[1].rgbRed = 0;
		//bw[1].rgbGreen = 0;
		//bw[1].rgbBlue = 0;
		//image->SetColorTable(0, 2, bw);
	}

	deletePGFFileStream(stream);
	deletePGFImage(pgf);

    return 0;
}

int ImagingPgfEncode(Imaging im, ImagingCodecState state, UINT8* buf, int bytes) {
    return 0;
}