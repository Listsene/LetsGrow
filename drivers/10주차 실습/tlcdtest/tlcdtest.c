

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
#include <tlcd.h>


int main(int argc , char **argv)
{

	int nCmdMode;
	int bCursorOn, bBlink, nline , nColumn;
	char strWtext[COLUMN_NUM+1];
	
	if (argc < 2 )
	{
		perror(" Args number is less than 2\n");
		doHelp();
		return 1;
	}
	
	if ( argv[1][0] == 'w' ) 
	{
		nCmdMode =  CMD_TXT_WRITE ;
		if (argc < 4 )
		{
			perror(" w argument number is short.\n");
			doHelp();
			return 1;
		}
		nline = atoi(argv[2]);
		printf("nline :%d\n",nline);
		if ( (nline != 1 ) && (nline != 2 ))
		{
			perror("line para is worng.\n");
			doHelp();
			return 1;
		}

		if (strlen(argv[3]) > COLUMN_NUM )
		{
			strncpy(strWtext,argv[3],COLUMN_NUM);
			strWtext[COLUMN_NUM] = '\0';
		}
		else
		{
			strcpy(strWtext,argv[3]);

		}
	}
	else if (  argv[1][0] == 'c' )
	{
		nCmdMode =  CMD_CURSOR_POS ;
		if ( argc < 6 ) 
		{
			perror(" c argument number is short.\n");
			doHelp();
			return 1;
		}
		bCursorOn = atoi(argv[2]);

		bBlink = atoi(argv[3]);

		nline = atoi(argv[4]);
		if ( (nline != 1 ) && (nline != 2 ))
		{
			perror("line para is worng.\n");
			doHelp();
			return 1;
		}
		nColumn = atoi(argv[5]);
		if ( nColumn >15 ) 
		{
			perror(" nColumn max number is 15.\n");
			doHelp();
			return 1;
		}
	}
	else if ( argv[1][0] == 'r' )
	{
		nCmdMode =  CMD_CEAR_SCREEN;
		if ( argc == 3 )
		{
			nline = atoi(argv[2]);
			if ( (nline != 1)&& (nline != 2))
				nline = 0;
		}
		else
			nline = 0;
	}
	else
	{
		perror(" not supported option. \n");
		doHelp();
		return 1;
	}
	
	// open  driver 
	fd = open(DRIVER_NAME,O_RDWR);
	if ( fd < 0 )
	{
		perror("driver open error.\n");
		return 1;
	}
	functionSet();

	switch ( nCmdMode )
	{
	case CMD_TXT_WRITE:
//		printf("nline:%d ,nColumn:%d\n",nline,nColumn);
		setDDRAMAddr(nColumn, nline);
		usleep(2000);
		writeStr(strWtext);
		break;
	case CMD_CURSOR_POS:
		displayMode(bCursorOn, bBlink, TRUE);
		setDDRAMAddr(nline-1, nColumn);
		break;
	case CMD_CEAR_SCREEN:
		clearScreen(nline);
		break;
	}

	close(fd);
	
	return 0;
}
