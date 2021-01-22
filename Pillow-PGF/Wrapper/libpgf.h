#pragma once

#include <stdint.h>
#include <stdbool.h>
#ifdef WIN64 
#include <Windows.h>
#else
typedef int HANDLE;
#endif

// C-Wrapper for C++ library

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct CPGFImage CPGFImage;
	typedef struct CPGFFileStream CPGFFileStream;

	CPGFImage* newPGFImage();
	void deletePGFImage(CPGFImage* pgf);
	void readPGFImage(CPGFImage* pgf);
	void openPGFImage(CPGFImage* pgf, CPGFFileStream* stream);
	void copyBitmap(CPGFImage* pgf, uint8_t* buf, size_t bytes, int stride, int bpp, int channelMap[]);
	void configureDecoder(CPGFImage* pgf, bool useOMP);
	uint8_t getMode(CPGFImage* pgf);
	uint32_t getWidth(CPGFImage* pgf);
	uint32_t getHeight(CPGFImage* pgf);

	CPGFFileStream* newPGFFileStream(HANDLE fd);
	void deletePGFFileStream(CPGFFileStream* stream);
	uint64_t getPos(CPGFFileStream* stream);

#ifdef __cplusplus
}
#endif

