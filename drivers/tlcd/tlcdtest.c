

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define tlcd_TRUE		1
#define tlcd_FALSE		0
/******************************************************************************
*
*      TEXT LCD FUNCTION
*
******************************************************************************/
#define tlcd_CLEAR_DISPLAY		0x0001
#define tlcd_CURSOR_AT_HOME		0x0002

// Entry Mode set 

// Display on off
#define tlcd_DIS_DEF				0x0008
#define tlcd_DIS_LCD				0x0004
#define tlcd_DIS_CURSOR			0x0002
#define tlcd_DIS_CUR_BLINK		0x0001

// set DDRAM  address 
#define tlcd_SET_DDRAM_ADD_DEF	0x0080

// read bit
#define tlcd_BUSY_BIT			0x0080

#define tlcd_DDRAM_ADDR_LINE_1	0x0000
#define tlcd_DDRAM_ADDR_LINE_2	0x0040


#define tlcd_SIG_BIT_E			0x0400
#define tlcd_SIG_BIT_RW			0x0200
#define tlcd_SIG_BIT_RS			0x0100

/***************************************************
read /write  sequence
write cycle 
RS,(R/W) => E (rise) => Data => E (fall) 

***************************************************/
int tlcd_IsBusy(void){
	unsigned short wdata, rdata;

	wdata = tlcd_SIG_BIT_RW;
	write(fd ,&wdata,2);

	wdata = tlcd_SIG_BIT_RW | tlcd_SIG_BIT_E;
	write(fd ,&wdata,2);

	read(fd,&rdata ,2);

	wdata = tlcd_SIG_BIT_RW;
	write(fd,&wdata,2);

	if (rdata &  tlcd_BUSY_BIT)
		return tlcd_TRUE;

	return tlcd_FALSE;
}
int tlcd_writeCmd(unsigned short cmd){
	unsigned short wdata ;

	if ( tlcd_IsBusy())
		return tlcd_FALSE;

	wdata = cmd;
	write(fd ,&wdata,2);

	wdata = cmd | tlcd_SIG_BIT_E;
	write(fd ,&wdata,2);

	wdata = cmd ;
	write(fd ,&wdata,2);

	return tlcd_TRUE;
}

int tlcd_setDDRAMAddr(int x , int y)
{
	unsigned short cmd = 0;
//	printf("x :%d , y:%d \n",x,y);
	if(tlcd_IsBusy())
	{
		perror("setDDRAMAddr busy error.\n");
		return tlcd_FALSE;

	}

	if ( y == 1 )
	{
		cmd = tlcd_DDRAM_ADDR_LINE_1 +x;
	}
	else if(y == 2 )
	{
		cmd = tlcd_DDRAM_ADDR_LINE_2 +x;
	}
	else
		return tlcd_FALSE;

	if ( cmd >= 0x80)
		return tlcd_FALSE;

	
//	printf("tlcd_setDDRAMAddr w1 :0x%X\n",cmd);

	if (!tlcd_writeCmd(cmd | tlcd_SET_DDRAM_ADD_DEF))
	{
		perror("tlcd_setDDRAMAddr error\n");
		return tlcd_FALSE;
	}
//	printf("tlcd_setDDRAMAddr w :0x%X\n",cmd|tlcd_SET_DDRAM_ADD_DEF);
	usleep(1000);
	return tlcd_TRUE;
}

int tlcd_displayMode(int bCursor, int bCursorblink, int blcd  )
{
	unsigned short cmd  = 0;

	if ( bCursor)
	{
		cmd = tlcd_DIS_CURSOR;
	}

	if (bCursorblink )
	{
		cmd |= tlcd_DIS_CUR_BLINK;
	}

	if ( blcd )
	{
		cmd |= tlcd_DIS_LCD;
	}

	if (!tlcd_writeCmd(cmd | tlcd_DIS_DEF))
		return tlcd_FALSE;

	return tlcd_TRUE;
}

int tlcd_writeCh(unsigned short ch)
{
	unsigned short wdata =0;

	if ( tlcd_IsBusy())
		return tlcd_FALSE;

	wdata = tlcd_SIG_BIT_RS | ch;
	write(fd ,&wdata,2);

	wdata = tlcd_SIG_BIT_RS | ch | tlcd_SIG_BIT_E;
	write(fd ,&wdata,2);

	wdata = tlcd_SIG_BIT_RS | ch;
	write(fd ,&wdata,2);
	usleep(1000);
	return tlcd_TRUE;

}

int tlcd_writeStr(char* str)
{
	unsigned char wdata;
	int i;
	for(i =0; i < strlen(str) ;i++ )
	{
		if (str[i] == '_')
			wdata = (unsigned char)' ';
		else
			wdata = str[i];
		tlcd_writeCh(wdata);
	}
	return tlcd_TRUE;

}

#define LINE_NUM			2
#define COLUMN_NUM			16			
int tlcd_clearScreen(int nline)
{
	int i;
	if (nline == 0)
	{
		if(tlcd_IsBusy())
		{	
			perror("clearScreen error\n");
			return tlcd_FALSE;
		}
		if (!tlcd_writeCmd(tlcd_CLEAR_DISPLAY))
			return tlcd_FALSE;
		return tlcd_TRUE;
	}
	else if (nline == 1)
	{	
		tlcd_setDDRAMAddr(0,1);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			tlcd_writeCh((unsigned char)' ');
		}	
		tlcd_setDDRAMAddr(0,1);

	}
	else if (nline == 2)
	{	
		tlcd_setDDRAMAddr(0,2);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			tlcd_writeCh((unsigned char)' ');
		}	
		tlcd_setDDRAMAddr(0,2);
	}
	return tlcd_TRUE;
}

void write_tlcd() {
	tlcd_clearScreen(0);
	tlcd_displayMode(1, 1, tlcd_TRUE);
	tlcd_setDDRAMAddr(0,1);
	usleep(2000);
	tlcd_writeStr("temp: 18C");

	tlcd_setDDRAMAddr(0,2);
	usleep(2000);
	tlcd_writeStr("hum: xx%");
}

int main(int argc , char **argv)
{
	fd_tlcd = open("/dev/cntlcd",O_RDWR);
	if ( fd_tlcd < 0 )
	{
		perror("driver open error.\n");
		return 1;
	}

	write_tlcd();
	close(fd);
	
	return 0;
}
