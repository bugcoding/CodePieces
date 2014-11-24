//@author aerror 
//2011/12/02
#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <list>


extern "C"{
#include "png.h"
}


	
//重置容器大小至ds
#define EnsureVectorSize(v,ds)  {if(v.size()<(unsigned int)ds){ v.resize(ds);}}

//矩形区域，上下左右
typedef struct _RECTANGLE
{
	int top;
	int left;
	int bottom;
	int right;
}TRECTANGLE;



//针对单个png文件，包括简单的png属性，源长宽，文件名字，png源数据指针
class PNG_SRC_FILE
{
public:
	std::string filename;
	png_bytep   bmp;
	unsigned int rawWidth  ;
	unsigned int rawHeight  ;
	PNG_SRC_FILE(const char * src)
	{
		filename =src;
		bmp = NULL;
	}
	virtual ~PNG_SRC_FILE()
	{
		if(bmp!=NULL)
		{
			delete []bmp;
		}
	}

};

//持有单个png文件对象的列表
typedef std::list<  PNG_SRC_FILE *> TPNG_SRC_FILE_LIST;
//迭代png文件对象的迭代器
typedef std::list<   PNG_SRC_FILE *>::iterator TPNG_SRC_FILE_LIST_ITER;

//按文件名排序
bool filename_sort(PNG_SRC_FILE * x1,PNG_SRC_FILE * x2)
{
	return x1->filename < x2->filename;
}

//每个动画的属性，包括的文件，帧长宽，所在目录，动画的名字，总帧数，帖速率，这个大图最大长宽
typedef struct _tanimation_info
{

	const char *actionName;//动画名称
	const char *actionDir;//动画所在目录
	unsigned rowHeight;//列高
	unsigned frameWidth;//帧宽
	unsigned frameHeight;//帧高
	unsigned totalFrame;//总帧数
	unsigned fps;//帧速率
	TPNG_SRC_FILE_LIST  * filelist;//这个动画对应的png对象列表
    //打出来的大图对应的区域，上下左右
	unsigned int minTop     ;
	unsigned int maxBottom  ;
	unsigned int minLeft    ;
	unsigned int maxRight   ;

}TANIMATION_INFO;

    //生成的png大图的源缓冲区
	std::vector<BYTE>				s_srcDibBitsBuffer;
    //将要复制到的缓冲区，即要生成的png文件的缓冲区
	std::vector<BYTE>				s_dstDibBitsBuffer;
	std::vector<BYTE>				s_dstPngRowsBuffer;
	std::vector<BYTE>				s_dstPngTempBuffer;
	
	

	int outputToPngFileNormal(const char *szFileName,unsigned int numDibBtis,const unsigned char *pDibBytes,unsigned int _width, unsigned int _height)
	{
		FILE *fp;
		png_structp png_ptr;
		png_infop info_ptr;
	

		/* open the file */
		fp = fopen(szFileName, "wb");
		if (fp == NULL)
		  return (ERROR);

		png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);

		if (png_ptr == NULL)
		{
		  fclose(fp);
		  return (ERROR);
		}

		/* Allocate/initialize the image information data.  REQUIRED */
		info_ptr = png_create_info_struct(png_ptr);
		if (info_ptr == NULL)
		{
		  fclose(fp);
		  png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
		  return (ERROR);
		}

		/* Set error handling.  REQUIRED if you aren't supplying your own
		* error handling functions in the png_create_write_struct() call.
		*/
		if (setjmp(png_jmpbuf(png_ptr)))
		{
		  /* If we get here, we had a problem reading the file */
		  fclose(fp);
		  png_destroy_write_struct(&png_ptr, &info_ptr);
		  return (ERROR);
		}

		png_init_io(png_ptr, fp);

		
		int src_bytes_per_pixel	= numDibBtis/_width/_height;	
		png_uint_32 k, height, width;

		height = _height;	
		width = _width;


		png_set_IHDR(png_ptr, info_ptr, width, height, 8, 
		   PNG_COLOR_TYPE_RGB_ALPHA,
		  PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		/* Write the file header information.  REQUIRED */
		png_write_info(png_ptr, info_ptr);

		if (height > PNG_UINT_32_MAX/png_sizeof(png_bytep))
		 png_error (png_ptr, "Image is too tall to process in memory");


		png_bytep image =  NULL;
		png_bytep imagerow =  NULL;
		
		
		EnsureVectorSize(s_dstPngRowsBuffer,width*4);
		imagerow =	&s_dstPngRowsBuffer[0];

		for(int y=0;y<_height;y++)
		{
			for(int x=0;x<_width;x++)
			{
				int dx = x;//+pInfo->left;
				int dy = y;//+pInfo->top;

				if(dx>width || dy>height)
				{
					printf("invalid offset operation\n");
					return ERROR;
				}

				const unsigned char* psrc = pDibBytes + y*_width*src_bytes_per_pixel+ x*src_bytes_per_pixel;
				png_bytep pdst = NULL;
				
				pdst = imagerow  +dx*4;
				
				if(src_bytes_per_pixel==4)
				{
					pdst[0] = psrc[0];
					pdst[1] = psrc[1];
					pdst[2] = psrc[2];
					pdst[3] = psrc[3];
				}
			
			}
			
			png_write_row(png_ptr,imagerow);

		}
		
		png_write_end(png_ptr, info_ptr);

		png_destroy_write_struct(&png_ptr, &info_ptr);

		fclose(fp);

		return 0;
	}


