#include <Windows.h>
#include <winnt.h>
#include <stdio.h>

#pragma pack(1)

typedef struct _BMP_HEADER {
	WORD name;
	DWORD fileSize;
	WORD reservedOne;
	WORD reservedTwo;
	DWORD lpPixelArray;
} BITMAP_FILE_HEADER, *PBITMAP_FILE_HEADER;


typedef struct _DIB_HEADER {
	DWORD sizeOfDIBHeader;
	DWORD imageWidth;
	DWORD imageHeight;
	WORD numberOfColorPlanes;
	WORD numberOfBitsPerPixel;
	DWORD compressionUsed;
	DWORD sizeOfRawBitmapData;
	DWORD horizontalResolution;
	DWORD verticalResolution;
	DWORD numberOfColorsInThePalette;
	DWORD importantColors;
}DIB_FILE_HEADER, *PDIB_FILE_HEADER;

typedef struct _PIXEL {
	BYTE blue;
	BYTE green;
	BYTE red;
}PIXEL, *PPIXEL;



void abort(char *message, HANDLE hFile, HANDLE hMapping, PBYTE fileStart)
{
	if (NULL == fileStart)
	{
		UnmapViewOfFile(fileStart);
		fileStart = NULL;
	}

	if (NULL == hMapping)
	{
		CloseHandle(hMapping);
		hMapping = NULL;
	}

	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}

	printf_s("%s\n", message);
	getchar();
	getchar();
	exit(2);
}

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		perror("Bad number of command line arguments.\n");
		exit(2);
	}

	PBITMAP_FILE_HEADER pFileHeader=NULL;
	BITMAP_FILE_HEADER fileHeader;
	PDIB_FILE_HEADER pDibFileHeader=NULL;
	DIB_FILE_HEADER dibFileHeader;
	HANDLE hFile, hMapping;
	PBYTE fileStart;
	char *filePath;
	DWORD fileDimension = 0;
	int i, j;

	hFile = INVALID_HANDLE_VALUE;
	hMapping = 0;
	fileStart = NULL;
	filePath = argv[1];

	printf("File to parse is: %s \n", filePath);

	hFile = CreateFileA(filePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		char message[] = "File can't be opened.";
		abort(message, hFile, hMapping, fileStart);
	}

	hMapping = CreateFileMappingA(hFile,
		0,
		PAGE_READONLY,
		0,
		0,
		0);

	if (NULL == hMapping)
	{
		char message[] = "File couldn't be mapped.";
		abort(message, hFile, hMapping, fileStart);
	}

	fileStart = (PBYTE)MapViewOfFile(hMapping,
		FILE_MAP_READ,
		0,
		0,
		0);

	if (NULL == fileStart)
	{
		char message[] = "Error at creating the view.";
		abort(message, hFile, hMapping, fileStart);
	}

	fileDimension = GetFileSize(hFile, 0);

	printf_s("The dimension of the file is: %d B.\n", fileDimension);
	

	if (fileDimension < sizeof(BITMAP_FILE_HEADER))
	{
		perror("File is too short.\n");
		exit(2);
	}

	pFileHeader = (PBITMAP_FILE_HEADER)fileStart;


	printf_s("FIle name is: %x\n", pFileHeader->name);
	printf_s("FIle size is: %x\n", pFileHeader->fileSize);
	
	pDibFileHeader = (PDIB_FILE_HEADER)(PBYTE)(fileStart + sizeof(BITMAP_FILE_HEADER));
	printf_s("Width of the image is: %d\n", pDibFileHeader->imageWidth);
	printf_s("Height of the image is: %d\n", pDibFileHeader->imageHeight);





	getchar();
	getchar();
	return 0;
}