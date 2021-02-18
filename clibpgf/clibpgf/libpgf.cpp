#define _CRT_SECURE_NO_WARNINGS
#define NEXCEPTIONS
#include "PGFimage.h"
#include "libpgf.h"
#if defined(WIN32) || defined(WIN64)
#include <io.h>
#endif

static const char* PGFErrors[] = {
	"no error",
	"memory allocation was not successfull",
	"invalid memory stream position",
	"user break by ESC",
	"wrong pgf version",
	"wrong data file format",
	"image is too small",
	"error in zlib functions",
	"errors related to color table size",
	"errors in png functions",
	"expected data cannot be read",
};
static OSError err;

extern "C" {
	bool checkPGFError(const char** errorMsg) {
		if (*errorMsg) {
			// contains already a store error message
			return true;
		} else {
			const char* msg = NULL;

			if (err != NoError) {
				size_t len = 0;

				if (err >= AppError) {
					msg = PGFErrors[err - AppError];
				} else {
					char s[10];
					sprintf(s, "%d", err);
					msg = s;
				}
				*errorMsg = _strdup(msg);
				return true;
			} else {
				return false;
			}
		}
	}

	CPGFImage* newPGFImage() {
		return new CPGFImage(); 
	}
	void deletePGFImage(CPGFImage* pgf) {
		delete pgf; 
	}
	void readPGFImage(CPGFImage* pgf) {
		try {
			pgf->Read();
		} catch (IOException& ex) {
			err = ex.error;
		}
	}
	void openPGFImage(CPGFImage* pgf, CPGFFileStream* stream) {
		try {
			pgf->Open(stream);
		} catch (IOException& ex) {
			err = ex.error;
		}
	}
	void copyBitmap(CPGFImage* pgf, uint8_t* buf, int stride, int bpp, int channelMap[]) {
		try {
			pgf->GetBitmap(stride, buf, bpp, channelMap);
		} catch (IOException& ex) {
			err = ex.error;
		}
	}
	void configureDecoder(CPGFImage* pgf, bool useOMP) {
		pgf->ConfigureDecoder(useOMP);
	}
	uint8_t getBpp(CPGFImage* pgf) {
		return pgf->BPP();
	}
	uint8_t getMode(CPGFImage* pgf) {
		return pgf->Mode();
	}
	uint32_t getWidth(CPGFImage* pgf) {
		return pgf->Width();
	}
	uint32_t getHeight(CPGFImage* pgf) {
		return pgf->Height();
	}


	CPGFFileStream* newPGFFileStream(int fd) {
#if defined(WIN32) || defined(WIN64)
		return new CPGFFileStream((HANDLE)_get_osfhandle(fd));
#elif __POSIX__
		return new CPGFFileStream(fd);
#endif
	}
	CPGFFileStream* newPGFFileStream2(FILE* file) {
#if defined(WIN32) || defined(WIN64)
		return new CPGFFileStream((HANDLE)_get_osfhandle(_fileno(file)));
#elif __POSIX__
		return new CPGFFileStream(fileno(file));
#endif
	}
	void deletePGFFileStream(CPGFFileStream* stream) {
		delete stream;
	}
	uint64_t getPos(CPGFFileStream* stream) {
		try {
			return stream->GetPos();
		} catch (IOException& ex) {
			err = ex.error;
			return 0;
		}
	}
	void setPos(CPGFFileStream* stream, int64_t posOff) {
		try {
			stream->SetPos(FSFromStart, posOff);
		} catch (IOException& ex) {
			err = ex.error;
		}
	}

}

