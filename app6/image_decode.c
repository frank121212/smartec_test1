#include <stdio.h>
#include <jpeglib.h>
//#include "jpeglib.h"
#include <jerror.h>
#include "image_decode.h"
#include "png.h"
static int halt = 0;
void rgb32to24(char *rgb32_buffer,char *rgb24_buffer,int height,int width)
{
  char *ptr32 = rgb32_buffer;
  char *ptr24 = rgb24_buffer;
  int i,j,k;
  for(j = 0;j<height;j++)
    for(i = 0,k = 0;i<width*4,k<width*3;i+=4,k+=3)
      {
        *(ptr24+j*width*3+k*3) = *(ptr32+j*width*4+i*4);
        *(ptr24+j*width*3+k*3+1) = *(ptr32+j*width*4+i*4+1);
        *(ptr24+j*width*3+k*3+2) = *(ptr32+j*width*4+i*4+2);
      }
}

int write_JPEG_file (char *filename,char *image_buffer, int image_height, int image_width)
{
    struct jpeg_compress_struct cinfo;

    struct jpeg_error_mgr jerr;
    /* More stuff */
    FILE * outfile;		/* target file */
    JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
    int row_stride;		/* physical row width in image buffer */

    /* Step 1: allocate and initialize JPEG compression object */
    cinfo.err = jpeg_std_error(&jerr);
    /* Now we can initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    /* Step 2: specify data destination (eg, a file) */
    /* Note: steps 2 and 3 can be done in either order. */
    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", JPEG_FILE);
        return -1;
    }
    jpeg_stdio_dest(&cinfo, outfile);

    /* Step 3: set parameters for compression */
    cinfo.image_width = image_width; 	/* image width and height, in pixels */
    cinfo.image_height = image_height;
    cinfo.input_components = 3;		/* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, QUALITY, TRUE /* limit to baseline-JPEG values */);

    /* Step 4: Start compressor */
    jpeg_start_compress(&cinfo, TRUE);

    /* Step 5: while (scan lines remain to be written) */
    /*           jpeg_write_scanlines(...); */
    row_stride = image_width * 3;	/* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = (JSAMPLE *)&image_buffer[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /* Step 6: Finish compression */

    jpeg_finish_compress(&cinfo);
    /* After finish_compress, we can close the output file. */
    fclose(outfile);

    /* Step 7: release JPEG compression object */
    jpeg_destroy_compress(&cinfo);
    return 0;
}
int read_jpeg_file(int x,int y,const char *input_filename)
{
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE *input_file;
        JSAMPARRAY buffer;
        char *buffer1;
        int row_width;
        int w,h;

        cinfo.err = jpeg_std_error(&jerr);
        printf("file is %s\n",input_filename);
        if ((input_file = fopen(input_filename, "rb")) == NULL) {
                fprintf(stderr, "can't open %s\n", input_filename);
                printf("can't open\n");
                return -1;
        }
        jpeg_create_decompress(&cinfo);
        /* Specify data source for decompression */
        jpeg_stdio_src(&cinfo, input_file);
        /* Read file header, set default decompression parameters */
        jpeg_read_header(&cinfo, TRUE);
        cinfo.scale_num=1;
        cinfo.scale_denom=1;
        /* Start decompressor */
        jpeg_start_decompress(&cinfo);
        row_width = cinfo.output_width * cinfo.output_components;
        buffer = (*cinfo.mem->alloc_sarray)
              ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);
        //buffer1 = (unsigned char *) malloc(cinfo.output_width * cinfo.output_components);
        //printf("w = %d,h = %d,p = %d\n",cinfo.output_width,cinfo.output_height,cinfo.output_components);
        //rintf("size of buffer is %d,size of J is %d\n",sizeof(buffer),sizeof(JSAMPARRAY));
        /* Process data */
        h = 0;
        while (cinfo.output_scanline < cinfo.output_height) 
        {
            jpeg_read_scanlines(&cinfo, buffer, 1);

            for (w = 0; w < cinfo.output_width; w++)
            {
            	pixel_dot(x+w,y+h,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
            	/*
            	pixel_dot(x+w*2,y+h*2,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
            	pixel_dot(x+w*2+1,y+h*2,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
            	pixel_dot(x+w*2,y+h*2+1,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
            	pixel_dot(x+w*2+1,y+h*2+1,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
	            */
	        }
            h++;

        }
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        //printf("jpeg_destroy\n");
        //jpeg_destroy_decompress(&cinfo);
        /* Close files, if we opened them */
        fclose(input_file);
        //free(buffer);
        return 0;
}
int read_jpeg_file_new(int x,int y,const char *input_filename)
{
        struct jpeg_decompress_struct cinfo;
        struct jpeg_error_mgr jerr;
        FILE *input_file;
        JSAMPARRAY buffer;
        //unsigned char *buffer;
        int row_width;
        int w,h;

        cinfo.err = jpeg_std_error(&jerr);

        if ((input_file = fopen(input_filename, "rb")) == NULL) {
                fprintf(stderr, "can't open %s\n", input_filename);
                printf("can't open\n");
                return -1;
        }
        jpeg_create_decompress(&cinfo);

        /* Specify data source for decompression */
        jpeg_stdio_src(&cinfo, input_file);

        /* Read file header, set default decompression parameters */
        jpeg_read_header(&cinfo, TRUE);

        cinfo.scale_num=1;
        cinfo.scale_denom=1;

        /* Start decompressor */
        jpeg_start_decompress(&cinfo);

        row_width = cinfo.output_width * cinfo.output_components;

        buffer = (*cinfo.mem->alloc_sarray)
               ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);
        //buffer = (unsigned char *) malloc(cinfo.output_width * cinfo.output_components);
        printf("w = %d,h = %d,p = %d\n",cinfo.output_width,cinfo.output_height,cinfo.output_components);
        //rintf("size of buffer is %d,size of J is %d\n",sizeof(buffer),sizeof(JSAMPARRAY));
        /* Process data */
        h = 0;
        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, buffer, 1);
            /*
            for (w = 0; w < cinfo.output_width; w++)
            {
                //pixel_dot(x+w,y+h,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
            	//pixel_dot(x+w,y+h,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
            }
            h++;
            */
        }
        printf("jpeg end\n");
        //free(buffer);
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);

        /* Close files, if we opened them */
        fclose(input_file);
        //free(buffer);
        return 0;
}
int detect_png(int x,int y,char *filename)
/* 用于解码png图片 */
{
    FILE *pic_fp;
    unsigned char* p1;
    int *p;
    pic_data out;
    pic_fp = fopen(filename, "rb");
    if(pic_fp == NULL) /* 文件打开失败 */
        return -1;
    /* 初始化各种结构 */
    png_structp png_ptr;
    png_infop   info_ptr;
    char        buf[PNG_BYTES_TO_CHECK];
    int         temp;

    png_ptr  = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    info_ptr = png_create_info_struct(png_ptr);

    setjmp(png_jmpbuf(png_ptr)); // 这句很重要

    temp = fread(buf,1,PNG_BYTES_TO_CHECK,pic_fp);
    temp = png_sig_cmp((void*)buf, (png_size_t)0, PNG_BYTES_TO_CHECK);
    printf("1\n");
    /*检测是否为png文件*/
    if (temp!=0) return 1;

    rewind(pic_fp);
    /*开始读文件*/
    png_init_io(png_ptr, pic_fp);
    // 读文件了
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);

    int color_type,channels;

    /*获取宽度，高度，位深，颜色类型*/
    channels       = png_get_channels(png_ptr, info_ptr);   /*获取通道数*/
    out.bit_depth = png_get_bit_depth(png_ptr, info_ptr);   /* 获取位深 */
    color_type     = png_get_color_type(png_ptr, info_ptr); /*颜色类型*/
    int i,j,k;
    int size;
    int pos = 0;
    /* row_pointers里边就是rgba数据 */
    png_bytep* row_pointers;
    row_pointers = png_get_rows(png_ptr, info_ptr);
    out.width = png_get_image_width(png_ptr, info_ptr);
    out.height = png_get_image_height(png_ptr, info_ptr);
    printf("width is %d,height is %d,depth is %d,channel is %d\n",out.width,out.height,out.bit_depth,channels);
    size = out.width * out.height;
    printf("size is %d\n",size);
    if(channels == 4 || color_type == PNG_COLOR_TYPE_RGB_ALPHA)
    {/*如果是RGB+alpha通道，或者RGB+其它字节*/
    	printf("4 depth\n");
        out.flag = HAVE_ALPHA;
        printf("2\n");
        temp = (4 * out.width);/* 每行有4 * out->width个字节 */
        for(i = 0; i < out.height; i++)
        {
			for (j = 0, k = 0; j < temp; j += 4, k++) {/* 一个字节一个字节的赋值 */
				//out->rgba[pos] = row_pointers[i][j]; // red
				//out->rgba[size + pos] = row_pointers[i][j+1]; // green
				// out->rgba[size * 2 + pos] = row_pointers[i][j+2];   // blue
				//out->rgba[size * 3 + pos] = row_pointers[i][j+3]; // alpha
				if((row_pointers[i][j + 3] == 0)||(row_pointers[i][j + 3] != 0xFF)){
					continue;
				}
				pixel_dot(x + k, y + i,
						row_pointers[i][j] << 16 | row_pointers[i][j + 1] << 8
								| row_pointers[i][j + 2]
								| 0 << 24);
			}
			//printf("pos is %d\n",pos);
			pos = 0;
		}
        printf("2\n");
    }
    else if(channels == 3 || color_type == PNG_COLOR_TYPE_RGB)
    {/* 如果是RGB通道 */
    	printf("3start\n");
        printf("size is %d\n",size);
        out.flag = NO_ALPHA;
        printf("2");
        temp = (3 * out.width);/* 每行有3 * out->width个字节 */
        printf("temp = %d\n",temp);
        for(i = 0; i < out.height; i++)
        {
        	for(j = 0,k = 0; j < temp; j += 3,k++)
            {
                //fbuffer[i * WIDTH * 4 + k] = row_pointers[i][j];
                //fbuffer[i * WIDTH * 4 + k + 1] = row_pointers[i][j+1];
                //fbuffer[i * WIDTH * 4 + k + 2] = row_pointers[i][j+2];
                //fbuffer[i * WIDTH * 4 + j] = row_pointers[i][j];
				//pixel_dot(x+j,y+i,buffer[0][w*3]<<16|buffer[0][w*3+1]<<8|buffer[0][w*3+2]|0x00<<24);
                pixel_dot(x+k,y+i,row_pointers[i][j]<<16|row_pointers[i][j+1]<<8|row_pointers[i][j+2]|0x00<<24);
            }
        }
        printf("3end\n");
    }
    else return 1;

    /* 撤销数据占用的内存 */
    printf("4\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    return 0;
}
int write_png_file(char *file_name , char *graph, int width,int height)
/* 将puc_data结构中的数据写入至png文件 */
{
    int j, i, k, temp, pos;
    png_byte color_type;

    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep * row_pointers;
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp)
    {
        printf("[write_png_file] File %s could not be opened for writing", file_name);
        return -1;
    }


    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
    {
        printf("[write_png_file] png_create_write_struct failed");
        return -1;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        printf("[write_png_file] png_create_info_struct failed");
        return -1;
    }
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during init_io");
        return -1;
    }
    png_init_io(png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during writing header");
        return -1;
    }
    /*
    if(graph->flag == HAVE_ALPHA) color_type = PNG_COLOR_TYPE_RGB_ALPHA;
    else color_type = PNG_COLOR_TYPE_RGB;
    */
    color_type = PNG_COLOR_TYPE_RGB;
    png_set_IHDR(png_ptr, info_ptr, width, height,
        8, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during writing bytes");
        return -1;
    }
    /*
    if(graph->flag == HAVE_ALPHA) temp = (4 * graph->width);
    else temp = (3 * graph->width);
    */
    temp = (3 * width);
    pos = 0;
    row_pointers = (png_bytep*)malloc(height*sizeof(png_bytep));
    for(i = 0; i < height; i++)
    {
        row_pointers[i] = (png_bytep)malloc(sizeof(unsigned char)*temp);
        for(j = 0,k = 0; j < temp; j += 3,k += 4)
        {
            row_pointers[i][j]   = graph[width * i * 4 + k + 2]; // red
            row_pointers[i][j+1] = graph[width * i * 4 + k + 1]; // green
            row_pointers[i][j+2] = graph[width * i * 4 + k + 0];   // blue
            /*
            if(graph->flag == HAVE_ALPHA)
                row_pointers[i][j+3] = graph->rgba[size * 3 + pos]; // alpha
            */
            //++pos;
        }
    }
    png_write_image(png_ptr, row_pointers);

    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        printf("[write_png_file] Error during end of write");
        return -1;
    }
    png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
    for (j=0; j<height; j++)
        free(row_pointers[j]);
    free(row_pointers);

    fclose(fp);
    return 0;
}
int halt1(void){
	halt = 1;
	printf("halt\n");
}
int play1(void){
	halt = 0;
	printf("play\n");
}
int player(int x,int y,char *filename) {
	int i,len;
	char s[9];
	int j = 0;
	int ret;
	char buf[10000];
	FILE *fp1;
	FILE *fp2;
	fp1 = fopen(filename, "rb+");
	while (j < 100) {
		while(halt == 1){
			usleep(1000*50);
		}
		fread(&i,sizeof(i),1,fp1);
		fp2 = fopen("/opt/PBAPPS/app/temp.jpg", "wb");
		ret = fread(buf,1,i, fp1);
		ret = fwrite(buf,1,i, fp2);
		fclose(fp2);
		if (ret != i)
			printf("write error!\n");
		else read_jpeg_file(x,y,"/opt/PBAPPS/app/temp.jpg");
		memset(buf, 0, 10000);
		//fclose(fp2);
		remove("/opt/PBAPPS/app/temp.jpg");
		j++;
	}
	fclose(fp1);
	return 0;
}
