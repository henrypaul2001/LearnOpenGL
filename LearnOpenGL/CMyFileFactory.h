#pragma once
#include "irrKlang/irrKlang.h"
#include <iostream>
#include "CMyReadFile.h"
class CMyFileFactory : public irrklang::IFileFactory
{
public:
	virtual irrklang::IFileReader* createFileReader(const irrklang::ik_c8* filename) {
		std::cout << "MyFileFactory: open file %s\n" << filename << std::endl;

		FILE* file;
		fopen_s(&file, filename, "rb");
		if (!file) {
			return 0;
		}

		return new CMyReadFile(file, filename);
	}
private:
};