//根据每个对应的png源数据数组，源长宽，算出一组png对象的上下左右边界
TRECTANGLE cutTransparent(png_bytep bmp, unsigned int width, unsigned int height )
{
	TRECTANGLE  t={0,0,height,width};

	//row by row from top
	//
	for(int y=0;y<height;y++)
	{
		bool thisLineEmpty = true;
		for(int x=0;x<width;x++)
		{
			unsigned char* psrc = bmp + y*width*4+ x*4;
			unsigned  int v =  *(unsigned int* )psrc;
			if(v &0xFF000000)
			{
				thisLineEmpty = false;
				break;
			}
		}

		if(thisLineEmpty)
		{
			t.top ++ ;
		}
		else
		{
			break;
		}
	}

	for(int y=height-1;y>=0;y--)
	{
		bool thisLineEmpty = true;
		for(int x=0;x<width;x++)
		{
			unsigned char* psrc = bmp + y*width*4+ x*4;
			unsigned  int v =  *(unsigned int* )psrc;
			if(v &0xFF000000)
			{
				thisLineEmpty = false;
				break;
			}
		}

		if(thisLineEmpty)
		{
			t.bottom --;
		}
		else
		{
			break;
		}
	}

	
	for(int x=0;x<width;x++)
	{
		bool thisLineEmpty = true;
		for(int y=0;y<height;y++)
		{
			unsigned char* psrc = bmp + y*width*4+ x*4;
			unsigned  int v =  *(unsigned int* )psrc;
			if(v &0xFF000000)
			{
				thisLineEmpty = false;
				break;
			}
		}

		if(thisLineEmpty)
		{
			t.left ++ ;
		}
		else
		{
			break;
		}
	}


	for(int x=width-1;x>=0;x--)
	{
		bool thisLineEmpty = true;
		for(int y=0;y<height;y++)
		{
			unsigned char* psrc = bmp + y*width*4+ x*4;
			unsigned  int v =  *(unsigned int* )psrc;
			if(v &0xFF000000)
			{
				thisLineEmpty = false;
				break;
			}
		}

		if(thisLineEmpty)
		{
			t.right --;
		}
		else
		{
			break;
		}
	}


	return t;
}

