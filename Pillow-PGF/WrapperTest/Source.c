#include "libpgf.h"
#include <stdio.h>
//#include <Windows.h>

// Test program in C

int main(int argc, char* argv[]) {
	HANDLE fd = CreateFile(L"Garden.pgf", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (fd == INVALID_HANDLE_VALUE) {
		perror("Error: Could not open source file.");
	} else {
		struct CPGFImage* pgf = newPGFImage();
		struct CPGFFileStream* s = newPGFFileStream(fd);

		uint64_t pos = getPos(s);

		openPGFImage(pgf, s);
		printf("PGF mode = %d, width = %d, height = %d\n", getMode(pgf), getWidth(pgf), getHeight(pgf));

		deletePGFFileStream(s);
		deletePGFImage(pgf);
		CloseHandle(fd);
	}
}