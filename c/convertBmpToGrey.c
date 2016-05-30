#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

//=========方便移植windows=========
typedef unsigned char BYTE;
typedef unsigned short WORD;

typedef unsigned long DWORD;
typedef long LONG;

#pragma pack(push,2)
typedef struct tagBITMAPFILEHEADER {
	WORD	bfType;
	DWORD	bfSize;
	WORD	bfReserved1;
	WORD	bfReserved2;
	DWORD	bfOffBits;
} BITMAPFILEHEADER,*LPBITMAPFILEHEADER,*PBITMAPFILEHEADER;
#pragma pack(pop)
typedef struct tagBITMAPINFOHEADER{
	DWORD	biSize;
	LONG	biWidth;
	LONG	biHeight;
	WORD	biPlanes;
	WORD	biBitCount;
	DWORD	biCompression;
	DWORD	biSizeImage;
	LONG	biXPelsPerMeter;
	LONG	biYPelsPerMeter;
	DWORD	biClrUsed;
	DWORD	biClrImportant;
} BITMAPINFOHEADER,*LPBITMAPINFOHEADER,*PBITMAPINFOHEADER;
typedef struct tagRGBQUAD {
	BYTE	rgbBlue;
	BYTE	rgbGreen;
	BYTE	rgbRed;
	BYTE	rgbReserved;
} RGBQUAD,*LPRGBQUAD;
//===========方便移植windows=============

int main(int argc, char *argv[])
{
    BITMAPFILEHEADER srcFileHeader; //BMP文件头结构体
    BITMAPINFOHEADER srcInfoHeader; //BMP信息头结构体
	printf("size == %d, %d, %d\n", sizeof(BITMAPFILEHEADER), sizeof(BITMAPFILEHEADER), sizeof(RGBQUAD));

    FILE* fp;           //文件指针
	long fileLineContainBytes = 0;//每一行的像素数
    BYTE **srcImgdata; //24位色图像数据指针
    long i,j;
    char bmpFileName[256];        //图像文件名

	BITMAPFILEHEADER greyFileHeader;	//灰度图像文件头
	BITMAPINFOHEADER greyInfoHeader;	//灰度图像信息头
	RGBQUAD *greyPaletee;		//灰度图像的调色板
	BYTE *greyImgData;//灰度图像，一维数据
	int greyBytesPerLine;
	int greyImageSize;
	int greyLineStart;

    //打开文件
    printf("请输入文件名:");
    scanf("%s",bmpFileName);
	fflush(stdin);
    if((fp = fopen(bmpFileName, "rb")) == NULL)
	{
        perror("Open file error!");
        exit(0);
    }
#ifdef _DEBUG_MY
	printf("bmp图像文件 %s 打开成功！\n", bmpFileName);
#endif

    //读取信息头、文件头
    fread(&srcFileHeader,sizeof(BITMAPFILEHEADER),1,fp); //把指针fp所指向的文件的头信息写入bf（地址）
    fread(&srcInfoHeader,sizeof(BITMAPINFOHEADER),1,fp);
#ifdef _DEBUG_MY
	printf("bmp图像文件头与信息头读取完成！\n");
	printf("位图数据的起始位置：%d\n", srcFileHeader.bfOffBits);
	printf("位图文件大小为：%d 字节\n",srcFileHeader.bfSize);
	printf("位图每个像素所需要的位数：%d\n", srcInfoHeader.biBitCount);
	printf("图像尺寸为：%d * %d \n", srcInfoHeader.biWidth, srcInfoHeader.biHeight);
	printf("位图大小为：%d 字节\n", srcInfoHeader.biSizeImage);
	printf("位图信息头结构占用字节数：%d \n", srcInfoHeader.biSize);
#endif

	//为24位图像数据分配 指针数组空间
   srcImgdata = (BYTE **)malloc((sizeof(BYTE *)) * srcInfoHeader.biHeight);

    if(srcInfoHeader.biBitCount == 24)	//24位色
	{
		fileLineContainBytes = (srcInfoHeader.biWidth * 3 + 3)/4 * 4;   //转换每行的字节数，一行的字节数，应该是4的整数倍
		for (i = 0; i < srcInfoHeader.biHeight; i++)
		{
			srcImgdata[i] = (BYTE *)malloc(sizeof(BYTE) * fileLineContainBytes);
		}
		for (i = 0; i < srcInfoHeader.biHeight; i++)  //将bmp图像的位图数据内容逐个字节的读到二维数组中
		{
			for(j = 0; j < fileLineContainBytes; j++)
			{
				fread(&srcImgdata[i][j], sizeof(BYTE), 1, fp);	//每次读取图像文件中的一个字节，存入数组
			}
		}
	}
#ifdef _DEBUG_MY
	printf("bmp图像 位图数据部分读取完成！\n");
#endif
	fclose(fp);   //关闭源数据文件


	//打开另一个文件，用于写入灰度图像
	if ((fp=fopen(bmpFileName,"wb")) == NULL)
	{
		perror("Open file mybmp.bmp error!");
		exit(0);
	}

#ifdef _DEBUG_MY
	printf("\n灰度图像文件 mybmp.bmp 打开成功！\n");
#endif

	greyBytesPerLine = ( (srcInfoHeader.biWidth+3)/4 ) * 4;     //灰度图每行字节数，4的整数倍
	greyImageSize = greyBytesPerLine * srcInfoHeader.biHeight;    //根据每行的字节数据,计算整个灰度图大小
	memcpy(&greyInfoHeader, &srcInfoHeader, sizeof(BITMAPINFOHEADER)); //把源位图信息头直接拷贝到灰度图中
	greyInfoHeader.biBitCount = 8;							   //设置灰度图文件像素位数
	greyInfoHeader.biSizeImage = greyImageSize;				   //设置灰度图大小
	greyFileHeader.bfType = 0x4d42;						   //灰度图文件类型
	greyFileHeader.bfReserved1 = greyFileHeader.bfReserved2 = 0;
	greyFileHeader.bfOffBits = sizeof(greyFileHeader) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	greyFileHeader.bfSize = greyFileHeader.bfOffBits + greyImageSize;      //计算灰度图文件大小

	fwrite(&greyFileHeader, sizeof(BITMAPFILEHEADER),1,fp);   //写入灰度图文件头
	fwrite(&greyInfoHeader, sizeof(BITMAPINFOHEADER),1,fp);   //写入灰度图的信息头

#ifdef _DEBUG_MY
	printf("灰度图像文件头与信息头写入完成！\n");
#endif

	//创建调色板，初始化 并写入灰度图像文件
	greyPaletee = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));

	for (i = 0; i < 256; i++ )
		greyPaletee[i].rgbRed = greyPaletee[i].rgbGreen = greyPaletee[i].rgbBlue = (BYTE)i;
    fwrite(greyPaletee, sizeof(RGBQUAD), 256, fp);		  //写入灰度图的调色板

