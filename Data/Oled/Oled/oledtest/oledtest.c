#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define TRUE	1
#define FALSE	0

#define DRIVER_NAME		"/dev/cnoled"

static  int  fd_oled ; 



// signal form 
//	12bit	11bit	10bit	9bit	8bit	7bit	6bit	5bit	4bit	3bit	2bit	1bit	0bit
//	RST#	CS#		D/C#	WD#		RD#		D7		D6		D5		D4		D3		D2		D1		D0
// trigger => WD or RD rising edge
/************************************************************************************************





************************************************************************************************/
#define oled_CS_BIT_MASK		0xF7FF
#define oled_DC_BIT_MASK		0xFBFF
#define oled_WD_BIT_MASK		0xFDFF
#define oled_DEFAULT_MASK	0xFFFF


#define oled_CMD_WRITE_RAM			0x5C
#define oled_CMD_LOCK				0xFD

int oled_writeCmd(int size , unsigned short* cmdArr)
{
	int i ;
	unsigned short wdata;

	//printf("wCmd : [0x%02X]",cmdArr[0]);
	//wdata = oled_CS_BIT_MASK;
	//write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK;
	write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK & oled_WD_BIT_MASK ;
	write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK & oled_WD_BIT_MASK & (cmdArr[0]|0xFF00) ;
	write(fd_oled,&wdata,2);
	
	wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK & (cmdArr[0] | 0xFF00) ;
	write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK & ( cmdArr[0] | 0xFF00);
	write(fd_oled,&wdata,2);

	for (i = 1; i < size ; i++ )
	{
	//	wdata = oled_CS_BIT_MASK ;
	//	write(fd_oled,&wdata,2);

	//	wdata = oled_CS_BIT_MASK ;
	//	write(fd_oled,&wdata,2);

		wdata = oled_CS_BIT_MASK & oled_WD_BIT_MASK ;
		write(fd_oled,&wdata,2);

		wdata = oled_CS_BIT_MASK & oled_WD_BIT_MASK & (cmdArr[i] | 0xFF00) ;
		write(fd_oled,&wdata,2);

		wdata = oled_CS_BIT_MASK & (cmdArr[i] | 0xFF00);
		write(fd_oled,&wdata,2);

	//	wdata = oled_CS_BIT_MASK & (cmdArr[i] | 0xFF00);
	//	write(fd_oled,&wdata,2);
	//	printf("[0x%02X]",cmdArr[i]);

	}
	wdata= oled_DEFAULT_MASK;
	write(fd_oled,&wdata,2);
	//printf("\n");
	return TRUE;
}

int oled_writeData(int size , unsigned char* dataArr)
{
	int i ;
	unsigned short wdata;
	
	//wdata = oled_CS_BIT_MASK;
	//write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK;
	write(fd_oled,&wdata,2);

	//wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK & oled_WD_BIT_MASK ;
	//write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK & oled_WD_BIT_MASK & (oled_CMD_WRITE_RAM | 0xFF00) ;
	write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK & oled_DC_BIT_MASK & (oled_CMD_WRITE_RAM | 0xFF00);
	write(fd_oled,&wdata,2);

	wdata = oled_CS_BIT_MASK &  (oled_CMD_WRITE_RAM | 0xFF00);
	write(fd_oled,&wdata,2);

	for (i = 0; i < size ; i++ )
	{
		wdata = oled_CS_BIT_MASK & oled_WD_BIT_MASK ;
		write(fd_oled,&wdata,2);

		wdata = oled_CS_BIT_MASK & oled_WD_BIT_MASK & ((unsigned char)dataArr[i] | 0xFF00 );
		write(fd_oled,&wdata,2);

		wdata = oled_CS_BIT_MASK & ( (unsigned char)dataArr[i] | 0xFF00);
		write(fd_oled,&wdata,2);


	}
	wdata = oled_DEFAULT_MASK;
	write(fd_oled,&wdata,2);

	return TRUE;

}

// to send cmd  , must unlock
int oled_setCmdLock(int bLock)
{
	unsigned short  cmd[3];
	
	cmd[0] = oled_CMD_LOCK;
	if (bLock)
	{
		cmd[1] = 0x16; // lock
		oled_writeCmd(2,cmd);

	}
	else
	{
		cmd[1] = 0x12; // lock
		oled_writeCmd(2,cmd);

		// A2,B1,B3,BB,BE accessible
		cmd[1] = 0xB1;
		oled_writeCmd(2,cmd);
	}
	return TRUE;
}

