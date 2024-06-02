#pragma once
#include "irrKlang/irrKlang.h"
#include <iostream>
class CMyReadFile : public irrklang::IFileReader
{
public:
	CMyReadFile(FILE* openedFile, const irrklang::ik_c8* filename);
	~CMyReadFile();

	irrklang::ik_s32 read(void* buffer, irrklang::ik_u32 sizeToRead);
	bool seek(irrklang::ik_s32 finalPos, bool relativeMovement);
	irrklang::ik_s32 getSize() override { return fileSize; }
	irrklang::ik_s32 getPos() override { return ftell(file); }
	const irrklang::ik_c8* getFileName() override { return filename; }

private:
	FILE* file;
	char filename[1024];
	irrklang::ik_s32 fileSize;
};