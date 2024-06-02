#include "CMyReadFile.h"
CMyReadFile::CMyReadFile(FILE* openedFile, const irrklang::ik_c8* filename)
{
	file = openedFile;
	strcpy_s(this->filename, filename);

	// get file size
	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
}

CMyReadFile::~CMyReadFile()
{
	fclose(file);
}

irrklang::ik_s32 CMyReadFile::read(void* buffer, irrklang::ik_u32 sizeToRead)
{
	printf("CMyReadFile: read %d bytes\n", sizeToRead);
	return (irrklang::ik_s32)fread(buffer, 1, sizeToRead, file);
}

bool CMyReadFile::seek(irrklang::ik_s32 finalPos, bool relativeMovement)
{
	printf("CMyReadFile: seek to position %d\n", finalPos);
	return fseek(file, finalPos, relativeMovement ? SEEK_CUR : SEEK_SET) == 0;
}