int oled_imageLoading(char* fileName)
{
	int imgfile;
	unsigned char* data =NULL;
	int  width , height;

	imgfile = open(fileName , O_RDONLY );
	if ( imgfile < 0 ) 
	{
		printf ("oled_imageLoading(%s)  file is not exist . err.\n",fileName);
		return FALSE;
	}
	oled_setCmdLock(FALSE);


	read(imgfile ,&width , sizeof(unsigned char));
	read(imgfile ,&height , sizeof(unsigned char));

	data = malloc( 128 * 128 * 3 );

	read(imgfile, data , 128 * 128 *3 );

	close(imgfile);

	oled_writeData(128 * 128 *3 , data );

	oled_setCmdLock(TRUE);
	return TRUE;
}

static unsigned short oled_gamma[64]= 
{
0xB8,
0x02, 0x03, 0x04, 0x05,
0x06, 0x07, 0x08, 0x09,
0x0A, 0x0B, 0x0C, 0x0D,
0x0E, 0x0F, 0x10, 0x11,
0x12, 0x13, 0x15, 0x17,
0x19, 0x1B, 0x1D, 0x1F,
0x21, 0x23, 0x25, 0x27,
0x2A, 0x2D, 0x30, 0x33,
0x36, 0x39, 0x3C, 0x3F,
0x42, 0x45, 0x48, 0x4C,
0x50, 0x54, 0x58, 0x5C,
0x60, 0x64, 0x68, 0x6C,
0x70, 0x74, 0x78, 0x7D,
0x82, 0x87, 0x8C, 0x91,
0x96, 0x9B, 0xA0, 0xA5,
0xAA, 0xAF, 0xB4

};


int oled_Init(void)
{
	unsigned short wdata[10];
	unsigned char  wcdata[10];
	int i,j;
	wdata[0]= 0xFD;
	wdata[1] = 0x12;
	oled_writeCmd(2,wdata);

	
	wdata[0] = 0xFD;
	wdata[1] = 0xB1;
	oled_writeCmd(2,wdata);

	wdata[0] = 0xAE;
	oled_writeCmd(1,wdata);

	wdata[0] = 0xB3;
	wdata[1] = 0xF1;
	oled_writeCmd(2,wdata);

	wdata[0] = 0xCA;
	wdata[1] = 0x7F;
	oled_writeCmd(2,wdata);

	wdata[0] = 0xA2;
	wdata[1] = 0x00;
	oled_writeCmd(2,wdata);

	wdata[0]= 0xA1;
	wdata[1]=0x00;
	oled_writeCmd(2,wdata);

	wdata[0]= 0xA0;
	wdata[1] = 0xB4;
	oled_writeCmd(2,wdata);

	wdata[0] = 0xAB;
	wdata[1] = 0x01;
	oled_writeCmd(2,wdata);

	wdata[0] = 0xB4;
	wdata[1] = 0xA0;
	wdata[2] = 0xB5;
	wdata[3] = 0x55;
	oled_writeCmd(4,wdata);

	wdata[0] = 0xC1;
	wdata[1] = 0xC8;
	wdata[2] = 0x80;
	wdata[3] = 0xC8;
	oled_writeCmd(4,wdata);

	wdata[0] = 0xC7;
	wdata[1] = 0x0F;
	oled_writeCmd(2,wdata);

	// oled_gamma setting 
	oled_writeCmd(64,oled_gamma);


	wdata[0] = 0xB1;
	wdata[1] = 0x32;
	oled_writeCmd(2,wdata);

	wdata[0] = 0xB2;
	wdata[1] = 0xA4;
	wdata[2] = 0x00;
	wdata[3] = 0x00;
	oled_writeCmd(4,wdata);

	wdata[0] = 0xBB;
	wdata[1] = 0x17;
	oled_writeCmd(2,wdata);

	wdata[0] = 0xB6;
	wdata[1] = 0x01;
	oled_writeCmd(2, wdata);

	wdata[0]= 0xBE;
	wdata[1] = 0x05;
	oled_writeCmd(2, wdata);

	wdata[0] = 0xA6;
	oled_writeCmd(1,wdata);
	

	for (i = 0; i < 128;i++ )
	{
		for(j = 0; j < 128; j++ )
		{
			wcdata[0]= 0x3F;
			wcdata[1]= 0;
			wcdata[2] = 0;
			oled_writeData(3,wcdata);
		}
	
	}

	wdata[0] = 0xAF;
	oled_writeCmd(1,wdata);

	return TRUE;
}
void oled_execution(char* fileName){
	oled_Init();
	oled_imageLoading(fileName);
}

int main(int argc , char **argv)
{
	// open  driver 
	fd_oled = open(DRIVER_NAME,O_RDWR);
	if ( fd_oled < 0 )
	{
		perror("driver open error.\n");
		return 1;
	}
	oled_execution("sunny2.png.img");

	close(fd_oled);
	return 0;
}
