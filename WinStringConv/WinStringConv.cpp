// WinStringConv.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdlib.h>
int main(int argc, char* argv[])
{
	if(argc < 7)
	{
		printf("Usage: WinStringConv.exe [e(cho)|u(nicode head add)|(unicode) b(ig endian)|j(ump 2 bytes)|] "
			"[MultiByteToWideChar|WideCharToMultiByte|LCMapString] OptionFlag OptionNumber Inputfile Outputfile\n");
		return 0;
	}

	const char *szOptionString = argv[1];
	const char *szFunctionString = argv[2];
	const char *szOptionFlag = argv[3];
	const char *szOptionNumber = argv[4];  // code page
	const char *szInputfile = argv[5];
	const char *szOutputfile = argv[6];

	DWORD dwFlags = atoi(szOptionFlag);
	UINT uiCodePage = (UINT)atoi(szOptionNumber);

	//选项
	bool bEcho = false;
	if(strchr(szOptionString, 'e'))
	{
		bEcho = true;
	}
	bool bUnicodeHeadAdd = false;
	if(strchr(szOptionString, 'u'))
	{
		bUnicodeHeadAdd = true;
	}
	bool bUnicodeBigHeadAdd = false;
	if(strchr(szOptionString, 'b'))
	{
		bUnicodeBigHeadAdd = true;
	}
	bool bJump2bytes = false;
	if(strchr(szOptionString, 'j'))
	{
		bJump2bytes = true;
	}


	//打开输入文件
	HANDLE hFileIn = NULL; 
	hFileIn = ::CreateFile(szInputfile, GENERIC_READ, FILE_SHARE_READ, NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFileIn == INVALID_HANDLE_VALUE) 
	{ 
		printf("Error : Could not open input file.\n");
		return 1;
	} 

	//输入文件大小
	DWORD dwSizeLow=0;
	DWORD dwSizeHigh=0;
	dwSizeLow = GetFileSize (hFileIn, &dwSizeHigh) ; 
	if (dwSizeLow == INVALID_FILE_SIZE && GetLastError() != NO_ERROR)
	{ 
		printf("Error : Could not get input file size.\n");
		return 1;
    } 
	CP_ACP;
	//分配输入内存
	HGLOBAL	hGlobalIn = NULL;
	void *bitsIn = NULL;
	long bitsAllocIn = dwSizeLow;
	if ((hGlobalIn = ::GlobalAlloc(GMEM_MOVEABLE, bitsAllocIn)) == 0)
	{
		printf("Error : GlobalAlloc Fail"); 
	    return 1;
	}
	bitsIn = (void *)::GlobalLock(hGlobalIn);

	//读入文件内容到内存
	DWORD  dwBytesRead=0;
    if (!ReadFile(hFileIn, bitsIn, bitsAllocIn, &dwBytesRead, NULL)) 
    { 
		printf("Error : ReadFile Fail"); 
	    return 1;
    } 

	//跳跃2字节
	if(bJump2bytes)
	{
		if(bitsAllocIn > 2 && dwBytesRead > 2)
		{
			bitsIn = (void *)((char *)bitsIn + 2);
			bitsAllocIn -= 2;
		}
		else
		{
			printf("Error : Could not jump 2 bytes"); 
			return 1;
		}
	}

	//分配输出内存
	HGLOBAL	hGlobalOut = NULL;
	void *bitsOut = NULL;
	long bitsAllocOut = 0;

	//转换分支
	if(!stricmp(szFunctionString, "MultiByteToWideChar"))
	{
		bitsAllocOut = MultiByteToWideChar (uiCodePage, dwFlags, (LPCSTR)bitsIn, dwBytesRead, NULL, 0) * sizeof(WCHAR);
		if ((hGlobalOut = ::GlobalAlloc(GMEM_MOVEABLE, bitsAllocOut)) == 0)
		{
			printf("Error : GlobalAlloc Fail"); 
			return 1;
		}
		bitsOut = (void *)::GlobalLock(hGlobalOut);
		bitsAllocOut = MultiByteToWideChar (uiCodePage, dwFlags, (LPCSTR)bitsIn, dwBytesRead, (LPWSTR)bitsOut, bitsAllocOut);
		if(bEcho)
		{
			printf("[%s][%s/%s][%ld/%ld]\n", szFunctionString, szInputfile, szOutputfile, dwBytesRead, bitsAllocOut); 
		}
	}

	else if(!stricmp(szFunctionString, "WideCharToMultiByte"))
	{
		bitsAllocOut = WideCharToMultiByte (uiCodePage, dwFlags, (LPCWSTR)bitsIn, dwBytesRead/sizeof(LPCWSTR), NULL, 0, NULL, NULL);
		if ((hGlobalOut = ::GlobalAlloc(GMEM_MOVEABLE, bitsAllocOut)) == 0)
		{
			printf("Error : GlobalAlloc Fail"); 
			return 1;
		}
		bitsOut = (void *)::GlobalLock(hGlobalOut);
		bitsAllocOut = WideCharToMultiByte (uiCodePage, dwFlags, (LPCWSTR)bitsIn, dwBytesRead/sizeof(LPCWSTR), (LPSTR)bitsOut, bitsAllocOut, NULL, NULL);
		if(bEcho)
		{
			printf("[%s][%s/%s][%ld/%ld]\n", szFunctionString, szInputfile, szOutputfile, dwBytesRead, bitsAllocOut); 
		}		
	}
	else if(!stricmp(szFunctionString, "LCMapString"))
	{
		LCMAP_TRADITIONAL_CHINESE;

		bitsAllocOut = LCMapString (uiCodePage, dwFlags, (LPCSTR)bitsIn, dwBytesRead, NULL, 0);
		if ((hGlobalOut = ::GlobalAlloc(GMEM_MOVEABLE, bitsAllocOut)) == 0)
		{
			printf("Error : GlobalAlloc Fail"); 
			return 1;
		}
		bitsOut = (void *)::GlobalLock(hGlobalOut);
		bitsAllocOut = LCMapString (uiCodePage, dwFlags, (LPCSTR)bitsIn, dwBytesRead, (LPSTR)bitsOut, bitsAllocOut);
		if(bEcho)
		{
			printf("[%s][%s/%s][%ld/%ld]\n", szFunctionString, szInputfile, szOutputfile, dwBytesRead, bitsAllocOut); 
		}		
	}
	else
	{
		printf("Error : No Function is Found!\n");
		return 1;
	}

	//打开输出文件
	HANDLE hFileOut = NULL; 
	hFileOut = CreateFile(szOutputfile,GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFileOut == INVALID_HANDLE_VALUE) 
	{ 
        printf("Error : Could not open output file.\n");
		return 1;
	} 

	//unicode文件头部标志
	DWORD dwBytesWritten=0;
	if(bUnicodeHeadAdd)
	{
		if(!WriteFile(hFileOut, "\xFF\xFE", 2, &dwBytesWritten, NULL))
		{
			printf("Error : WriteFile Fail"); 
			return 1;
		}
	}
	else if(bUnicodeBigHeadAdd)
	{
		if(!WriteFile(hFileOut, "\xFE\xFF", 2, &dwBytesWritten, NULL))
		{
			printf("Error : WriteFile Fail"); 
			return 1;
		}
	}

	//写文件
	if(!WriteFile(hFileOut, bitsOut, bitsAllocOut, &dwBytesWritten, NULL))
	{
		printf("Error : WriteFile Fail"); 
	    return 1;
	}

	if (hGlobalIn)
	{
		::GlobalUnlock(hGlobalIn);
		::GlobalFree(hGlobalIn);
	}
	if (hGlobalOut)
	{
		::GlobalUnlock(hGlobalOut);
		::GlobalFree(hGlobalOut);
	}
	if (hFileIn) 
	{
		::CloseHandle(hFileIn);
	}
	if (hFileOut) 
	{
		::CloseHandle(hFileOut);
	}

	//printf("Hello World!\n");
	return 0;
}


