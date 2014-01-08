/*
 * fbutils.c
 *
 * Utility routines for framebuffer interaction
 *
 * This file is placed under the GPL.
 *
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>

#include <linux/vt.h>
#include <linux/kd.h>
#include <linux/fb.h>
#include <math.h>

#include "font.h"
#include "fbutils.h"
#include "char.h"
#define PIXE   4
#define PI     3.14
#define U32 unsigned int
union multiptr {
	unsigned char *p8;
	unsigned short *p16;
	unsigned long *p32;
};
typedef struct
{
  char cfType[2];
  char cfSize[4];
  char cfReserved[4];
  char cfoffBits[4];
}__attribute__((packed)) BITMAPFILEHEADER;

//40byte
typedef struct
{
  char ciSize[4];         /* size of BITMAPINFOHEADER */
  char ciWidth[4];
  char ciHeight[4];
  char ciPlanes[2];
  char ciBitCount[2];
  char ciCompress[4];
  char ciSizeImage[4];
  char ciXPelsPerMeter[4];
  char ciYPelsPerMeter[4];
  char ciClrUsed[4];
  char ciClrImportant[4];
}__attribute__((packed)) BITMAPINFOHEADER;



typedef struct
{
  unsigned int blue:8;
  unsigned int green:8;
  unsigned int red:8;
  unsigned int rev:8;
}__attribute__((packed)) PIXEL;

BITMAPFILEHEADER FileHead;
BITMAPINFOHEADER InfoHead;

static int con_fd, fb_fd, last_vt = -1;
static struct fb_fix_screeninfo fix;
static struct fb_var_screeninfo var;
static int bmp_w,bmp_h;
unsigned char *fbuffer;
static unsigned int screensize;
unsigned char **line_addr;
static int fb_fd=0;
static int bytes_per_pixel;
static unsigned colormap [256];
unsigned char pic_buffer[HEIGHT*WIDTH];
unsigned char *pic_ptr = pic_buffer;
__u32 xres, yres;

static char *defaultfbdevice = "/dev/fb0";
static char *defaultconsoledevice = "/dev/tty";
static char *fbdevice = NULL;
static char *consoledevice = NULL;
static int palette [] =
{
	0x000000, 0xffe080, 0xffffff, 0xe0c0a0, 0x304050, 0x80b8c0
};
#define NR_COLORS (sizeof (palette) / sizeof (palette [0]))
static int button_palette [6] =
{
	1, 4, 2,
	1, 5, 0
};
void palette_init(void){
	int i;
	for (i = 0; i < NR_COLORS; i++)
		setcolor(i, palette[i]);
}
int open_framebuffer(void)
{
	struct vt_stat vts;
	char vtname[128];
	int fd, nr;
	unsigned y, addr;

	if ((fbdevice = getenv ("TSLIB_FBDEVICE")) == NULL)
		fbdevice = defaultfbdevice;
    /*
	if ((consoledevice = getenv ("TSLIB_CONSOLEDEVICE")) == NULL)
		consoledevice = defaultconsoledevice;

	if (strcmp (consoledevice, "none") != 0) {
		sprintf (vtname,"%s%d", consoledevice, 1);
        	fd = open (vtname, O_WRONLY);
        	if (fd < 0) {
        	        perror("open consoledevice");
        	        return -1;
        	}

		if (ioctl(fd, VT_OPENQRY, &nr) < 0) {
        	        perror("ioctl VT_OPENQRY");
        	        return -1;
        	}
        	close(fd);

        	sprintf(vtname, "%s%d", consoledevice, nr);

        	con_fd = open(vtname, O_RDWR | O_NDELAY);
        	if (con_fd < 0) {
        	        perror("open tty");
        	        return -1;
        	}

        	if (ioctl(con_fd, VT_GETSTATE, &vts) == 0)
        	        last_vt = vts.v_active;

        	if (ioctl(con_fd, VT_ACTIVATE, nr) < 0) {
        	        perror("VT_ACTIVATE");
        	        close(con_fd);
        	        return -1;
        	}

#ifndef TSLIB_NO_VT_WAITACTIVE
        	if (ioctl(con_fd, VT_WAITACTIVE, nr) < 0) {
        	        perror("VT_WAITACTIVE");
        	        close(con_fd);
        	        return -1;
        	}
#endif

        	if (ioctl(con_fd, KDSETMODE, KD_GRAPHICS) < 0) {
        	        perror("KDSETMODE");
        	        close(con_fd);
        	        return -1;
        	}

	}
    */
	fb_fd = open(fbdevice, O_RDWR);
	if (fb_fd == -1) {
		perror("open fbdevice");
		return -1;
	}

	if (ioctl(fb_fd, FBIOGET_FSCREENINFO, &fix) < 0) {
		perror("ioctl FBIOGET_FSCREENINFO");
		close(fb_fd);
		return -1;
	}

	if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &var) < 0) {
		perror("ioctl FBIOGET_VSCREENINFO");
		close(fb_fd);
		return -1;
	}
	xres = var.xres;
	yres = var.yres;
        screensize = var.xres * var.yres * var.bits_per_pixel / 8;
        printf("screensize = %d,smem_len = %d\n",screensize,fix.smem_len);
	fbuffer = mmap(NULL, fix.smem_len, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED, fb_fd, 0);
        //fbuffer = mmap(NULL,screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if (fbuffer == (unsigned char *)-1) {
		perror("mmap framebuffer");
		close(fb_fd);
		return -1;
	}
	memset(fbuffer,0,fix.smem_len);
        //memset(fbuffer,0,screensize);
	bytes_per_pixel = (var.bits_per_pixel + 7) / 8;
	line_addr = malloc (sizeof (__u32) * var.yres_virtual);
        printf("yres_virtual =%d,xres_virtual =%d\n",var.yres_virtual,var.xres_virtual);
	addr = 0;
	for (y = 0; y < var.yres_virtual; y++, addr += fix.line_length)
		line_addr [y] = fbuffer + addr;
        printf("fix.line_length = %d",fix.line_length);
	return 0;
}

