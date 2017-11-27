#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<ctype.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc, char **argv)
{
	int fd;
	int retvalue;
	int heart;
	int i = 0;
	fd = open("/dev/busled", O_RDWR);
	if( fd < 0 ){
		perror("driver open error.\n");
		return 1;
	}
    char insert = 0xFE;
    write(fd, &insert, 1);

	close(fd);
	return 1;
}

