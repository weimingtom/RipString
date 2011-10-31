/**
 *
 * Original code is from 
 * http://nis-lab.is.s.u-tokyo.ac.jp/~koiso/software/lib/BitmapObject.cpp
 * http://nis-lab.is.s.u-tokyo.ac.jp/~koiso/software/software.html
 *
 */

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <math.h>

typedef int WINAPI GetPluginInfoProc(int infono, LPSTR buf, int buflen);
typedef int WINAPI IsSupportedProc(LPSTR filename, DWORD dw);
typedef int WINAPI GetPictureProc(LPSTR buf, long len, unsigned int flag,
	HANDLE *pHBInfo, HANDLE *pHBm,
	FARPROC lpPrgressCallback, long lData);

static BITMAPINFOHEADER m_bmih = {0};
static BITMAPFILEHEADER m_bmfh = {0};
static unsigned char* m_pBits = NULL;

static int GetWidth(void)
{ 
	return m_bmih.biWidth; 
}

static int GetHeight(void) 
{ 
	return abs(m_bmih.biHeight); 
}

static void AllocBits(void)
{
	m_pBits = (unsigned char *)malloc(m_bmih.biSizeImage * sizeof(unsigned char));
}

static void DeleteBits(void)
{
	if (m_pBits != NULL) 
	{
		free(m_pBits);
		m_pBits = NULL;
	}
}

static void ReallocBits(void)
{
	if (m_pBits != NULL) 
	{
		DeleteBits();
	}
	AllocBits();
}

static void CreateFileHeader(void)
{
	int w;
	int h;
	w = GetWidth();
	h = GetHeight();
	m_bmfh.bfType = 0x4d42; // 0x4d42='BM'
	m_bmfh.bfReserved1 = 0;
	m_bmfh.bfReserved2 = 0;
	if (m_bmih.biClrUsed == 0)
	{
		switch (m_bmih.biBitCount) 
		{
		case 1:
			m_bmfh.bfOffBits = sizeof(RGBQUAD) * 2; 
			break;
		
		case 4:
			m_bmfh.bfOffBits = sizeof(RGBQUAD) *  16; 
			break;
		
		case 8: 
			m_bmfh.bfOffBits = sizeof(RGBQUAD) * 256; 
			break;
		
		case 24: 
			m_bmfh.bfOffBits = 0; 
			break;
		
		case 16:
		case 32:
			m_bmfh.bfOffBits = (m_bmih.biCompression == BI_RGB) ? 0 : sizeof(DWORD) * 3;
			break;
		}
	}
	else 
	{
		m_bmfh.bfOffBits = m_bmih.biClrUsed * sizeof(RGBQUAD);
	}
	m_bmfh.bfOffBits += sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	if (m_bmih.biSizeImage == 0) 
	{
		m_bmih.biSizeImage = ((w * m_bmih.biBitCount + 31 ) & ~31) / 8 * h;
	}
	m_bmfh.bfSize = m_bmfh.bfOffBits + m_bmih.biSizeImage;
}

static void SPICreateFileHeader(LPBITMAPINFOHEADER pm_bmih)
{
	unsigned long iOldSize;
	iOldSize = m_bmih.biSizeImage;
	memcpy(&m_bmih, pm_bmih, sizeof(BITMAPINFOHEADER));
	CreateFileHeader();
	if (m_bmih.biSizeImage != iOldSize) 
	{
		ReallocBits();
	}
}

int SPILoadImage(char *strFileName)
{
	FILE *fp;
	int fh;
	int len;
	char *buf;
    struct _finddata_t c_file;
    long hFile;
	char version[10];
	fp = fopen(strFileName, "rb");
	if (fp == NULL) 
	{
		fprintf(stderr, "file '%s' not found.\n", strFileName);
		return -1;
	}
	fh = _fileno(fp);
	len = (((_filelength(fh) - 1) >> 12) + 1) << 12;
	buf = (char *)malloc(len * sizeof(char));
	fread(buf, len, 1, fp);
    if ((hFile = _findfirst("*.spi", &c_file)) != -1L) 
	{
		while (_findnext(hFile, &c_file) == 0) 
		{
			HINSTANCE hModule;
			GetPluginInfoProc* GetPluginInfo;
			int valid;
			hModule = LoadLibrary(c_file.name);
			GetPluginInfo = (GetPluginInfoProc*) GetProcAddress(hModule, "GetPluginInfo");
			valid = GetPluginInfo(0, version, 10);
			if (valid) 
			{
				IsSupportedProc* IsSupported;
				int bSupported;
				IsSupported = (IsSupportedProc*) GetProcAddress(hModule, "IsSupported");
				bSupported = IsSupported ( strFileName, (DWORD) buf );
				printf("%-12s[%s] - %s \n",
					c_file.name, 
					version, 
					(bSupported ? "SUPPORTED" : "NOT supported"));
				if (bSupported) 
				{
					HLOCAL hBm;
					HLOCAL hBInfo;
					int iRet;
					GetPictureProc* GetPicture;
					GetPicture = (GetPictureProc*) GetProcAddress(hModule, "GetPicture");
					iRet = GetPicture(buf, len, 1, &hBInfo, &hBm, NULL, 0);
					if (iRet == 0) 
					{
						LPBITMAPINFOHEADER pm_bmih;
						unsigned char* bm;
						pm_bmih = (LPBITMAPINFOHEADER) LocalLock(hBInfo);
						SPICreateFileHeader(pm_bmih);
						bm = (unsigned char*) LocalLock(hBm);
						memcpy(m_pBits, bm, m_bmih.biSizeImage);
						LocalFree(pm_bmih);
						LocalFree(bm);
					}
					FreeLibrary(hModule);
					break;
				}
			}
			FreeLibrary(hModule);
		}
		_findclose(hFile);
	}
	fclose(fp);
	free(buf);
	return 0;
}

int SPISaveImage(char* strFileName)
{
	FILE *fp;
	
	fp = fopen(strFileName, "wb+");
	if (fp == NULL) 
	{
		return -1;
	}
	fwrite(&m_bmfh, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&m_bmih, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(m_pBits, m_bmih.biSizeImage, 1, fp );
	fclose(fp);
	return 0;
}

void SPIRelease(void)
{
	DeleteBits();
}

int main(int argc, char **argv)
{
	char strInput[MAX_PATH], strOutput[MAX_PATH];
	if (argc != 2 && argc != 3) 
	{
		printf("Usage : %s <input file> or\n"
			"\t%s <input file> [output file]\n", 
			argv[0], argv[0]);
		return 1;
	}
	strcpy(strInput, argv[1]);
	if (argc == 3)
	{
		strcpy(strOutput, argv[2]);
	}
	else
	{
		strcpy(strOutput, strInput);
		strcat(strOutput, ".bmp");
	}
	printf("convert start...\n");	
	if (SPILoadImage(strInput) == 0)
	{
		printf("Load success : %s\n", strInput);
		printf("Try to save bmp file : %s\n", strOutput);
		SPISaveImage(strOutput);
	}
	else
	{
		fprintf(stderr, "Load fail!\n");
		SPIRelease();
		return 2;
	}
	SPIRelease();
	return 0;
}
