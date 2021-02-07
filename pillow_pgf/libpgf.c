#include "Python.h"
#include "libpgf.h"
#include "Imaging.h"

// https://pillow.readthedocs.io/en/5.1.x/handbook/writing-your-own-file-decoder.html

static PyObject* decode(PyObject* self, PyObject* args)
{
    printf("Hello World\n");
    return Py_None;
	//return decodePGF(args.fd, args.dst, args.bytes, args.stride);
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


PyMODINIT_FUNC PyInit_libpgf(void)
{
    return PyModule_Create(&_decode);
}

static int decodePGF(HANDLE fd, UINT8* buf, Py_ssize_t bytes, int stride) {
	struct CPGFImage *pgf = newPGFImage();
	struct CPGFFileStream *stream = newPGFFileStream(fd);

	// optional PGF encoder configuration
	configureDecoder(pgf, true); // true: use openMP (if codec is compiled with openMP)

	// open pgf image
	setPos(stream, FSFromStart, 0);
	uint64_t startpos = getPos(stream);
	openPGFImage(pgf, stream);

	// read entire file down to level 0
	readPGFImage(pgf);
	uint64_t sourceSize = getPos(stream) - startpos;
	uint8_t mode = getMode(pgf);

	// copy image to buffer (BGR mode)
	if (mode == ImageModeRGBColor) {
		//int map[] = { 2, 1, 0 }; // BGR to RGB
		int map[] = { 0, 1, 2 }; // BGR to BGR
		copyBitmap(pgf, buf, bytes, stride, 24, map);
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

static int encodePGF(HANDLE fd, UINT8* buf, Py_ssize_t bytes, int stride) {
    return 0;
}