//进行拼合操作，将要打包的目录，要输出的目录
void concat(const char *packsDir, const char *outputDir)
{
    //遍历文件夹
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[MAX_PATH];  // directory specification
	char DirBase[MAX_PATH];  // directory specification
	char filename_dest[MAX_PATH];
	char npc_name[MAX_PATH];

	DWORD dwError;


	TANIMATION_INFO aniInfo[4]={
		{ "stand","standby",0,0,0,0,0,0,0,0,0,0},
		{ "attack","Normal attack",0,0,0,0,0,0,0,0,0,0},
		{ "squat","Morale attack",0,0,0,0,0,0,0,0,0,0},
		{ "attacked","under attack",0,0,0,0,0,0,0,0,0,0}
	};

	
	strcpy(DirSpec,packsDir);
	int rlen = strlen(DirSpec);
	if(DirSpec[rlen -1]=='\\')
	{
		DirSpec[rlen-1]='\0';
	}
	strcpy(DirBase,DirSpec);
	char *last = strrchr(DirBase,'\\');
	strcpy(npc_name,last+1);


	char szCurPath[MAX_PATH];


	GetCurrentDirectoryA(MAX_PATH,szCurPath);
	
	unsigned int currentRowHeight = 0;
	unsigned int output_width=0;
	unsigned int output_height=0;
	for(int n=0;n<4;n++)
	{
	
		strcpy(DirSpec,DirBase);
		strcat(DirSpec,"\\序列\\");
		strcat(DirSpec,aniInfo[n].actionDir);
		SetCurrentDirectoryA(DirSpec);
		strcat(DirSpec,"\\*.png");
		
		aniInfo[n].filelist = new TPNG_SRC_FILE_LIST();
		TPNG_SRC_FILE_LIST &filelist = *aniInfo[n].filelist;

		hFind = FindFirstFileA(DirSpec, &FindFileData);
		if (hFind != INVALID_HANDLE_VALUE) 
		{
			while (FindNextFileA(hFind, &FindFileData) != 0) 
			{
				filelist.push_back(new PNG_SRC_FILE(FindFileData.cFileName));
			}
			dwError = GetLastError();
			FindClose(hFind);
		}

		if(filelist.empty())
		{
			continue;
		}

        //针对每组数据，每组png对象，按文件名排序
		filelist.sort(filename_sort);


		//剪切
		//四个文件，确定这组png对象所占的最小区域，（在一张大图中）
		unsigned int minTop    =0 ;
		unsigned int maxBottom =0 ;
		unsigned int minLeft   =0 ;
		unsigned int maxRight  =0 ;

        //遍历每组png对象
		for(TPNG_SRC_FILE_LIST_ITER  iter = filelist.begin(); 
			iter !=filelist.end(); iter++)
		{
		
            //临时放置png数据变量
			png_structp png_ptr=NULL;
			png_infop info_ptr=NULL;
			png_infop end_info_ptr=NULL;

            //读取单个的png文件获取png源数据
			FILE *fp = fopen((*iter)->filename.c_str(), "rb");
			if (fp == NULL)
				return ;
            //libpng的数据初始化
			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL, NULL, NULL);
			info_ptr = png_create_info_struct(png_ptr);
			end_info_ptr = png_create_info_struct(png_ptr);
			png_init_io(png_ptr, fp);
            //读取png源数据
			png_read_info(png_ptr,info_ptr);

            //源数据数组指针
			png_bytep dibbytes = new png_byte[info_ptr->width * info_ptr->height * 4];
			
            //交给统一的png对象内部数据管理
			(*iter)->bmp = dibbytes ;
            //长宽
			(*iter)->rawWidth = info_ptr->width;
			(*iter)->rawHeight = info_ptr->height;
			
            //扫描每行，将每行数据写入数据数组中	
			for(int y=0;y<info_ptr->height;y++)
			{
				png_bytep pdest = dibbytes + y*info_ptr->width*4;
				png_read_row(png_ptr,pdest,NULL);
			}

            //根据读取到的单个png文件源数据，得到最小的区域
			TRECTANGLE cut = cutTransparent(dibbytes,info_ptr->width,info_ptr->height);
            //确保是当前的文件的小区域
			if(minTop==0 || minTop> cut.top)
			{
				minTop = cut.top;
			}
			if(minLeft==0 || minLeft> cut.left)
			{
				minLeft = cut.left;
			}

			if(maxRight < cut.right)
			{
				maxRight = cut.right;
			}
			
			if(maxBottom < cut.bottom)
			{
				maxBottom  = cut.bottom;
			}
			
            //销毁对应的数据
			png_destroy_read_struct(&png_ptr,&info_ptr,&end_info_ptr);
            //关闭文件
			fclose(fp);
		}


		//定义当前这组动画的帧率
		aniInfo[n].fps = 10;
        //根据算出的最小区域得到要拼合的长宽
		aniInfo[n].frameWidth = maxRight - minLeft;
		aniInfo[n].frameHeight  = maxBottom-minTop;
        //帧数，也就是这一组动画中文件的总数
        aniInfo[n].totalFrame= filelist.size();
		aniInfo[n].rowHeight = currentRowHeight;

        //计算出的区域数据，放到png动画对象的（大图）的源数据中
		aniInfo[n].minTop     =minTop     ;
		aniInfo[n].maxBottom  =maxBottom  ;
		aniInfo[n].minLeft    =minLeft    ;
		aniInfo[n].maxRight   =maxRight   ;

		currentRowHeight += aniInfo[n].frameHeight;

        //保证输出宽度是足够的
		if(output_width  < aniInfo[n].frameWidth * aniInfo[n].totalFrame)
		{
			output_width   = aniInfo[n].frameWidth * aniInfo[n].totalFrame;
		}

        //大图的累计高度，根据
		output_height = currentRowHeight;
	}
	
    //保证输出缓冲区足够
	EnsureVectorSize(s_srcDibBitsBuffer, output_height * output_width * 4);

    //获取操作输出缓冲区的指针
	png_bytep  bigmap = &s_srcDibBitsBuffer[0];
    //重置临时数据
	currentRowHeight = 0;
	
    
    //生成合并数据相关，输出xml文件
	std::string xml;
	const char *headline_fmt =  "<SWFLoader isBattle=\"true\" name=\"%s\" url=\"assets/soldiers/%s.swf\" load=\"false\">\n";
	const char *actionline_fmt =  "  <action name=\"%s\" rowHeight=\"%d\" frameWidth=\"%d\" frameHeight=\"%d\" fps=\"120\" totalFrame=\"%d\" attackFrame=\"0\" attackedFrame=\"0\" isRight=\"true\" offsetX=\"115\" offsetY=\"-268\"/>\n";
	const char *endline_fmt =  "</SWFLoader>\n";
	sprintf(DirSpec,headline_fmt,npc_name,npc_name);
	xml = DirSpec;

    //四组动画
	for(int n=0;n<4;n++)
	{
		
        //对应动画的文件列表
		TPNG_SRC_FILE_LIST &filelist = *aniInfo[n].filelist;
		if(filelist.empty())
		{
			continue;
		}

        //输出文件内容生成
		sprintf(DirSpec,actionline_fmt,aniInfo[n].actionName,aniInfo[n].rowHeight,aniInfo[n].frameWidth,aniInfo[n].frameHeight,aniInfo[n].totalFrame);
		xml +=DirSpec;

		int curCol= 0;
        //遍历一组动画包含的png文件
		for(TPNG_SRC_FILE_LIST_ITER   iter = filelist.begin(); iter !=filelist.end(); iter++)
		{
			int yy = 0;
            //遍历每个动画里单独的png文件。逐行读取源数据，放入bigmap，也就是大图的缓冲区中
			for(int y=aniInfo[n].minTop;y<aniInfo[n].maxBottom;y++)
			{
				int xx = 0;
				for(int x=aniInfo[n].minLeft;x<aniInfo[n].maxRight;x++)
				{
					unsigned int* psrc = (unsigned int*)((*iter)->bmp + y*(*iter)->rawWidth *4+ x*4);
					int dx =  curCol * aniInfo[n].frameWidth  +  xx;
					int dy =  currentRowHeight +yy;
					unsigned int* dest = (unsigned int*)(bigmap + dy * output_width * 4 + dx*4);
					xx++;
					*dest = *psrc;
				}

				yy ++;
			}
			
			curCol ++;
			PNG_SRC_FILE * f = *iter;
			delete f;
		}

		delete aniInfo[n].filelist;
        
        //最终的高度
		currentRowHeight += aniInfo[n].frameHeight;
	}
	xml += endline_fmt;


	SetCurrentDirectoryA(szCurPath);

	strcpy(filename_dest,outputDir);
	if(outputDir[strlen(outputDir)-1]!='\\')
		strcat(filename_dest,"\\");
	strcat(filename_dest,npc_name);
	strcat(filename_dest,".png");
    //将缓冲区数据写入文件，生成大图.param：文件名，缓冲区总数据bytes数，缓冲区操作指针，输入图片长宽
	outputToPngFileNormal(filename_dest,4*output_width * output_height, &s_srcDibBitsBuffer[0],output_width,output_height);


	strcpy(filename_dest,outputDir);
	
	if(outputDir[strlen(outputDir)-1]!='\\')
		strcat(filename_dest,"\\");
	strcat(filename_dest,"AnimationConfig.xml");

	FILE *xmlFile = fopen(filename_dest,"a");
	
	if(xmlFile==NULL)
	{
		return ;
	}

	fwrite(xml.c_str(),1,xml.length(),xmlFile);
	fclose(xmlFile);
}

