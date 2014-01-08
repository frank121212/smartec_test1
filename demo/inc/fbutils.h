/*
 * fbutils.h
 *
 * Headers for utility routines for framebuffer interaction
 *
 * Copyright 2002 Russell King and Doug Lowder
 *
 * This file is placed under the GPL.  Please see the
 * file COPYING for details.
 *
 */

#ifndef _FBUTILS_H
#define _FBUTILS_H

#include <asm/types.h>

/* This constant, being ORed with the color index tells the library
 * to draw in exclusive-or mode (that is, drawing the same second time
 * in the same place will remove the element leaving the background intact).
 */
#define XORMODE	0x80000000
#define SIGN_WIDTH      500
#define SIGN_HEIGHT     440
#define HEIGHT 480
#define WIDTH  800
#define U32 unsigned int
enum size{
 SMALL,
 MIDDLE,
 BIG,
};
extern __u32 xres, yres;
extern unsigned char *fbuffer;
extern unsigned char **line_addr;
extern unsigned char *pic_ptr;
int x_coordinate,y_coordinate;
int pressure;
void palette_init(void);
int open_framebuffer(void);
void close_framebuffer(void);
void setcolor(unsigned colidx, unsigned value);
void put_cross(int x, int y, unsigned colidx);
void put_string(int x, int y, char *s, unsigned colidx);
void put_string_center(int x, int y, char *s,enum size type, unsigned colidx);
void en_string(int x,int y,char *string,enum size type,unsigned colidx);
void en_draw_text32(U32 x,U32 y,const unsigned char ch[],unsigned colidx);
void en_draw_text48(U32 x,U32 y,const unsigned char ch[],unsigned colidx);
void pixel (int x, int y, unsigned colidx);
void pixel_dot(int x, int y, unsigned colidx);
void mid_pixel(int x,int y,unsigned colidx);
void line (int x1, int y1, int x2, int y2, unsigned colidx);
void mid_line (int x1, int y1, int x2, int y2, unsigned colidx);
void pixel_pic (int x, int y, unsigned colidx);
void mid_pixel_pic(int x,int y,unsigned colidx);
void line_pic (int x1, int y1, int x2, int y2, unsigned colidx);
void rect (int x1, int y1, int x2, int y2, unsigned colidx);
void fillrect (int x1, int y1, int x2, int y2, unsigned colidx);
void gradient_fillrect (int x1, int y1, int x2, int y2, unsigned colidx);
void arc (int x, int y, int w, int h, unsigned colidx);
int show_bmp(int w,int h,char *bmpfile );
long chartolong( char * string, int length );
void printscreen_JPEG(void);
void printscreen_PNG(void);
#endif /* _FBUTILS_H */
