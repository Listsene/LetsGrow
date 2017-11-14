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
#include "tlcd.h"

/******************************************************************************
*
*      TEXT LCD FUNCTION
*
******************************************************************************/
int IsBusy(void)
{
	unsigned short wdata, rdata;

	wdata = SIG_BIT_RW;
	write(fd ,&wdata,2);

	wdata = SIG_BIT_RW | SIG_BIT_E;
	write(fd ,&wdata,2);

	read(fd,&rdata ,2);

	wdata = SIG_BIT_RW;
	write(fd,&wdata,2);

	if (rdata &  BUSY_BIT)
		return TRUE;

	return FALSE;
}
int writeCmd(unsigned short cmd)
{
	unsigned short wdata ;

	if ( IsBusy())
		return FALSE;

	wdata = cmd;
	write(fd ,&wdata,2);

	wdata = cmd | SIG_BIT_E;
	write(fd ,&wdata,2);

	wdata = cmd ;
	write(fd ,&wdata,2);

	return TRUE;
}

int setDDRAMAddr(int x , int y)
{
	unsigned short cmd = 0;
//	printf("x :%d , y:%d \n",x,y);
	if(IsBusy())
	{
		perror("setDDRAMAddr busy error.\n");
		return FALSE;

	}

	if ( y == 1 )
	{
		cmd = DDRAM_ADDR_LINE_1 +x;
	}
	else if(y == 2 )
	{
		cmd = DDRAM_ADDR_LINE_2 +x;
	}
	else
		return FALSE;

	if ( cmd >= 0x80)
		return FALSE;

	
//	printf("setDDRAMAddr w1 :0x%X\n",cmd);

	if (!writeCmd(cmd | SET_DDRAM_ADD_DEF))
	{
		perror("setDDRAMAddr error\n");
		return FALSE;
	}
//	printf("setDDRAMAddr w :0x%X\n",cmd|SET_DDRAM_ADD_DEF);
	usleep(1000);
	return TRUE;
}

int displayMode(int bCursor, int bCursorblink, int blcd  )
{
	unsigned short cmd  = 0;

	if ( bCursor)
	{
		cmd = DIS_CURSOR;
	}

	if (bCursorblink )
	{
		cmd |= DIS_CUR_BLINK;
	}

	if ( blcd )
	{
		cmd |= DIS_LCD;
	}

	if (!writeCmd(cmd | DIS_DEF))
		return FALSE;

	return TRUE;
}

int writeCh(unsigned short ch)
{
	unsigned short wdata =0;

	if ( IsBusy())
		return FALSE;

	wdata = SIG_BIT_RS | ch;
	write(fd ,&wdata,2);

	wdata = SIG_BIT_RS | ch | SIG_BIT_E;
	write(fd ,&wdata,2);

	wdata = SIG_BIT_RS | ch;
	write(fd ,&wdata,2);
	usleep(1000);
	return TRUE;

}


int setCursorMode(int bMove , int bRightDir)
{
	unsigned short cmd = MODE_SET_DEF;

	if (bMove)
		cmd |=  MODE_SET_SHIFT;

	if (bRightDir)
		cmd |= MODE_SET_DIR_RIGHT;

	if (!writeCmd(cmd))
		return FALSE;
	return TRUE;
}

int functionSet(void)
{
	unsigned short cmd = 0x0038; // 5*8 dot charater , 8bit interface , 2 line
	printf("inserted!!!!!!\n");

	if (!writeCmd(cmd))
		return FALSE;
	return TRUE;
}

int writeStr(char* str)
{
	printf("is it good?\n");
	unsigned char wdata;
	int i;
	for(i =0; i < strlen(str) ;i++ )
	{
		if (str[i] == '_')
			wdata = (unsigned char)' ';
		else
			wdata = str[i];
		writeCh(wdata);
	}
	return TRUE;

}

#define LINE_NUM			2
#define COLUMN_NUM			16			
int clearScreen(int nline)
{
	int i;
	if (nline == 0)
	{
		if(IsBusy())
		{	
			perror("clearScreen error\n");
			return FALSE;
		}
		if (!writeCmd(CLEAR_DISPLAY))
			return FALSE;
		return TRUE;
	}
	else if (nline == 1)
	{	
		setDDRAMAddr(0,1);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			writeCh((unsigned char)' ');
		}	
		setDDRAMAddr(0,1);

	}
	else if (nline == 2)
	{	
		setDDRAMAddr(0,2);
		for(i = 0; i <= COLUMN_NUM ;i++ )
		{
			writeCh((unsigned char)' ');
		}	
		setDDRAMAddr(0,2);
	}
	return TRUE;
}

void doHelp(void)
{
	printf("Usage:\n");
	printf("tlcdtest w line string :=>display the string  at line  , charater  '_' =>' '\n");
	printf(" ex) tlcdtest w 0 cndi_text_test :=>display 'cndi text test' at line 1 \n");
	printf("tlcdtest c on|off blink line column : \n");
	printf(" => cursor set on|off =>1 or 0 , b => blink 1|0 , line column line position \n");
	printf("tlcdtset c  1 1 2 12  :=> display blink cursor at 2 line , 12 column.\n"); 
	printf("tlcdtest r [line] : => clear screen or clear line \n");
	printf("tlcdtest r  : => clear screen \n");
	printf("tlcdtest r 1: => clear line 1 \n");
}


#define CMD_TXT_WRITE		0
#define CMD_CURSOR_POS		1
#define CMD_CEAR_SCREEN		2
