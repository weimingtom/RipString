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
		printf("nMode = Output Format(1:display 2:read 3:both)\n");
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

	FILE *fpIn = fopen(strInputFilename, "r");
	FILE *fpOut = fopen(strOutputFilename, "w+");
	if(!fpIn || !fpOut)
	{
		printf("Read input file or write output file failed\n");
		return 0;
	}

	char strLine[4 * 1024];

	LPTSTR	inString;

	while(fgets(strLine,sizeof(strLine),fpIn))
	{
		//inString = (LPTSTR)strLine;
		{
			int nLen = strlen(strLine) + 1;
			int nwLen = MultiByteToWideChar(CP_ACP, 0, strLine, nLen, NULL, 0); 
			inString = (LPTSTR)malloc(sizeof(WCHAR) * nwLen);     
			MultiByteToWideChar(CP_ACP, 0, strLine, nLen, inString, nwLen); 
			inString[nwLen - 1] = L'\0';
		}
		int i;
		if( m_engine.GetJMorphResult( dwRequest, dwCMode, (LPCTSTR)inString ) == S_OK )
		{
			if( m_engine.m_pmorrslt ) 
			{
				//CStringW	tmpString1, tmpString2, tmpString3, tmpString4, tmpString5, tmpString6;
				LPWSTR tmpString2, tmpString4;
				setlocale(LC_ALL, strSetLocale);
				
				tmpString2 = (LPWSTR)malloc(sizeof(WCHAR) * (m_engine.m_pmorrslt->cchOutput + 1));					
				//tmpString1 = m_engine.m_pmorrslt->pwchOutput;
				//tmpString1.Delete(m_engine.m_pmorrslt->cchOutput,tmpString1.GetLength());
				//tmpString2.Format( L"%s", tmpString1);
				_wcsnset(tmpString2, L'\0', m_engine.m_pmorrslt->cchOutput + 1);
				wcsncpy(tmpString2, m_engine.m_pmorrslt->pwchOutput, m_engine.m_pmorrslt->cchOutput);
				if(nMode == 1)
				{					
					if(nOutMode == 1 || nOutMode == 3) wprintf( (LPCWSTR)tmpString2 );
					if(nOutMode == 2 || nOutMode == 3) fwprintf(fpOut, (LPCWSTR)tmpString2 );
				}
				
				tmpString4 = (LPWSTR)malloc(sizeof(WCHAR) * (m_engine.m_pmorrslt->cchRead + 1));		
				//tmpString3 = m_engine.m_pmorrslt->pwchRead;
				//tmpString3.Delete(m_engine.m_pmorrslt->cchRead,tmpString1.GetLength());
				//tmpString4.Format( L"%s", tmpString3 );
				_wcsnset(tmpString4, L'\0', m_engine.m_pmorrslt->cchRead + 1);
				wcsncpy(tmpString4, m_engine.m_pmorrslt->pwchRead, m_engine.m_pmorrslt->cchRead);
				
				if(nMode == 2)
				{
					if(nOutMode == 1 || nOutMode == 3) wprintf( (LPCWSTR)tmpString4 );
					if(nOutMode == 2 || nOutMode == 3) fwprintf(fpOut,(LPCWSTR)tmpString4 );
				}
				if (tmpString2) free(tmpString2);
				if (tmpString4) free(tmpString4);
			}
		}
		if (inString) free(inString);
	}

	fclose(fpIn);
	fclose(fpOut);
	
	return nRetCode;
}