void print_uasge()
{
	printf("USEAGE:\n"
			"   pngconcat  x:\\path\\to\\yournpcdirectory x:\\path\\to\\output \n"
			"EXAMPLE:\n"
			"   pngconcat  D:\\<a href="file://\\hero_infantry_jolu">hero_</a>xxxxxx D:\\temp \n"
			);
}

int checkDir(const char * szdir)
{
	 struct _stat buf;
   int result;
   char timebuf[26];
   const char* filename = szdir;
   errno_t err;

   // Get data associated with "crt_stat.c": 
   result = _stat( filename, &buf );

   // Check if statistics are valid: 
   if( result != 0 )
   {
      perror( "Problem getting information" );
      switch (errno)
      {
         case ENOENT:
           printf("File %s not found.\n", filename);
           break;
         case EINVAL:
           printf("Invalid parameter to _stat.\n");
           break;
         default:
           /* Should never be reached. */
           printf("Unexpected error in _stat.\n");
      }
	  return -1;
   }
   else
   {
	   if( (buf.st_mode & _S_IFDIR)==0)
	   {
		     printf("input %s is not a directory.\n", filename);
			 return -1;
	   }
		
   }
   return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	if(argc != 3 || checkDir(argv[1])  || checkDir(argv[2]) )
	{
		print_uasge();
		return -1;
	}

	concat(argv[1],argv[2]);
	return 0;
}
            //根据读取到的单个png文件源数据，得到最小的
