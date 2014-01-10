#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <jpeglib.h>
#include <sys/stat.h>   
FILE *bmpfile;//输入文件
int image_width,image_height,image_size,bits_per_pixel,headersize,depth;
unsigned char *src_data;//位图数据
int get_2b(unsigned char*a,int offset)
{
    return a[offset+1]<<8|a[offset];
}
int get_4b(unsigned char*a,int offset)
{
    return (a[offset+3]<<24)|(a[offset+2]<<16)|(a[offset+1]<<8)|a[offset];
}   
unsigned long get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}
//读取位图文件头和位图数据
void read_bmp_header(char *bmpfilename)
{
    unsigned char bmpfileheader[14];//文件头
    unsigned char bmpinfoheader[40];//信息头
    bmpfile=fopen(bmpfilename,"r");//
    if(bmpfile<0)
        printf("open bmp file error!\n");
    printf("open bmp file success!\n");
    //读取bmp文件头
    fread(bmpfileheader,14,1,bmpfile);
    int type=get_2b(bmpfileheader,0);
    printf("type=0x%x\n",type);
    int filesize=get_4b(bmpfileheader,2);
    printf("filesize=%d bytes\n",filesize);
    headersize=get_4b(bmpfileheader,10);
    printf("headersize=%d bytes\n",headersize);
    if(headersize>54)
        printf("colormap size=%d bytes\n",headersize-54);
    //读取bmp信息头
    fseek(bmpfile,14,SEEK_SET);
    fread(bmpinfoheader,40,1,bmpfile);
    image_width=get_4b(bmpinfoheader,4);
    /*图像的宽度必须被4整除，否则jpg图像会变形！！！
 49     这个很多代码里都没有说明,我在压缩图片时发现有些图片能够正确压缩，有些图片压缩后变形
 50     经过仔细研究发现是宽度有问题*/
    while (image_width%4!=0)
       image_width++;
    printf("weight=%d\n",image_width);
    image_height=get_4b(bmpinfoheader,8);
    printf("height=%d\n",image_height);
    bits_per_pixel=get_2b(bmpinfoheader,14);
    printf("bits_per_pixel=%d\n",bits_per_pixel);
    depth=bits_per_pixel/8;
    image_size=image_width*image_height*depth;
    src_data=(unsigned char *)malloc(image_size);
    fseek(bmpfile,headersize,SEEK_SET);
    fread(src_data,sizeof(unsigned char)*image_size,1,bmpfile);
    fclose(bmpfile);
}
//参考了libjpeg库里的example.c
void write_JPEG_file (char * outfilename, unsigned char * buffer,int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * outfile;
    unsigned char *dst_data;
    int i,j;
    //char *point;
    JSAMPROW row_pointer[1];
    //JSAMPARRAY buffer;
    int row_stride;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    //打开输出jpg文件
    if ((outfile = fopen(outfilename, "ab+")) == NULL) {
    fprintf(stderr, "can't open %s\n", outfilename);
    exit(1);
}
    jpeg_stdio_dest(&cinfo, outfile);
    cinfo.image_width = image_width;     /* image width and height, in pixels */
    cinfo.image_height = image_height;
    cinfo.input_components = depth;        /* # of color components per pixel */
    cinfo.in_color_space = (depth==3) ? JCS_RGB : JCS_GRAYSCALE;     /* colorspace of input image */
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);
    dst_data=(unsigned char *)malloc(image_size*sizeof(unsigned char));
    //调整RGB顺序BGR->RGB
    for(i=0;i<image_height;i++){
        for(j=0;j<image_width;j++)
    {
           if(depth==1)//灰度图
                *(dst_data+i*image_width+j)=*(src_data+i*image_width+j);
           else //真彩图
                {
                     *(dst_data+i*image_width*depth+j*3+0)=*(src_data+i*image_width*depth+j*3+2);
                     *(dst_data+i*image_width*depth+j*3+1)=*(src_data+i*image_width*depth+j*3+1);
                     *(dst_data+i*image_width*depth+j*3+2)=*(src_data+i*image_width*depth+j*3+0);
                }
         }
     }
     jpeg_start_compress(&cinfo, TRUE);
     row_stride = image_width * cinfo.input_components;    /* JSAMPLEs per row in image_buffer */
     while (cinfo.next_scanline < cinfo.image_height) {
         row_pointer[0] = & dst_data[(cinfo.image_height - cinfo.next_scanline - 1) * row_stride];//cinfo.next_scanline * row_stride
         (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
     }
     jpeg_finish_compress(&cinfo);
     fclose(outfile);
     jpeg_destroy_compress(&cinfo);
     free(src_data);
     free(dst_data);
 }
int main(int argc,char *argv[])
{
    //if(argc<4)
        //printf("use:appname inputname outputname quality\n");
    //else{
        //read_bmp_header(argv[1]);IMG00000.bmp
       int i = 0;
       char s[9];
       char a[9];
       FILE *fp1;
       FILE *fp2;
       int j,ret;
       int num = 0;
       //a;
       
        while(i < 100)
        {
          if(i<10)
            sprintf(s,"IMG0000%d.bmp",i);
          else
            sprintf(s,"IMG000%d.bmp",i);
          printf("s is %s\n",s);
          read_bmp_header(s);
          memset(s,0,9);
        //write_JPEG_file(argv[2],src_data,atoi(argv[3]));
          if(i<10)
            sprintf(a,"0%d.jpg",i);
          else
            sprintf(a,"%d.jpg",i);
          printf("a is %s\n",a);
          write_JPEG_file(a,src_data,80);
          memset(a,0,9);
         i++;
        } 
        fp1 = fopen("gif.dat","rb+");
        while(num < 100)
        {
        fread(&j,1,sizeof(j),fp1);
        printf("file %d len is %d\n",num,j);
        fseek(fp1,j,SEEK_CUR);
        num++;
        }
        fclose(fp1);
    return 0;
}
