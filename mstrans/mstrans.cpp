// mstrans.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
	FILE *fpin,*fpout;
	fpin = fopen("input.txt", "r");
	fpout = fopen("output.txt", "w+");
	if(!fpin || !fpout)
	{
		printf("cannot open input.txt or output.txt !\n");
		return 1;
	}

	while(!feof(fpin))
	{
		char line[300] = {0};
		fgets(line, 300, fpin);
		//printf("%s", line);
		int i;
		//printf("%s", line);
		for(i=0; i < strlen(line) - 1; i+=3)
		{
			int d;
			sscanf(line+i, "%02X", &d);
			fprintf(fpout, "%c", (char)d);
			//printf("%d,%02X ", i,d);
		}
		//printf("\n");
	}
	
	fclose(fpin);
	fclose(fpout);
	return 0;
}
