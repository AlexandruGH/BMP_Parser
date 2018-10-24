#include <stdio.h>
#include <Windows.h>
#include <winnt.h>

typedef struct _BITMAP_FILE_HEADER{
	WORD signature;
	DWORD fileSize;
	WORD reserved1;
	WORD reserved2;
	DWORD lpPixelArray;
}BITMAP_FILE_HEADER, *PBITMAP_FILE_HEADER;

typedef struct pixel {
	BYTE blue;
	BYTE green;
	BYTE red;
}PIXEL, *PPIXEL;


typedef struct _DIB_HEADER {
	DWORD size;
	DWORD width;
	DWORD height;
	WORD nrOfColorPlanes;
	WORD nrOfBitsPerPixel;
	DWORD compressionUsed;
	DWORD sizeOfRawBitmapData;
	DWORD horizontalPixelResolution;
	DWORD verticalPixelResolution;
	DWORD importantColors;
}DIB_HEADER, *PDIB_HEADER;


PBITMAP_FILE_HEADER pFileHeader1;
BITMAP_FILE_HEADER *pFileHeader2;


PDIB_HEADER pDibHeader1;
DIB_HEADER *pDibHeader2;


void abort(char [], HANDLE, HANDLE, BYTE*);

int main(int argc, char *argv[])
{
	char *filePath;
	HANDLE hFile = INVALID_HANDLE_VALUE, hMapping = 0;
	IMAGE_DOS_HEADER *pDos;
	IMAGE_NT_HEADERS *pHeader;
	IMAGE_SECTION_HEADER *sectionHeader;
	PBITMAP_FILE_HEADER imageHeader;
	DWORD dimensiuneFisier;
	BYTE* inceputFisier = 0;
	int i, j;

	if (argc != 2)
	{
		perror("Bad number of command line arguments.\n");
		exit(2);
	}

	filePath = argv[1];

	printf_s("%s\n", filePath);


	if (INVALID_HANDLE_VALUE == (hFile = CreateFileA(filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ , 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0)))
	{
		char message[] = "File can't be open.\n";
		abort(message, hFile, hMapping, inceputFisier);
	}

	if (NULL == (hMapping = CreateFileMappingA(hFile, 0, PAGE_READONLY, 0, 0, 0)))
	{
		char message[] = "File couldn't be mapped.\n";
		abort(message, hFile, hMapping, inceputFisier);
	}

	if (NULL == (inceputFisier = (BYTE*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, 0)))
	{
		char message[] = "Error at creating the view.\n";
		abort(message, hFile, hMapping, inceputFisier);
	}

	dimensiuneFisier = GetFileSize(hFile, 0);


	imageHeader = (PBITMAP_FILE_HEADER)inceputFisier;
	
	PPIXEL arrayPixel = (PPIXEL)((PBYTE)inceputFisier + imageHeader->lpPixelArray);

	
	pDibHeader1 = (PDIB_HEADER)((PBYTE)inceputFisier + 14);

	printf_s("The width of the file is: %d\n", pDibHeader1->width);
	printf_s("The height of the file is: %d\n", pDibHeader1->height);

	for (i = 0; i < pDibHeader1->width; i++)
	{
		for (j = 0; j < pDibHeader1->height; j++)
		{
			((arrayPixel+i) + j * pDibHeader1->width)->red = 255;
		}
	}

	printf_s("Am ajuns aici.\n");
	getchar();
	getchar();

	char message[] = "Success";
	abort(message, hFile, hMapping, inceputFisier);

	getchar();
	getchar();

}

void abort(char *message, HANDLE hFile, HANDLE hMapping, BYTE* inceputFisier)
{
	if (INVALID_HANDLE_VALUE == hFile)
	{
		CloseHandle(hFile);
	}

	if (NULL == hMapping)
	{
		CloseHandle(hMapping);
	}

	if (NULL == inceputFisier)
	{
		UnmapViewOfFile(inceputFisier);
	}

	printf_s("%s\n", message);
	getchar();
	getchar();
	exit(2);
}