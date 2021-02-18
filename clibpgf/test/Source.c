#include "libpgf.h"
#include <stdio.h>
#include <malloc.h>

// Test program in C

int main(int argc, char* argv[]) {
	FILE* stream;
	errno_t err = fopen_s(&stream, "Garden.pgf", "r");

	if (err) {
		perror("Error: Could not open source file.");
	} else {
		const char* error = NULL;
		uint8_t* buf = NULL;
		CPGFImage* pgf = newPGFImage();
		CPGFFileStream* s = newPGFFileStream2(stream);

		uint64_t pos = getPos(s);

		openPGFImage(pgf, s);
		if (checkPGFError(&error)) goto quick_exit;
		
		int32_t width = getWidth(pgf);
		int32_t height = getHeight(pgf);
		int bpp = getBpp(pgf);
		int stride = (width*bpp + 7)/8;
		int32_t bytes = height*stride;
		buf = malloc(bytes);
		printf("PGF mode = %d, width = %d, height = %d\n", getMode(pgf), width, height);
		
		readPGFImage(pgf);
		if (checkPGFError(&error)) goto quick_exit;

		int map[] = { 2, 1, 0 }; // BGR to RGB
		copyBitmap(pgf, buf, stride, bpp, map);
		if (checkPGFError(&error)) goto quick_exit;

	quick_exit:
		if (error) {
			printf("PGF error: %s\n", error);
			free(error);
		}
		if (buf) free(buf);
		deletePGFFileStream(s);
		deletePGFImage(pgf);
		fclose(stream);

	}
}