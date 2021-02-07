//#include <Windows.h>
#include "PGFimage.h"
#include "libpgf.h"

extern "C" {
	CPGFImage* newPGFImage() {
		return new CPGFImage(); 
	}
	void deletePGFImage(CPGFImage* pgf) {
		delete pgf; 
	}
	void readPGFImage(CPGFImage* pgf) {
		pgf->Read(); 
	}
	void openPGFImage(CPGFImage* pgf, CPGFFileStream* stream) {
		pgf->Open(stream);
	}
	void copyBitmap(CPGFImage* pgf, uint8_t* buf, size_t bytes, int stride, int bpp, int channelMap[]) {
		pgf->GetBitmap(stride, buf, bpp, channelMap);
	}
	void configureDecoder(CPGFImage* pgf, bool useOMP) {
		pgf->ConfigureDecoder(useOMP);
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



	CPGFFileStream* newPGFFileStream(HANDLE fd) {
		return new CPGFFileStream(fd);
	}
	void deletePGFFileStream(CPGFFileStream* stream) {
		delete stream;
	}
	uint64_t getPos(CPGFFileStream* stream) {
		return stream->GetPos();
	}
	void setPos(CPGFFileStream* stream, short posMode, int64_t posOff) {
		stream->SetPos(posMode, posOff);
	}

}

