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
	int fd_keymx;
	
	int prev_keymx;
	int val_keymx;

	int prev_dipsw;
	int val_dipsw;

	int fd_dipsw;

	fd_keymx = open("/dev/keymx", O_RDWR);
	fd_dipsw = open("/dev/dipsw", O_RDWR);
	

	if ( fd_keymx < 0 )
	{
		perror("driver open error.\n");
		return 1;
	}
	if ( fd_dipsw < 0 )
	{
		perror("driver open error.\n");
		return 1;
	}

	while(1){
		read(fd_keymx,&val_keymx,4);
		read(fd_dipsw,&val_dipsw,4);
		val_dipsw &= 0x1;
		val_keymx &= 0xF;

		if(val_dipsw == 1 && prev_dipsw != val_dipsw){
			prev_dipsw = 1; // 동작을 한번만 수행해라
			// ChanBin's Work // Water Split
			printf("dipsw state : %X\n", val_dipsw);
		}else if(val_dipsw == 0 && prev_dipsw != val_dipsw){
			prev_dipsw = 0; // 동작을 한번만 수행해라
			// 9 bit off
			printf("dipsw state : %X\n", val_dipsw);
		}

		if(val_keymx!=0 && prev_keymx != val_keymx){
			prev_keymx = val_keymx; // 동작을 1번만 수행해라
			printf("keymx state : %X\n", val_keymx);
		}
		// printf("\n-----------------\n\n");

		usleep(100);
	}	
	close(fd_keymx);
	close(fd_dipsw);
	return 0;	
}