void close_framebuffer(void)
{
	munmap(fbuffer, fix.smem_len);
        //munmap(fbuffer,screensize);
	close(fb_fd);


	if(strcmp(consoledevice,"none")!=0) {
	
        	if (ioctl(con_fd, KDSETMODE, KD_TEXT) < 0)
        	        perror("KDSETMODE");

        	if (last_vt >= 0)
        	        if (ioctl(con_fd, VT_ACTIVATE, last_vt))
        	                perror("VT_ACTIVATE");

        	close(con_fd);
	}

        free (line_addr);
}

void put_cross(int x, int y, unsigned colidx)
{
	line (x - 10, y, x - 2, y, colidx);
	line (x + 2, y, x + 10, y, colidx);
	line (x, y - 10, x, y - 2, colidx);
	line (x, y + 2, x, y + 10, colidx);

#if 1
	line (x - 6, y - 9, x - 9, y - 9, colidx + 1);
	line (x - 9, y - 8, x - 9, y - 6, colidx + 1);
	line (x - 9, y + 6, x - 9, y + 9, colidx + 1);
	line (x - 8, y + 9, x - 6, y + 9, colidx + 1);
	line (x + 6, y + 9, x + 9, y + 9, colidx + 1);
	line (x + 9, y + 8, x + 9, y + 6, colidx + 1);
	line (x + 9, y - 6, x + 9, y - 9, colidx + 1);
	line (x + 8, y - 9, x + 6, y - 9, colidx + 1);
#else
	line (x - 7, y - 7, x - 4, y - 4, colidx + 1);
	line (x - 7, y + 7, x - 4, y + 4, colidx + 1);
	line (x + 4, y - 4, x + 7, y - 7, colidx + 1);
	line (x + 4, y + 4, x + 7, y + 7, colidx + 1);
#endif
}

void put_char_8(int x, int y, int c, int colidx)
{
	int i,j,bits;

	for (i = 0; i < font_vga_8x8.height; i++) {
		bits = font_vga_8x8.data [font_vga_8x8.height * c + i];
		for (j = 0; j < font_vga_8x8.width; j++, bits <<= 1)
			if (bits & 0x80)
				pixel (x + j, y + i, colidx);
	}
}
void put_char_16(int x, int y, int c, int colidx)
{
	int i,j,bits;

	for (i = 0; i < font_vga_8x16.height; i++) {
		bits = font_vga_8x16.data [font_vga_8x16.height * c + i];
		for (j = 0; j < font_vga_8x16.width; j++, bits <<= 1)
			if (bits & 0x80)
				pixel (x + j, y + i, colidx);
	}
}

