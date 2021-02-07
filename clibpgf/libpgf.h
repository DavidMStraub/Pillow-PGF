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
	//-------------------------------------------------------------------------------
	//	Defines -- Adobe image modes.
	//-------------------------------------------------------------------------------
	#define ImageModeBitmap				0
	#define ImageModeGrayScale			1
	#define ImageModeIndexedColor		2
	#define ImageModeRGBColor			3
	#define ImageModeCMYKColor			4
	#define ImageModeHSLColor			5
	#define ImageModeHSBColor			6
	#define ImageModeMultichannel		7
	#define ImageModeDuotone			8
	#define ImageModeLabColor			9
	#define ImageModeGray16				10		// 565
	#define ImageModeRGB48				11
	#define ImageModeLab48				12
	#define ImageModeCMYK64				13
	#define ImageModeDeepMultichannel	14
	#define ImageModeDuotone16			15
	// pgf extension
	#define ImageModeRGBA				17
	#define ImageModeGray32				18		// MSB is 0 (can be interpreted as signed 15.16 fixed point format)
	#define ImageModeRGB12				19
	#define ImageModeRGB16				20
	#define ImageModeUnknown			255

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

