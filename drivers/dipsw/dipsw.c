#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<ctype.h>
#include<sys/ipc.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char **argv) {
	int fd;
	int retvalue;

	fd = open("/dev/dipsw", O_RDWR);
	if ( fd < 0 )
	{
		perror("driver open error.\n");
		return 1;
	}
	
	read(fd,&retvalue,4);
	retvalue &= 0xF;
	printf("dipsw state : %X\n", retvalue);

	close(fd);
	return retvalue;
}