void put_string_8(int x, int y, char *s, unsigned colidx)
{
	int i;
	for (i = 0; *s; i++, x += font_vga_8x8.width, s++)
		put_char_8(x, y, *s, colidx);
}
void put_string_16(int x, int y, char *s, unsigned colidx)
{
	int i;
	for (i = 0; *s; i++, x += font_vga_8x16.width, s++)
		put_char_16(x, y, *s, colidx);
}

void put_string_center(int x, int y, char *s,enum size type, unsigned colidx)
{
	size_t sl = strlen (s);
	switch(type) {
    case MIDDLE:
    	 put_string_16(x - (sl / 2) * font_vga_8x16.width,
    	                    y - font_vga_8x16.height / 2, s, colidx);
        break;

    case SMALL:
    	 put_string_8(x - (sl / 2) * font_vga_8x8.width,
    	                    y - font_vga_8x8.height / 2, s, colidx);
        break;

    case BIG:
    	//en_string(x - (sl / 2) * 16,y - 16,s,type,colidx);
        break;
    default :
    	break;
    }

}

void setcolor(unsigned colidx, unsigned value)
{
	unsigned res;
	unsigned short red, green, blue;
	struct fb_cmap cmap;

#ifdef DEBUG
	if (colidx > 255) {
		fprintf (stderr, "WARNING: color index = %u, must be <256\n",
			 colidx);
		return;
	}
#endif

	switch (bytes_per_pixel) {
	default:
	case 1:
		res = colidx;
		red = (value >> 8) & 0xff00;
		green = value & 0xff00;
		blue = (value << 8) & 0xff00;
		cmap.start = colidx;
		cmap.len = 1;
		cmap.red = &red;
		cmap.green = &green;
		cmap.blue = &blue;
		cmap.transp = NULL;

        	if (ioctl (fb_fd, FBIOPUTCMAP, &cmap) < 0)
        	        perror("ioctl FBIOPUTCMAP");
		break;
	case 2:
	case 4:
		red = (value >> 16) & 0xff;
		green = (value >> 8) & 0xff;
		blue = value & 0xff;
		res = ((red >> (8 - var.red.length)) << var.red.offset) |
                      ((green >> (8 - var.green.length)) << var.green.offset) |
                      ((blue >> (8 - var.blue.length)) << var.blue.offset);
	}
        colormap [colidx] = res;
}

static inline void __setpixel (union multiptr loc, unsigned xormode, unsigned color)
{
	switch(bytes_per_pixel) {
	case 1:
	default:
		if (xormode)
			*loc.p8 ^= color;
		else
			*loc.p8 = color;
		break;
	case 2:
		if (xormode)
			*loc.p16 ^= color;
		else
			*loc.p16 = color;
		break;
	case 4:
		if (xormode)
			*loc.p32 ^= color;
		else
			*loc.p32 = color;
		break;
	}
}

