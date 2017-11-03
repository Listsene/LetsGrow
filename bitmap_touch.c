#include <stdio.h>
#include <stdlib.h>     // for exit
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <linux/fb.h>   // for fb_var_screeninfo, FBIOGET_VSCREENINFO
#include <string.h>
#include <linux/input.h>
#include "bitmap.h"

#define  FBDEV_FILE "/dev/fb0"

#define  INPUT_DEVICE_LIST  "/proc/bus/input/devices"

#define  EVENT_STR  "/dev/input/event"

#define  MAX_BUFF   200
#define  BIT_VALUE_24BIT   24

#define MAX_TOUCH_X 0x740
#define MAX_TOUCH_Y 0x540
#define CUSOR_THICK     10

int     screen_width;
int     screen_height;
int     bits_per_pixel;
int     line_length;

void readFirstCoordinate(int fd, int* cx , int* cy)
{
    struct input_event event;
    int readSize;
    while(1)
    {
        readSize = read(fd, &event, sizeof(event));

        if ( readSize == sizeof(event) )
        {
            if( event.type == EV_ABS )
            {
                if (event.code == ABS_MT_POSITION_X )
                {
                    *cx = event.value*screen_width/MAX_TOUCH_X; 
                }
                else if ( event.code == ABS_MT_POSITION_Y )
                {
                    *cy = event.value*screen_height/MAX_TOUCH_Y;
                }
            }
            else if ((event.type == EV_SYN) && (event.code == SYN_REPORT ))
            {
                break;
            }

        }
    }
}

