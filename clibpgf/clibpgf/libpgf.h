#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// C-Wrapper for C++ library

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct CPGFImage CPGFImage;
	typedef struct CPGFFileStream CPGFFileStream;

	bool checkPGFError(const char** errorMsg);

	CPGFImage* newPGFImage();
	void deletePGFImage(CPGFImage* pgf);
	void readPGFImage(CPGFImage* pgf);
	void openPGFImage(CPGFImage* pgf, CPGFFileStream* stream);
	void copyBitmap(CPGFImage* pgf, uint8_t* buf, int stride, int bpp, int channelMap[]);
	void configureDecoder(CPGFImage* pgf, bool useOMP);
	uint8_t getBpp(CPGFImage* pgf);
	uint8_t getMode(CPGFImage* pgf);
	uint32_t getWidth(CPGFImage* pgf);
	uint32_t getHeight(CPGFImage* pgf);

	CPGFFileStream* newPGFFileStream(int fd);
	CPGFFileStream* newPGFFileStream2(FILE* file);
	void deletePGFFileStream(CPGFFileStream* stream);
	uint64_t getPos(CPGFFileStream* stream);
	void setPos(CPGFFileStream* stream, int64_t posOff);

#ifdef __cplusplus
}
#endif

