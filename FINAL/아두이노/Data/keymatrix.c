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

	fd = open("/dev/keymx", O_RDWR);
	if ( fd < 0 )
	{
		perror("driver open error.\n");
		return 1;
	}
	while(1){
		read(fd,&retvalue,4);
		retvalue &= 0xF;
		if(retvalue!=0){
			printf("keymx state : %X\n", retvalue);
		}else if(retvalue == 0xA){
			break;
		}
	}	
	close(fd);
	return retvalue;
}