void initScreen(unsigned char *fb_mem )
{
    int     coor_y;
    int     coor_x;
    unsigned long *ptr;

    for(coor_y = 0; coor_y < screen_height; coor_y++)
    {
        ptr =   (unsigned long *)fb_mem + screen_width * coor_y ;
        for(coor_x = 0; coor_x < screen_width; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
}


void drawRect(unsigned char *fb_mem , int sx , int sy, int ex, int ey,  unsigned long color)
{
    int x,y;
    unsigned long *ptr;

    for(y = sy; y < ey; y++)
    {
        ptr = (unsigned long*)fb_mem + screen_width * y ;

        for (x = sx; x < ex; x++)
        {
            *(ptr + x)  =   color;
        }   
    }
}
void drawCoordinate(unsigned char *fb_mem , int cx , int cy, int prex , int prey)
{
    int sx,sy,ex,ey;

    sx = prex - CUSOR_THICK;
    sy = prey - CUSOR_THICK;
    ex = prex + CUSOR_THICK;
    ey = prey + CUSOR_THICK;

    // erase previous cross
    if ( sx < 0 )
        sx = 0;

    if ( sy < 0 )
        sy = 0;

    if (ex >= screen_width)
        ex = screen_width - 1;

    if (ey >= screen_height)
        ey = screen_height -1;

    drawRect(fb_mem, sx, sy, ex, ey, 0x00000000);

    // draw current cross
    sx = cx - CUSOR_THICK;
    sy = cy - CUSOR_THICK;
    ex = cx + CUSOR_THICK;
    ey = cy + CUSOR_THICK;

    // erase previous cross
    if ( sx < 0 )
        sx = 0;

    if ( sy < 0 )
        sy = 0;

    if (ex >= screen_width)
        ex = screen_width - 1;

    if (ey >= screen_height)
        ey = screen_height -1;

    drawRect(fb_mem, sx, sy, ex, ey, 0xFFFFFFFF);
    printf("corX:%d, corY:%d\n",cx,cy);
}

void Init_TouchAPP(char *argv[], unsigned char **fb_mapped, int *fp, int *fb_fd){

    char eventFullPathName[100];
    int eventnum;

    struct  fb_var_screeninfo fbvar;
    struct  fb_fix_screeninfo fbfix;

    int     mem_size;

    eventnum = atoi(argv[1]);   

    sprintf(eventFullPathName,"%s%d",EVENT_STR,eventnum);

    printf("touch input event name:%s\n", eventFullPathName);

    *fp = open(eventFullPathName, O_RDONLY);
    if (-1 == *fp)
    {
        printf("%s open fail\n",eventFullPathName);
        return 1;
    }

    if( access(FBDEV_FILE, F_OK) )
    {
        printf("%s: access error\n", FBDEV_FILE);
        close(*fp);
        return 1;
    }

    if( (*fb_fd = open(FBDEV_FILE, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE);
        close(*fp);
        return 1;
    }

    if( ioctl(*fb_fd, FBIOGET_VSCREENINFO, &fbvar) )
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
        goto fb_err;
    }

    if( ioctl(*fb_fd, FBIOGET_FSCREENINFO, &fbfix) )
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
        goto fb_err;
    }

    screen_width    =   fbvar.xres;
    screen_height   =   fbvar.yres;
    bits_per_pixel  =   fbvar.bits_per_pixel;
    line_length     =   fbfix.line_length;

    printf("screen_width : %d\n", screen_width);
    printf("screen_height : %d\n", screen_height);
    printf("bits_per_pixel : %d\n", bits_per_pixel);
    printf("line_length : %d\n", line_length);

    mem_size    =   screen_width * screen_height * 4;
    *fb_mapped   =   (unsigned char *)mmap(0, mem_size,
                     PROT_READ|PROT_WRITE, MAP_SHARED, *fb_fd, 0);
    if (*fb_mapped < 0)
    {
        printf("mmap error!\n");
        goto fb_err;
    }

    initScreen(*fb_mapped);
fb_err:
    close(*fb_fd);
    close(*fp);
}

void read_bmp(char *filename, char **pDib, char **data, int *cols, int *rows)
{
    BITMAPFILEHEADER    bmpHeader;
    BITMAPINFOHEADER    *bmpInfoHeader;
    unsigned int    size;
    unsigned char   magicNum[2];
    int     nread;
    FILE    *fp;

    fp  =  fopen(filename, "rb");
    if(fp == NULL) {
        printf("ERROR\n");
        return;
    }

    // identify bmp file
    magicNum[0]   =   fgetc(fp);
    magicNum[1]   =   fgetc(fp);
    printf("magicNum : %c%c\n", magicNum[0], magicNum[1]);

    if(magicNum[0] != 'B' && magicNum[1] != 'M') {
        printf("It's not a bmp file!\n");
        fclose(fp);
        return;
    }

    nread   =   fread(&bmpHeader.bfSize, 1, sizeof(BITMAPFILEHEADER), fp);
    size    =   bmpHeader.bfSize - sizeof(BITMAPFILEHEADER);
    *pDib   =   (unsigned char *)malloc(size);      // DIB Header(Image Header)
    fread(*pDib, 1, size, fp);
    bmpInfoHeader   =   (BITMAPINFOHEADER *)*pDib;

    printf("nread : %d\n", nread);
    printf("size : %d\n", size);

    // check 24bit
    if(BIT_VALUE_24BIT != (bmpInfoHeader->biBitCount))     // bit value
    {
        printf("It supports only 24bit bmp!\n");
        fclose(fp);
        return;
    }

    *cols   =   bmpInfoHeader->biWidth;
    *rows   =   bmpInfoHeader->biHeight;
    *data   =   (char *)(*pDib + bmpHeader.bfOffBits - sizeof(bmpHeader) - 2);
    fclose(fp);
}

void close_bmp(char **pDib)     // DIB(Device Independent Bitmap)
{
    free(*pDib);
}


void Bitmap_App(char *filename){
    int i, j, k, t;
    int fbfd;
    int screen_width;
    int screen_height;
    int bits_per_pixel;
    int line_length;
    int coor_x, coor_y;
    int cols = 0, rows = 0;
    int mem_size;

    char    *pData, *data;
    char    r, g, b;
    unsigned long   bmpdata[1280*800];
    unsigned long   pixel;
    unsigned char   *pfbmap;
    unsigned long   *ptr;
    struct  fb_var_screeninfo fbvar;
    struct  fb_fix_screeninfo fbfix;

    printf("=================================\n");
    printf("Frame buffer Application - Bitmap\n");
    printf("=================================\n\n");

    read_bmp(filename, &pData, &data, &cols, &rows);
    printf("Bitmap : cols = %d, rows = %d\n", cols, rows);

    for(j = 0; j < rows; j++)
    {
        k   =   j * cols * 3;
        t   =   (rows - 1 - j) * cols;

        for(i = 0; i < cols; i++)
        {
            b   =   *(data + (k + i * 3));
            g   =   *(data + (k + i * 3 + 1));
            r   =   *(data + (k + i * 3 + 2));

            pixel = ((r<<16) | (g<<8) | b);
            bmpdata[t+i]    =   pixel;          // save bitmap data bottom up
        }
    }
    close_bmp(&pData);

    if( (fbfd = open(FBDEV_FILE, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE);
        exit(1);
    }

    if( ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar) )
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
        exit(1);
    }

    if( ioctl(fbfd, FBIOGET_FSCREENINFO, &fbfix) )
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
        exit(1);
    }

    if (fbvar.bits_per_pixel != 32)
    {
        fprintf(stderr, "bpp is not 32\n");
        exit(1);
    }

    screen_width    =   fbvar.xres;
    screen_height   =   fbvar.yres;
    bits_per_pixel  =   fbvar.bits_per_pixel;
    line_length     =   fbfix.line_length;
    mem_size    =   line_length * screen_height;
    
    printf("screen_width : %d\n", screen_width);
    printf("screen_height : %d\n", screen_height);
    printf("bits_per_pixel : %d\n", bits_per_pixel);
    printf("line_length : %d\n", line_length);

    pfbmap  =   (unsigned char *)
        mmap(0, mem_size, PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);

    if ((unsigned)pfbmap == (unsigned)-1)
    {
        perror("fbdev mmap\n");
        exit(1);
    }

    // fb clear - black
    for(coor_y = 0; coor_y < screen_height; coor_y++) {
        ptr =   (unsigned long *)pfbmap + (screen_width * coor_y);
        for(coor_x = 0; coor_x < screen_width; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }

// direction for image generating : (0,0)-> (1,0)-> (2,0)-> ...-> (row, column)
    for(coor_y = 0; coor_y < rows; coor_y++) {
        ptr =   (unsigned long*)pfbmap + (screen_width * coor_y);
        for (coor_x = 0; coor_x < cols; coor_x++) {
            *ptr++  =   bmpdata[coor_x + coor_y*cols];
        }
    }

    munmap( pfbmap, mem_size);
    close( fbfd);
}

int main(int argc, char *argv[])
{
    char *filename;
    // Bitmap_App(filename);

    // return 0;
    int x,y,prex = 0,prey = 0;
    unsigned char *fb_mapped;
    int fp ,fb_fd;

    Init_TouchAPP(argv, &fb_mapped, &fp, &fb_fd);

    while(1)
    {
        readFirstCoordinate(fp,&x, &y);
        if(x<30 && y<30){
            Bitmap_App(filename);
        }

        drawCoordinate(fb_mapped,x,y, prex, prey);
        prex = x;
        prey = y;
    }
    return 0;

}
//--------------------------------


