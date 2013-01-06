


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>

unsigned char Bom[] = {0xef, 0xbb, 0xbf};

void ProcessFileAddBom(const char *filePath, int length)
{
	FILE *file = NULL;
	file=fopen(filePath, "rb");
	unsigned char *buff = new unsigned char[length];
	fread(buff, length, 1, file);
	fclose(file);

	if (buff[0] == Bom[0] && buff[1] == Bom[1] && buff[2] == Bom[2])
	{
		delete []buff;
		return;
	}
	std::cout << filePath << std::endl;

	file=fopen(filePath, "wb");
	fwrite(Bom, 3, 1, file);
	fwrite(buff, length, 1, file);
	fclose(file);
	delete[] buff;
}


void ProcessFileRemoveBom(const char *filePath, int length)
{
	FILE *file = NULL;
	file=fopen(filePath, "rb");
	unsigned char temp[3]={'\0','\0','\0'};
	fread(temp, 3, 1, file);
	if (temp[0] != Bom[0] || temp[1] != Bom[1] || temp[2] != Bom[2])
	{
		fclose(file);
		return;
	}
	std::cout << filePath << std::endl;
	unsigned char *buff = new unsigned char[length - 3];
	fread(buff, length - 3, 1, file);
	fclose(file);

	//remove(filePath);

	file=fopen(filePath, "wb+");
	fwrite(buff, length - 3, 1, file);
	fclose(file);
	delete[] buff;
}


void ProcessFolder(const char *folder,bool isAddBom)
{
	WIN32_FIND_DATA find;
	memset(&find, 0, sizeof(find));
	char* filePath=new char[1024];
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
			char* newPath=new char[1024];
			sprintf(newPath, "%s\\%s", folder, find.cFileName);
			ProcessFolder(newPath,isAddBom);
			continue;
		}
		std::string fileStr = find.cFileName;
		if (std::string::npos == fileStr.find(".cpp")
			&& std::string::npos == fileStr.find(".h")
			&& std::string::npos == fileStr.find(".inl")
			&& std::string::npos == fileStr.find(".inc"))
		{
			continue;
		}
		sprintf(filePath, "%s\\%s", folder, find.cFileName);
		
		if(isAddBom)
		{
			ProcessFileAddBom(filePath, find.nFileSizeLow);
		}
		else
		{
			ProcessFileRemoveBom(filePath, find.nFileSizeLow);
		}
		
	}
	delete []filePath;
}

int main(int argc, _TCHAR* argv[])
{
	if(argc<3){
		printf("usage:\nSwitchBom path a[r]\n");
		return -1;
	}
	
	ProcessFolder(argv[1],argv[2][0]=='a');
	return 0;
}