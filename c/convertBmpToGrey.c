#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

//=========������ֲwindows=========
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
//===========������ֲwindows=============

int main(int argc, char *argv[])
{
    BITMAPFILEHEADER srcFileHeader; //BMP�ļ�ͷ�ṹ��
    BITMAPINFOHEADER srcInfoHeader; //BMP��Ϣͷ�ṹ��
	printf("size == %d, %d, %d\n", sizeof(BITMAPFILEHEADER), sizeof(BITMAPFILEHEADER), sizeof(RGBQUAD));

    FILE* fp;           //�ļ�ָ��
	long fileLineContainBytes = 0;//ÿһ�е�������
    BYTE **srcImgdata; //24λɫͼ������ָ��
    long i,j;
    char bmpFileName[256];        //ͼ���ļ���

	BITMAPFILEHEADER greyFileHeader;	//�Ҷ�ͼ���ļ�ͷ
	BITMAPINFOHEADER greyInfoHeader;	//�Ҷ�ͼ����Ϣͷ
	RGBQUAD *greyPaletee;		//�Ҷ�ͼ��ĵ�ɫ��
	BYTE *greyImgData;//�Ҷ�ͼ��һά����
	int greyBytesPerLine;
	int greyImageSize;
	int greyLineStart;

    //���ļ�
    printf("�������ļ���:");
    scanf("%s",bmpFileName);
	fflush(stdin);
    if((fp = fopen(bmpFileName, "rb")) == NULL)
	{
        perror("Open file error!");
        exit(0);
    }
#ifdef _DEBUG_MY
	printf("bmpͼ���ļ� %s �򿪳ɹ���\n", bmpFileName);
#endif

    //��ȡ��Ϣͷ���ļ�ͷ
    fread(&srcFileHeader,sizeof(BITMAPFILEHEADER),1,fp); //��ָ��fp��ָ����ļ���ͷ��Ϣд��bf����ַ��
    fread(&srcInfoHeader,sizeof(BITMAPINFOHEADER),1,fp);
#ifdef _DEBUG_MY
	printf("bmpͼ���ļ�ͷ����Ϣͷ��ȡ��ɣ�\n");
	printf("λͼ���ݵ���ʼλ�ã�%d\n", srcFileHeader.bfOffBits);
	printf("λͼ�ļ���СΪ��%d �ֽ�\n",srcFileHeader.bfSize);
	printf("λͼÿ����������Ҫ��λ����%d\n", srcInfoHeader.biBitCount);
	printf("ͼ��ߴ�Ϊ��%d * %d \n", srcInfoHeader.biWidth, srcInfoHeader.biHeight);
	printf("λͼ��СΪ��%d �ֽ�\n", srcInfoHeader.biSizeImage);
	printf("λͼ��Ϣͷ�ṹռ���ֽ�����%d \n", srcInfoHeader.biSize);
#endif

	//Ϊ24λͼ�����ݷ��� ָ������ռ�
   srcImgdata = (BYTE **)malloc((sizeof(BYTE *)) * srcInfoHeader.biHeight);

    if(srcInfoHeader.biBitCount == 24)	//24λɫ
	{
		fileLineContainBytes = (srcInfoHeader.biWidth * 3 + 3)/4 * 4;   //ת��ÿ�е��ֽ�����һ�е��ֽ�����Ӧ����4��������
		for (i = 0; i < srcInfoHeader.biHeight; i++)
		{
			srcImgdata[i] = (BYTE *)malloc(sizeof(BYTE) * fileLineContainBytes);
		}
		for (i = 0; i < srcInfoHeader.biHeight; i++)  //��bmpͼ���λͼ������������ֽڵĶ�����ά������
		{
			for(j = 0; j < fileLineContainBytes; j++)
			{
				fread(&srcImgdata[i][j], sizeof(BYTE), 1, fp);	//ÿ�ζ�ȡͼ���ļ��е�һ���ֽڣ���������
			}
		}
	}
#ifdef _DEBUG_MY
	printf("bmpͼ�� λͼ���ݲ��ֶ�ȡ��ɣ�\n");
#endif
	fclose(fp);   //�ر�Դ�����ļ�


	//����һ���ļ�������д��Ҷ�ͼ��
	if ((fp=fopen(bmpFileName,"wb")) == NULL)
	{
		perror("Open file mybmp.bmp error!");
		exit(0);
	}

#ifdef _DEBUG_MY
	printf("\n�Ҷ�ͼ���ļ� mybmp.bmp �򿪳ɹ���\n");
#endif

	greyBytesPerLine = ( (srcInfoHeader.biWidth+3)/4 ) * 4;     //�Ҷ�ͼÿ���ֽ�����4��������
	greyImageSize = greyBytesPerLine * srcInfoHeader.biHeight;    //����ÿ�е��ֽ�����,���������Ҷ�ͼ��С
	memcpy(&greyInfoHeader, &srcInfoHeader, sizeof(BITMAPINFOHEADER)); //��Դλͼ��Ϣͷֱ�ӿ������Ҷ�ͼ��
	greyInfoHeader.biBitCount = 8;							   //���ûҶ�ͼ�ļ�����λ��
	greyInfoHeader.biSizeImage = greyImageSize;				   //���ûҶ�ͼ��С
	greyFileHeader.bfType = 0x4d42;						   //�Ҷ�ͼ�ļ�����
	greyFileHeader.bfReserved1 = greyFileHeader.bfReserved2 = 0;
	greyFileHeader.bfOffBits = sizeof(greyFileHeader) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
	greyFileHeader.bfSize = greyFileHeader.bfOffBits + greyImageSize;      //����Ҷ�ͼ�ļ���С

	fwrite(&greyFileHeader, sizeof(BITMAPFILEHEADER),1,fp);   //д��Ҷ�ͼ�ļ�ͷ
	fwrite(&greyInfoHeader, sizeof(BITMAPINFOHEADER),1,fp);   //д��Ҷ�ͼ����Ϣͷ

#ifdef _DEBUG_MY
	printf("�Ҷ�ͼ���ļ�ͷ����Ϣͷд����ɣ�\n");
#endif

	//������ɫ�壬��ʼ�� ��д��Ҷ�ͼ���ļ�
	greyPaletee = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));

	for (i = 0; i < 256; i++ )
		greyPaletee[i].rgbRed = greyPaletee[i].rgbGreen = greyPaletee[i].rgbBlue = (BYTE)i;
    fwrite(greyPaletee, sizeof(RGBQUAD), 256, fp);		  //д��Ҷ�ͼ�ĵ�ɫ��

#ifdef _DEBUG_MY
	printf("�Ҷ�ͼ���ɫ��д����ɣ�\n");
#endif

	greyImgData = (BYTE *)malloc(greyImageSize);//���ݻҶ�ͼ��С������Ӧ����
	for (i=0; i < srcInfoHeader.biHeight; i++)
	{
		greyLineStart = greyBytesPerLine * i;      //��ά���鷽ʽ����i�е���λ��
		for ( j = 0; j < greyBytesPerLine; j++ ) //�������Ҷ�ͼ������ֵ
		{
			greyImgData[greyLineStart + j]= (int)( (float)srcImgdata[i][3 * j] * 0.114 + \
				(float)srcImgdata[i][3 * j + 1] * 0.587 + \
				(float)srcImgdata[i][3 * j + 2] * 0.299 );//����Դͼ�����ص㲻rgbֵ�������Ӧ�Ҷ�ͼ���ص�Ҷ�ֵ
		}
	}
    fwrite(greyImgData, greyImageSize, 1, fp);	  //д��Ҷ�ͼ��ͼ������

#ifdef _DEBUG_MY
	printf("�Ҷ�ͼ��д����ɣ�\n");
#endif

	fclose(fp);

	//�����˳�ǰ �ͷ��ڴ�
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