void pixel (int x, int y, unsigned colidx)
{
	unsigned xormode;
	union multiptr loc;

	if ((x < 0) || ((__u32)x >= var.xres_virtual) ||
	    (y < 0) || ((__u32)y >= var.yres_virtual))
		return;

	xormode = colidx & XORMODE;
	colidx &= ~XORMODE;

#ifdef DEBUG
	if (colidx > 255) {
		fprintf (stderr, "WARNING: color value = %u, must be <256\n",
			 colidx);
		return;
	}
#endif

	loc.p8 = line_addr [y] + x * bytes_per_pixel;
	__setpixel (loc, xormode, colormap [colidx]);
}
void pixel_dot(int x, int y, unsigned colidx)
{
	unsigned xormode;
	union multiptr loc;

	if ((x < 0) || ((__u32)x >= var.xres_virtual) ||
	    (y < 0) || ((__u32)y >= var.yres_virtual))
		return;

	xormode = colidx & XORMODE;
	colidx &= ~XORMODE;

#ifdef DEBUG
	if (colidx > 255) {
		fprintf (stderr, "WARNING: color value = %u, must be <256\n",
			 colidx);
		return;
	}
#endif

	loc.p8 = line_addr [y] + x * bytes_per_pixel;
	__setpixel (loc,xormode,colidx);
}
void mid_pixel(int x,int y,unsigned colidx)
{
   pixel(x,y-2,colidx);
   pixel(x-1,y-1,colidx);
   pixel(x,y-1,colidx);
   pixel(x+1,y-1,colidx);
   pixel(x-2,y,colidx);
   pixel(x-1,y,colidx);
   pixel(x,y,colidx);
   pixel(x+1,y,colidx);
   pixel(x+2,y,colidx);
   pixel(x-1,y+1,colidx);
   pixel(x,y+1,colidx);
   pixel(x+1,y+1,colidx);
   pixel(x,y+2,colidx);
}
void variable_pixel(int x,int y,int l,unsigned colidx){
   int i,j;
   for(j = 0;j <= l;j++)
	   for(i = x - j;i <= x + j;i++)
		   pixel(i,y + j,colidx);
   for(j = 1;j <= l; j++)
	   for(i = x - l + j;i <= x + l - j;i++)
		   pixel(i,y + j,colidx);
}
void pixel_pic(int x,int y,unsigned colidx)
{
  pic_ptr[y*WIDTH+x] = 1;  
}
void mid_pixel_pic(int x,int y,unsigned colidx)
{
   pixel_pic(x,y-2,colidx);
   pixel_pic(x-1,y-1,colidx);
   pixel_pic(x,y-1,colidx);
   pixel_pic(x+1,y-1,colidx);
   pixel_pic(x-2,y,colidx);
   pixel_pic(x-1,y,colidx);
   pixel_pic(x,y,colidx);
   pixel_pic(x+1,y,colidx);
   pixel_pic(x+2,y,colidx);
   pixel_pic(x-1,y+1,colidx);
   pixel_pic(x,y+1,colidx);
   pixel_pic(x+1,y+1,colidx);
   pixel_pic(x,y+2,colidx);
}
void line (int x1, int y1, int x2, int y2, unsigned colidx)
{
	int tmp;
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (abs (dx) < abs (dy)) {
		if (y1 > y2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		x1 <<= 16;
		/* dy is apriori >0 */
		dx = (dx << 16) / dy;
		while (y1 <= y2) {
			pixel (x1 >> 16, y1, colidx);
			x1 += dx;
			y1++;
		}
	} else {
		if (x1 > x2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		y1 <<= 16;
		dy = dx ? (dy << 16) / dx : 0;
		while (x1 <= x2) {
			pixel (x1, y1 >> 16, colidx);
			y1 += dy;
			x1++;
		}
	}
}
void brush (int x1, int y1, int x2, int y2,int l, unsigned colidx)
{
	int tmp;
	int dx = x2 - x1;
	int dy = y2 - y1;
    if(l > 9)l = 8;
    if(l <= 0)l = 2;
	if (abs (dx) < abs (dy)) {
		if (y1 > y2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		x1 <<= 16;
		/* dy is apriori >0 */
		dx = (dx << 16) / dy;
		while (y1 <= y2) {
			variable_pixel(x1 >> 16, y1,l, colidx);
			x1 += dx;
			y1++;
		}
	} else {
		if (x1 > x2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		y1 <<= 16;
		dy = dx ? (dy << 16) / dx : 0;
		while (x1 <= x2) {
			variable_pixel(x1, y1 >> 16,l, colidx);
			y1 += dy;
			x1++;
		}
	}
}
void mid_line (int x1, int y1, int x2, int y2, unsigned colidx)
{
	int tmp;
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (abs (dx) < abs (dy)) {
		if (y1 > y2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		x1 <<= 16;
		/* dy is apriori >0 */
		dx = (dx << 16) / dy;
		while (y1 <= y2) {
			mid_pixel (x1 >> 16, y1, colidx);
			//variable_pixel(x1 >> 16, y1,5, colidx);
			x1 += dx;
			y1++;
		}
	} else {
		if (x1 > x2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		y1 <<= 16;
		dy = dx ? (dy << 16) / dx : 0;
		while (x1 <= x2) {
			mid_pixel (x1, y1 >> 16, colidx);
			//variable_pixel(x1, y1 >> 16,5, colidx);
			y1 += dy;
			x1++;
		}
	}
}
void line_pic (int x1, int y1, int x2, int y2, unsigned colidx)
{
	int tmp;
	int dx = x2 - x1;
	int dy = y2 - y1;

	if (abs (dx) < abs (dy)) {
		if (y1 > y2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		x1 <<= 16;
		/* dy is apriori >0 */
		dx = (dx << 16) / dy;
		while (y1 <= y2) {
			mid_pixel_pic (x1 >> 16, y1, colidx);
			x1 += dx;
			y1++;
		}
	} else {
		if (x1 > x2) {
			tmp = x1; x1 = x2; x2 = tmp;
			tmp = y1; y1 = y2; y2 = tmp;
			dx = -dx; dy = -dy;
		}
		y1 <<= 16;
		dy = dx ? (dy << 16) / dx : 0;
		while (x1 <= x2) {
			mid_pixel_pic (x1, y1 >> 16, colidx);
			y1 += dy;
			x1++;
		}
	}
}
void rect (int x1, int y1, int x2, int y2, unsigned colidx)
{
	line (x1, y1, x2, y1, colidx);
	line (x2, y1, x2, y2, colidx);
	line (x2, y2, x1, y2, colidx);
	line (x1, y2, x1, y1, colidx);
}
void fillrect (int x1, int y1, int x2, int y2, unsigned colidx)
{
	int tmp;
	unsigned xormode;
	union multiptr loc;

	/* Clipping and sanity checking */
	if (x1 > x2) { tmp = x1; x1 = x2; x2 = tmp; }
	if (y1 > y2) { tmp = y1; y1 = y2; y2 = tmp; }
	if (x1 < 0) x1 = 0; if ((__u32)x1 >= xres) x1 = xres - 1;
	if (x2 < 0) x2 = 0; if ((__u32)x2 >= xres) x2 = xres - 1;
	if (y1 < 0) y1 = 0; if ((__u32)y1 >= yres) y1 = yres - 1;
	if (y2 < 0) y2 = 0; if ((__u32)y2 >= yres) y2 = yres - 1;

	if ((x1 > x2) || (y1 > y2))
		return;

	xormode = colidx & XORMODE;
	colidx &= ~XORMODE;

#ifdef DEBUG
	if (colidx > 255) {
		fprintf (stderr, "WARNING: color value = %u, must be <256\n",
			 colidx);
		return;
	}
#endif

	colidx = colormap [colidx];

	for (; y1 <= y2; y1++) {
		loc.p8 = line_addr [y1] + x1 * bytes_per_pixel;
		for (tmp = x1; tmp <= x2; tmp++) {
			__setpixel (loc, xormode, colidx);
			loc.p8 += bytes_per_pixel;
		}
	}
}
void gradient_fillrect (int x1, int y1, int x2, int y2, unsigned colidx)
{
	int tmp;
	unsigned xormode;
	union multiptr loc;

	/* Clipping and sanity checking */
	if (x1 > x2) { tmp = x1; x1 = x2; x2 = tmp; }
	if (y1 > y2) { tmp = y1; y1 = y2; y2 = tmp; }
	if (x1 < 0) x1 = 0; if ((__u32)x1 >= xres) x1 = xres - 1;
	if (x2 < 0) x2 = 0; if ((__u32)x2 >= xres) x2 = xres - 1;
	if (y1 < 0) y1 = 0; if ((__u32)y1 >= yres) y1 = yres - 1;
	if (y2 < 0) y2 = 0; if ((__u32)y2 >= yres) y2 = yres - 1;

	if ((x1 > x2) || (y1 > y2))
		return;

	xormode = colidx & XORMODE;
	colidx &= ~XORMODE;

#ifdef DEBUG
	if (colidx > 255) {
		fprintf (stderr, "WARNING: color value = %u, must be <256\n",
			 colidx);
		return;
	}
#endif

	//colidx = colormap [colidx];

	for (; y1 <= y2; y1++) {
		loc.p8 = line_addr [y1] + x1 * bytes_per_pixel;
		for (tmp = x1; tmp <= x2; tmp++) {
			__setpixel (loc, xormode, colidx);
			loc.p8 += bytes_per_pixel;
		}
	}
}
void arc (int x, int y, int w, int h, unsigned colidx)
{
    float arc = PI/20;
    int len = 10;
    int i,j;
    /*
    for(i = cos(10*arc);i < cos(0);i+=cos(arc))
      line(x-(int)(w/2)+(int)(len*(1-i)),y-(int)(h/2)+(int)(len*i),x+(int)(w/2)-(int)(len*(1-i)),y-(int)(h/2)+(int)(len*i),colidx);
    fillrect(x-(int)(w/2)-1,y-(int)(h/2)+len,x+(int)(w/2)+1,y+(int)(h/2)-len,colidx);
    for(i = cos(10*arc);i < cos(0);i+=cos(arc))
      line(x-(int)(w/2)+(int)(len*i),y+(int)(h/2)-len*(1-i),x+(int)(w/2)-(int)(len*i),y+(int)(h/2)-len*(1-i),colidx); 
    */
    /*
    for(i = 10*arc;i > 0;i-=arc)
      line(x-(int)(w/2)+(int)(len*(1-cos(i))),y-(int)(h/2)+(int)(len*(1-sin(i))),x+(int)(w/2)-(int)(len*(1-cos(i))),y-(int)(h/2)+(int)(len*(1-sin(i))),colidx);
    fillrect(x-(int)(w/2)-1,y-(int)(h/2)+len,x+(int)(w/2)+1,y+(int)(h/2)-len,colidx);
    for(i = 10*arc;i > 0;i-=arc)
      line(x-(int)(w/2)+(int)(len*cos(i)),y+(int)(h/2)-len*(1-sin(i)),x+(int)(w/2)-(int)(len*cos(i)),y+(int)(h/2)-len*(1-sin(i)),colidx);
    */
    for(i = 0;i < 10;i++)
    { 
       j = arc_len_up(len,i);
       line(x-(int)(w/2)+(len-j),y-(h/2)+i,x+(int)(w/2)-len+j,y-(h/2)+i,colidx);
    }
    fillrect(x-(int)(w/2),y-(int)(h/2)+len,x+(int)(w/2),y+(int)(h/2)-len,colidx);
    for(i = 0;i < 10;i++)
    { 
       j = arc_len_down(len,i);
       line(x-(int)(w/2)+len-j,y+(h/2)-len+i,x+(int)(w/2)-len+j,y+(h/2)-len+i,colidx);
    } 
}
void en_draw_text32(U32 x,U32 y,const unsigned char ch[],unsigned colidx)
{
	unsigned int i, j, k;
	//unsigned char mask;
	short mask, buffer;
	for(i=0,k=0;i<16,k<32;i++,k++)
	{
		mask = 0x80;
		//buffer=(ch[i*2]<<8)|ch[i*2+1];
		buffer = ch[i * 2];
		for (j = 0; j < 8; j++) {
			if (buffer & mask) {
				pixel(x + j, y + k, colidx);
				//  PutPixel(x+j,y+k,type);
				mask = mask >> 1;
			}
			//buffer=(ch[i*2]<<8)|ch[i*2+1];
			mask = 0x80;
			buffer = ch[i * 2 + 1];
			for (j = 0; j < 8; j++) {
				if (buffer & mask) {
					pixel(x + j + 8, y + k, colidx);
					//  PutPixel(x+j+8,y+k,type);
					mask = mask >> 1;
				}
			}
		}
	}
}
void en_draw_text40(U32 x,U32 y,const unsigned char ch[],unsigned colidx)
{
	unsigned int i, j, k;
	//unsigned char mask;
	short mask, buffer;
	for (i = 0, k = 0; i < 20, k < 40; i++, k++) {
		mask = 0x80;
		//buffer=(ch[i*2]<<8)|ch[i*2+1];
		buffer = ch[i * 2];
		for (j = 0; j < 8; j++) {
			if (buffer & mask) {
				pixel(x + j, y + k, colidx);
				//  PutPixel(x+j,y+k,type);
				mask = mask >> 1;
			}
			//buffer=(ch[i*2]<<8)|ch[i*2+1];
			mask = 0x80;
			buffer = ch[i * 2 + 1];
			for (j = 0; j < 8; j++) {
				if (buffer & mask) {
					pixel(x + j + 8, y + k, colidx);
					//  PutPixel(x+j+8,y+k,type);//
					mask = mask >> 1;
				}
			}
		}
	}
}
void en_draw_text48(U32 x,U32 y,const unsigned char ch[],unsigned colidx)
{
	unsigned int i, j, k;
	//unsigned char mask;
	short mask, buffer;
	for (i = 0, k = 0; i < 48, k < 48; i++, k++) {
		mask = 0x80;
		//buffer=(ch[i*2]<<8)|ch[i*2+1];
		buffer = ch[i * 3];
		for (j = 0; j < 8; j++) {
			if (buffer & mask) {
				pixel(x + j, y + k, colidx);
				//PutPixel(x+j,y+k,type);
				mask = mask >> 1;
			}
			//buffer=(ch[i*2]<<8)|ch[i*2+1];
			mask = 0x80;
			buffer = ch[i * 3 + 1];
			for (j = 0; j < 8; j++) {
				if (buffer & mask) {
					pixel(x + j + 8, y + k, colidx);
					//PutPixel(x+j+8,y+k,type);
					mask = mask >> 1;
				}

				mask = 0x80;
				buffer = ch[i * 3 + 2];
				for (j = 0; j < 8; j++) {
					if (buffer & mask) {
						pixel(x + j + 16, y + k, colidx);
						//PutPixel(x+j+16,y+k,type);
						mask = mask >> 1;
					}

				}
			}
		}
	}
}
void en_string(int x,int y,char *string,enum size type,unsigned colidx)
{
    int i,j,n,k,m;
    unsigned char *ptr = NULL;
    n = strlen(string);
    m = sizeof(code)/sizeof(code[0]);
    for(i = 0;i < n;i++)
     {
        k = string[i];
        for(j = 0;j < m;j++)
          {
            if(code[j].ascll_code == k)
               {
                 ptr = code[j].ptr;
                 en_draw_text32(x+i*15,y,ptr,colidx);
                 break;
               }
            /*
            else
            	printf("unsupport font\n"); */
          }
     }
}
int show_bmp(int w,int h,char *bmpfile )
{
	FILE *fp;
	int rc;
	int ciBitCount, ciWidth, ciHeight;
	int line_x, line_y;
	long int location = 0, BytesPerLine = 0;
	char tmp[1024 * 10];
	union multiptr loc;

	fp = fopen(bmpfile, "rb");
	if (fp == NULL) {
		return (-1);
	}
	rc = fread(&FileHead, sizeof(BITMAPFILEHEADER), 1, fp);
	if (rc != 1) {
		printf("read header error!\n");
		fclose(fp);
		return (-2);
	}
	if (memcmp(FileHead.cfType, "BM", 2) != 0) {
		printf("it's not a BMP file\n");
		fclose(fp);
		return (-3);
	}
	rc = fread((char *) &InfoHead, sizeof(BITMAPINFOHEADER), 1, fp);
	if (rc != 1) {
		printf("read infoheader error!\n");
		fclose(fp);
		return (-4);
	}

	ciWidth = (int) chartolong(InfoHead.ciWidth, 4);
	ciHeight = (int) chartolong(InfoHead.ciHeight, 4);
	ciBitCount = (int) chartolong(InfoHead.ciBitCount, 4);

	fseek(fp, (int) chartolong(FileHead.cfoffBits, 4), SEEK_SET);
	BytesPerLine = (ciWidth * ciBitCount + 31) / 32 * 4;

	printf("width=%d, height=%d, bitCount=%d, offset=%d\n", ciWidth, ciHeight,
			ciBitCount, (int) chartolong(FileHead.cfoffBits, 4));
	line_x = line_y = 0;
	//line_x = w,line_y = h;
	bmp_w = ciWidth;
	bmp_h = ciHeight;
	unsigned short short_buf;

	while (!feof(fp)) {
		PIXEL pix;
		unsigned int tmp;
		rc = fread((char *) &pix, 1, sizeof(unsigned int), fp);
		if (rc != sizeof(unsigned int)) {
			break;
		}
		location = line_x * bytes_per_pixel
				+ (ciHeight - line_y - 1) * xres * bytes_per_pixel;

		//tmp=pix.red<<11 | pix.green<<6 | pix.blue;
		tmp = pix.red << 16 | pix.green << 8 | pix.blue | 0x00 << 24;
		//*ptr++ = pix.red;
		//*ptr++ = pix.green;
		//*ptr++ = pix.blue;
		loc.p8 = line_addr[h] + w * 4 * bytes_per_pixel + location;
		__setpixel(loc, 0, tmp);
		//*((unsigned short *)(fbp + location + w * 2 + h * xres * 2 )) = tmp;
		/*
		 short_buf=(((pix.red&0xf8)>>3)<<11)|(((pix.green&0xfc) >> 2) << 5) | ((pix.blue&0xf8) >> 3);
		 *(fbp + location + 0) = (char)(short_buf&0xff);
		 *(fbp + location + 1) = (char)((short_buf&0xff00) >> 8);
		 */
		line_x++;
		if (line_x == ciWidth) {
			line_x = 0;
			line_y++;

			if (line_y == ciHeight - 1) {
				break;
			}
		}
	}
	fclose(fp);
	//munmap(fbp,screensize);
	return (0);
}
void printscreen(void){
	int j = 0;
	char file[20];
	char *buffer = malloc(WIDTH * HEIGHT * 3);
	sprintf(file,"screen%d.jpg",j++);
	while (1) {
		if (access(file, F_OK) == 0)
			sprintf(file, "screen%d.jpg", j++);
		else
			break;
		//break;
	}
    rgb32torgb24(line_addr[0],buffer,WIDTH,HEIGHT);
    write_JPEG_file(file,buffer,HEIGHT,WIDTH);
    free(buffer);
}
void rgb32torgb24(char *src,char *tar,int w,int h){
	char *src_ptr = malloc(w * h * 4);
	char *tar_ptr = malloc(w * h * 3);
	int i,j,k;
	for(i = 0;i < h;i++){
		for(k = 0,j = 0;j < w * 4;j+=4,k+=3){
			tar[i * 2400 + k] = src[i * 3200 + j + 2];
			tar[i * 2400 + k + 1] = src[i * 3200 + j + 1];
			tar[i * 2400 + k + 2] = src[i * 3200 + j];
		}
	}
}
/******************************************************************************
*
******************************************************************************/
long chartolong( char * string, int length )
{
        long number;
        
        if (length <= 4)
        {
                memset( &number, 0x00, sizeof(long) );
                memcpy( &number, string, length );
        }
        
        return( number );
}
void printscreen_JPEG(void){
	int j = 0;
	char file[20];
	char *buffer = malloc(WIDTH * HEIGHT * 3);
	sprintf(file,"screen%d.jpg",j++);
	while (1) {
		if (access(file, F_OK) == 0)
			sprintf(file, "screen%d.jpg", j++);
		else
			break;
		//break;
	}
    rgb32torgb24(line_addr[0],buffer,WIDTH,HEIGHT);
    write_JPEG_file(file,buffer,HEIGHT,WIDTH);
    free(buffer);
}
void printscreen_PNG(void) {
	int j = 0;
	char file[20];
	char *buffer = malloc(WIDTH * HEIGHT * 3);
	sprintf(file, "screen%d.png", j++);
	while (1) {
		if (access(file, F_OK) == 0)
			sprintf(file, "screen%d.png", j++);
		else
			break;
		//break;
	}
	write_png_file(file, line_addr[0], WIDTH, HEIGHT);
}
int arc_len_up(int r,int x)
{
   int i = r*r - (r-x)*(r-x);
   return (int)sqrt(i);
}
int arc_len_down(int r,int x)
{
   int i = r*r - x*x;
   i = (int)sqrt(i);
   return i;
}
