// UTF8URL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "UTF8URL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;
using namespace std;

//utf-8 to ansi, simple implement
string UTF8ToANSI(string strUtf)
{
	WCHAR strTemp1[2000] ={0};
	char strANSI[2000] ={0};
	MultiByteToWideChar(CP_UTF8, 0, strUtf.c_str(), (int)strUtf.length(), strTemp1, 100);   
	WideCharToMultiByte(CP_ACP,0, strTemp1, 1000, strANSI, 1000, "",0);
	return strANSI;
}

long fgetlength(FILE *stream)   
{   
    long   filesize=0;   
    long   oldseek=ftell(stream);   
    fseek(stream, 0L, SEEK_END);   
    filesize=ftell(stream);   
    fseek(stream, oldseek, 0);   
    return filesize;   
}   


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	if(argc < 3)
	{
		printf("usage: [input string] [output file]\n");
		return 0;
	}

	string strIn = argv[1]; 
	string strOutFile = argv[2];

	FILE *fpout;
	fpout = fopen(strOutFile.c_str(), "w+");
	if(!fpout) {
		return 0;
	}
	
	WCHAR strTemp1[2000] ={0};
	char strUTF8[2000] ={0};
	MultiByteToWideChar(CP_ACP, 0, strIn.c_str(), -1, strTemp1, 1000);   
	int ret = WideCharToMultiByte(CP_UTF8,0, strTemp1, 1000, strUTF8, 1000, 0, 0);

	//²âÊÔ
	//%E6%B5%8B%E8%AF%95
	for(char *p = strUTF8; *p != 0; ++p) {
		unsigned char i = *p;
		fprintf(fpout, "%%%X%X", i / 16, i % 16); 
	}
	fclose(fpout);
	
	return 0;
}







