#include <stdio.h>
#include "zlib.h"

#include <string.h>
#include <stdlib.h>

int AS3_ByteArray_uncompress(const char input_file_name[], const char output_file_name[])
{
    char *compr, *uncompr;
    unsigned long comprLen, uncomprLen;
	FILE *fpin, *fpout;
	int input_file_length = 0;
	int err;

	fpin = fopen(input_file_name, "rb");
	fpout = fopen(output_file_name, "wb+");
	
	if(fpin && fpout)
	{
		fseek(fpin, 0, SEEK_END);
		input_file_length = ftell(fpin);

		fseek(fpin, 0, SEEK_SET);
		if(input_file_length > 0)
		{
			comprLen = input_file_length;
			uncomprLen = input_file_length * 10;

		    compr = (Byte*)calloc((uInt)comprLen, 1);
			uncompr = (Byte*)calloc((uInt)uncomprLen, 1);

			if (compr == Z_NULL || uncompr == Z_NULL) {
				fprintf(stderr, "内存分配失败\n");
				return 1;
			}

			fread(compr, input_file_length, 1, fpin);

			err = uncompress(uncompr, &uncomprLen, compr, comprLen);
			if (err != Z_OK) 
			{ 
				fprintf(stderr, "解压错误\n");
			}
			else
			{
				fwrite(uncompr, uncomprLen, 1, fpout);
			}

			free(compr);
			free(uncompr);
		} 
		else
		{
			fprintf(stderr, "输入文件长度为0");
		}
		
	}
	else
	{
		printf("打开文件出错\n");
	}

	return 0;
}

int main(int argc, char *argv[])
{
	if(argc < 2) 
	{
		printf("参数: 压缩文件, 解压文件\n");
	}
	else
	{
		AS3_ByteArray_uncompress(argv[1], argv[2]);
	}
	return 0;
}

