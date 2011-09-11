// RipString.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "RipString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;


static int Table_1[256][256];
static CString strLoadTable_1;

static BOOL SetupTable_1(const char*filename)
{
	if(!strLoadTable_1.CompareNoCase(filename))
	{
		return TRUE;
	}

	strLoadTable_1 = filename;
	memset(Table_1,0,sizeof(Table_1));
	FILE *fp;
	fp=fopen(filename,"r");
	if(fp)
	{
		while(!feof(fp))
		{
			int i;
			unsigned char c1,c2;
			fscanf(fp,"%04X=%c%c",&i,&c1,&c2);
			if(i<256*256)
			{
				Table_1[i/256][i%256] = c1<<8 | c2;
			}
		}	
		fclose(fp);
		return TRUE;
	}
	return FALSE;
}

static void TransTable_1(unsigned char i1, unsigned char j1, unsigned char &i2, unsigned char&j2)
{
	i2 = Table_1[i1][j1] / 256;
	j2 = Table_1[i1][j1] % 256;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		if(argc <= 4)
		{
			CString str = argv[0];
			str = str.Right(str.GetLength() - str.ReverseFind('\\') -1); 
			cerr << "Usage : "<< (LPCTSTR)str << " [e(cho)|l(ocation)|s(hift)|q(uote)] TableFile InputFile OutputFile "<< endl;
		}
		else
		{
			CString strOption = argv[1];
			bool bEcho = false;
			if(strOption.Find("e") != -1)
			{
				bEcho = true;
			}
			
			bool bLocation = false;
			if(strOption.Find("l") != -1)
			{
				bLocation = true;
			}

			bool bShift = false;
			if(strOption.Find("s") != -1)
			{
				bShift = true;
			}

			char separation = '\n';
			if(strOption.Find("q") != -1)
			{
				separation = ',';
			}

			CString TableFileName = argv[2];
			if(!SetupTable_1(TableFileName))	
			{
				printf("Cannot open %s .\n", (LPCTSTR)TableFileName);
				return EXIT_SUCCESS;
			}

			CString strInputFileName = argv[3];
			CString strFileOut = argv[4];

			////////////////////////////////

			FILE *fpin,*fpout;
			bool bUnknown = false;
			long int TotalCharNum = 0;
			long int TransCharNum = 0;


			fpin=fopen(strInputFileName,"rb");
			fpout=fopen(strFileOut,"w+");
			if(fpin && fpout)
			{
				if(bShift)
					fseek(fpin,1,SEEK_SET);

				while(!feof(fpin))
				{
					unsigned char ch[4];
					long loc = ftell(fpin);
					fread(ch,1,1,fpin);
					
					if(feof(fpin))
						break;
					
					TotalCharNum ++;

					{
						fread(&(ch[1]),1,1,fpin);
						ch[2]='\0';
						
						unsigned char str[4];
						TransTable_1(ch[0],ch[1],str[0],str[1]);
						str[2]='\0';
						
						if(str[0]==0 && str[1]==0)
						{
							bUnknown = true;
							//fprintf(fpout,"　");
						}
						else
						{
							if(bUnknown == true || loc == 0)
							{
								fprintf(fpout,"\n");
								bUnknown = false;
								if(bLocation)
									fprintf(fpout,"%ld%c",loc,separation);
									
							}
							else
							{
								if(TransCharNum == 0)
									fprintf(fpout,"%ld%c",loc,separation);
							}

							TransCharNum++;
							fprintf(fpout,"%s",str);
						}
					}

					//if(TotalCharNum%16 == 0)
					//	fprintf(fpout,"\n");

					if(bEcho && ch[0]=='\n')//只在回车时回显
					{
						printf("[%ld/%ld/%.2f%%]%s\r",
							TransCharNum,TotalCharNum,
							(double)TransCharNum/TotalCharNum*100,
							(LPCTSTR)strInputFileName);
					}
				}

				fclose(fpout);
				fclose(fpin);


				if(bEcho)
				{
					printf("[%ld/%ld/%.2f%%]%s ",
						TransCharNum,TotalCharNum,
						(double)TransCharNum/TotalCharNum*100,
						(LPCTSTR)strInputFileName);
				}

				printf("\n");
			}
			else
			{
				printf("OPEN FAIL : %s or %s\n",strInputFileName,strFileOut);
			}
		}
	}

	return nRetCode;
}