#ifdef _DEBUG_MY
	printf("灰度图像调色板写入完成！\n");
#endif

	greyImgData = (BYTE *)malloc(greyImageSize);//根据灰度图大小创建对应数组
	for (i=0; i < srcInfoHeader.biHeight; i++)
	{
		greyLineStart = greyBytesPerLine * i;      //二维数组方式计算i行的首位置
		for ( j = 0; j < greyBytesPerLine; j++ ) //逐个计算灰度图的像素值
		{
			greyImgData[greyLineStart + j]= (int)( (float)srcImgdata[i][3 * j] * 0.114 + \
				(float)srcImgdata[i][3 * j + 1] * 0.587 + \
				(float)srcImgdata[i][3 * j + 2] * 0.299 );//根据源图像像素点不rgb值，算出对应灰度图像素点灰度值
		}
	}
    fwrite(greyImgData, greyImageSize, 1, fp);	  //写入灰度图的图像数据

#ifdef _DEBUG_MY
	printf("灰度图像写入完成！\n");
#endif

	fclose(fp);

	//程序退出前 释放内存
	if(srcImgdata != NULL)
	{
		for (i=0; i < srcInfoHeader.biHeight; i++)
		{
			if(srcImgdata[i] != NULL)
			{
				free(srcImgdata[i]);
				srcImgdata[i] = NULL;
			}
		}
		free(srcImgdata);
		srcImgdata = NULL;
	}
	if(greyImgData != NULL)
	{
		free(greyImgData);
		greyImgData = NULL;
	}

	return 0;
}
