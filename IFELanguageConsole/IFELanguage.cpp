#include "IMEEngine.h"
//#include "afxwin.h"
#include "locale.h"
#include <stdio.h>
#include <malloc.h>

using namespace std;
IMEEngine m_engine;

int main(int argc, char* argv[])
{
	int nRetCode = 0;

	DWORD	dwCMode, dwRequest;
	int nMode, nOutMode, nHideEqual;
	char *strSetLocale, *strIMEEngineName, *strInputFilename, *strOutputFilename;  

	if(argc < 10) {
		printf("usage:\n");
		printf("dwCMode = Mode(196608)\n");
		printf("dwRequest = Request(0)\n");
		printf("nMode = Output Format(1:display 2:read)\n");
		printf("nOutMode = Output Mode(1:print 2:file 3:both)\n");
		printf("nHideEqual = hide equal display and read(1:hide 0:show)\n");
		printf("strSetLocale = setlocale option(.ACP)\n");
		printf("strIMEEngineName = IME Engine(MSIME.Japan,MSIME.Japan,MSIME.Taiwan,MSIME.Taiwan.ImeBbo)\n");
		printf("strInputFilename = input file name\n");
		printf("strOutputFilename = output file name\n");
		return 0;
	}else{
		dwCMode = atoi(argv[1]);
		dwRequest = atoi(argv[2]);
		nMode = atoi(argv[3]); 
		nOutMode = atoi(argv[4]);
		nHideEqual = atoi(argv[5]); 
		strSetLocale = argv[6]; 
		strIMEEngineName = argv[7]; 
		strInputFilename = argv[8];
		strOutputFilename = argv[9];
	}

	LPTSTR wstrIMEEngine;
	//wstrIMEEngine = strIMEEngineName;
	{
		int nLen = strlen(strIMEEngineName) + 1;
		int nwLen = MultiByteToWideChar(CP_ACP, 0, strIMEEngineName, nLen, NULL, 0); 
		wstrIMEEngine = (LPTSTR)malloc(sizeof(WCHAR) * nwLen);     
		MultiByteToWideChar(CP_ACP, 0, strIMEEngineName, nLen, wstrIMEEngine, nwLen);
		wstrIMEEngine[nwLen - 1] = L'\0';
	}
	m_engine.SetIMEEngine(wstrIMEEngine);

	FILE *fpIn = fopen(strInputFilename, "rb");
	FILE *fpOut = fopen(strOutputFilename, "wb+");
	if(!fpIn || !fpOut)
	{
		printf("Read input file or write output file failed\n");
		return 0;
	}

	char strLine[4 * 1024];

	LPTSTR	inString;
	int ch1, ch2, ch3;
	ch1 = fgetc(fpIn);
	ch2 = fgetc(fpIn);
	ch3 = fgetc(fpIn);
	if (ch1 == 0xEF && ch2 == 0xBB && ch3 == 0xBF) 
	{
		if(nOutMode == 2 || nOutMode == 3) fprintf(fpOut, "\xEF\xBB\xBF" );
		while(fgets(strLine,sizeof(strLine),fpIn))
		{
			//inString = (LPTSTR)strLine;
			{
				int nLen = strlen(strLine) + 1;
				int nwLen = MultiByteToWideChar(CP_UTF8, 0, strLine, nLen, NULL, 0); 
				inString = (LPTSTR)malloc(sizeof(WCHAR) * nwLen);     
				MultiByteToWideChar(CP_UTF8, 0, strLine, nLen, inString, nwLen); 
				inString[nwLen - 1] = L'\0';
			}
			char *oriansiString;
			char *oriutf8String;
			{
				int nwLen = wcslen(inString) + 1;
				int nLen = WideCharToMultiByte(CP_ACP, 0, inString, nwLen, NULL, 0, NULL, NULL); 
				oriansiString = (char *)malloc(sizeof(char) * nLen);     
				WideCharToMultiByte(CP_ACP, 0, inString, nwLen, oriansiString, nLen, NULL, NULL); 
				oriansiString[nLen - 1] = '\0';
			}
			{
				int nwLen = wcslen(inString) + 1;
				int nLen = WideCharToMultiByte(CP_UTF8, 0, inString, nwLen, NULL, 0, NULL, NULL); 
				oriutf8String = (char *)malloc(sizeof(char) * nLen);     
				WideCharToMultiByte(CP_UTF8, 0, inString, nwLen, oriutf8String, nLen, NULL, NULL); 
				oriutf8String[nLen - 1] = '\0';
			}
			int i;
			if( m_engine.GetJMorphResult( dwRequest, dwCMode, (LPCTSTR)inString ) == S_OK )
			{
				if( m_engine.m_pmorrslt ) 
				{
					//CStringW	tmpString1, tmpString2, tmpString3, tmpString4, tmpString5, tmpString6;
					LPWSTR tmpString4;
					setlocale(LC_ALL, strSetLocale);
					
					tmpString4 = (LPWSTR)malloc(sizeof(WCHAR) * (m_engine.m_pmorrslt->cchRead + 1));		
					//tmpString3 = m_engine.m_pmorrslt->pwchRead;
					//tmpString3.Delete(m_engine.m_pmorrslt->cchRead,tmpString1.GetLength());
					//tmpString4.Format( L"%s", tmpString3 );
					_wcsnset(tmpString4, L'\0', m_engine.m_pmorrslt->cchRead + 1);
					wcsncpy(tmpString4, m_engine.m_pmorrslt->pwchRead, m_engine.m_pmorrslt->cchRead);
					
					if(nMode == 2)
					{
						LPWSTR inStr = tmpString4; //inString;//
						char *ansiString;
						char *utf8String;
						int utf8StringLen;
						{
							int nwLen = wcslen(inStr) + 1;
							int nLen = WideCharToMultiByte(CP_ACP, 0, inStr, nwLen, NULL, 0, NULL, NULL); 
							ansiString = (char *)malloc(sizeof(char) * nLen);     
							WideCharToMultiByte(CP_ACP, 0, inStr, nwLen, ansiString, nLen, NULL, NULL); 
							ansiString[nLen - 1] = '\0';
						}
						{
							int nwLen = wcslen(inStr) + 1;
							int nLen = WideCharToMultiByte(CP_UTF8, 0, inStr, nwLen, NULL, 0, NULL, NULL); 
							utf8String = (char *)malloc(sizeof(char) * nLen);     
							WideCharToMultiByte(CP_UTF8, 0, inStr, nwLen, utf8String, nLen, NULL, NULL); 
							utf8String[nLen - 1] = '\0';
							utf8StringLen = nLen - 1; 
						}
						if (strlen(strLine) >= 3 && strLine[0] == '>' && strLine[1] == ' ' && strLine[2] != '#')
						{
							if(nOutMode == 1 || nOutMode == 3) printf( "%s", ansiString);
							if(nOutMode == 2 || nOutMode == 3) 
							{
								fprintf(fpOut,oriutf8String);
								fprintf(fpOut,"> %s", utf8String+6 );
							}
						}
						else
						{
							if(nOutMode == 1 || nOutMode == 3) printf( oriansiString );
							if(nOutMode == 2 || nOutMode == 3) 
							{
								fprintf(fpOut,oriutf8String);
							}
						}
						if (ansiString) free(ansiString);
						if (utf8String) free(utf8String);
					}
					if (tmpString4) free(tmpString4);
				}
			} 
			else
			{
				printf( oriansiString );
			}
			if (inString) free(inString);
			if (oriansiString) free(oriansiString);
			if (oriutf8String) free(oriutf8String);
		}
	}
	else
	{
printf("Input file <%s> is not UTF8, 0x%X,0x%X,0x%X\n", strInputFilename, ch1, ch2, ch3);		
	}
	fclose(fpIn);
	fclose(fpOut);
	
	return nRetCode;
}
