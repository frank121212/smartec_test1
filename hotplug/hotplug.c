/*
 * hotplug.c
 *
 *  Created on: 2013Äê12ÔÂ18ÈÕ
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include <errno.h>
#include <sys/stat.h>
static int init_hotplug_sock ( void )
{
    struct sockaddr_nl snl ;
    const int buffersize = 16 * 1024 * 1024;
    int retval ;
    memset (& snl , 0x00, sizeof ( struct sockaddr_nl));
    snl .nl_family = AF_NETLINK;
    snl .nl_pid = getpid ();
    snl .nl_groups = 1;
    int hotplug_sock = socket (PF_NETLINK, SOCK_DGRAM , NETLINK_KOBJECT_UEVENT);
    if ( hotplug_sock == -1) {
        printf ( "error getting socket: %s" , strerror ( errno ));
        return -1;
    }
    /* set receive buffersize */
    setsockopt ( hotplug_sock , SOL_SOCKET , SO_RCVBUFFORCE, & buffersize , sizeof ( buffersize ));
   retval = bind ( hotplug_sock , ( struct sockaddr *) & snl , sizeof ( struct sockaddr_nl));
    if ( retval < 0) {
        printf ( "bind failed: %s" , strerror ( errno ));
        close ( hotplug_sock );
        hotplug_sock = -1;
        return -1;
    }
    return hotplug_sock ;
}
#define UEVENT_BUFFER_SIZE       2048
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
        printf("1\n");
        /* Specify data source for decompression */
        jpeg_stdio_src(&cinfo, input_file);
        printf("2\n");
        /* Read file header, set default decompression parameters */
        jpeg_read_header(&cinfo, TRUE);
        printf("3\n");
        cinfo.scale_num=1;
        cinfo.scale_denom=1;
        printf("4\n");
        /* Start decompressor */
        jpeg_start_decompress(&cinfo);
        printf("5\n");
        row_width = cinfo.output_width * cinfo.output_components;

        buffer = (*cinfo.mem->alloc_sarray)
              ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_width, 1);
        printf("6\n");
        //buffer1 = (unsigned char *) malloc(cinfo.output_width * cinfo.output_components);
        printf("w = %d,h = %d,p = %d\n",cinfo.output_width,cinfo.output_height,cinfo.output_components);
        //rintf("size of buffer is %d,size of J is %d\n",sizeof(buffer),sizeof(JSAMPARRAY));
        /* Process data */
        printf("7\n");
        h = 0;
        while (cinfo.output_scanline < cinfo.output_height)
        {
            jpeg_read_scanlines(&cinfo, buffer, 1);
            /*
            for (w = 0; w < cinfo.output_width; w++)
            {
            	pixel_dot(x+w,y+h,buffer1[w*3]<<16|buffer1[w*3+1]<<8|buffer1[w*3+2]|0x00<<24);
            	//pixel_dot(x+w,y+h,buffer1[0][w*3]<<16|buffer1[0][w*3+1]<<8|buffer1[0][w*3+2]|0x00<<24);
	        }
            h++;
            */
        }
        printf("jpeg end\n");
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        printf("jpeg_destroy\n");
        //jpeg_destroy_decompress(&cinfo);
        /* Close files, if we opened them */
        fclose(input_file);
        //free(buffer);
        return 0;
}
int main ( int argc , char * argv [])
{
         int hotplug_sock        = init_hotplug_sock ();
         int i,ret;
         struct stat _stat;
         char device[100];

         while (1)
         {
                   char buf [ UEVENT_BUFFER_SIZE *2] = {0};
                   recv ( hotplug_sock , & buf , sizeof ( buf ), 0);
                   //printf ( "%s/n" , buf );
                   printf("find a new device\n");
                   /*
                   for(i = 0;i < 10;i++){
                	   sprintf(device,"%s%d","/dev/sda",i);
                	   ret = stat(device, &_stat);
                	   		if(ret == 0){
                	   			printf("udisk %s is found\n",device);
                	   			break;
                	   		}
                   }*/
                   //sprintf(device,"%s%d","/dev/sda1");
				   ret = stat("/dev/sda1", &_stat);
					if(ret == 0){
						printf("udisk %s is found\n",device);
						system("mount /dev/sda1 /media/ram");
						break;
					}
					else printf("udisk is unsert\n");
         }
         return 0;
}

