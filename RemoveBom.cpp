// RemoveBom.cpp : 
//

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <windows.h>
#include <string>

unsigned char Bom[] = {0xef, 0xbb, 0xbf};

void ProcessFile(const char *filePath, int length)
{
	FILE *file = NULL;
	file=fopen(filePath, "rb");
	unsigned char temp[3];
	fread(temp, 3, 1, file);
	if (temp[0] != Bom[0] || temp[1] != Bom[1] || temp[2] != Bom[2])
	{
		fclose(file);
		return;
	}
	unsigned char *buff = new unsigned char[length - 3];
	fread(buff, length - 3, 1, file);
	fclose(file);

	//remove(filePath);

	file=fopen(filePath, "wb+");
	fwrite(buff, length - 3, 1, file);
	fclose(file);
	delete[] buff;
}

void ProcessFolder(const char *folder)
{
	WIN32_FIND_DATA find;
	memset(&find, 0, sizeof(find));
	char filePath[1024];
	sprintf(filePath, "%s\\*.*", folder);
	HANDLE file = FindFirstFile(filePath, &find);
	while (FindNextFile(file, &find))
	{
		if (find.cFileName[0] == '.')
		{
			continue;
		}
		if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			sprintf(filePath, "%s\\%s", folder, find.cFileName);
			ProcessFolder(find.cFileName);
			continue;
		}
		std::string fileStr = find.cFileName;
		if (std::string::npos == fileStr.find(".cpp")
			&& std::string::npos == fileStr.find(".h"))
		{
			continue;
		}
		sprintf(filePath, "%s\\%s", folder, find.cFileName);
		std::cout << filePath << std::endl;
		ProcessFile(filePath, find.nFileSizeLow);
	}
}

int main(int argc, _TCHAR* argv[])
{
	WIN32_FIND_DATA find;
	memset(&find, 0, sizeof(find));
	HANDLE file = FindFirstFile("*.*", &find);
	while (FindNextFile(file, &find))
	{
		if (find.cFileName[0] == '.')
		{
			continue;
		}
		if (find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ProcessFolder(find.cFileName);
			continue;
		}
		std::string fileStr = find.cFileName;
		if (std::string::npos == fileStr.find(".cpp")
			&& std::string::npos == fileStr.find(".h"))
		{
			continue;
		}
		std::cout << find.cFileName << std::endl;
		ProcessFile(find.cFileName, find.nFileSizeLow);
	}
	return 0;
}