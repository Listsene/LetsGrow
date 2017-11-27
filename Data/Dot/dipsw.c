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


int main(int argc, char **argv)
{
	int fd;
	int fa;
	int retvalue;
	int heart;
	int i = 0;
	fa = open("/dev/Dot_matrix", O_RDWR);
	if( fa < 0 ){
                perror("driver 2 open error.\n");
                return 1;
        }	
	fd = open("/dev/dipsw",O_RDWR);
	if( fd < 0 ){
		perror("driver open error.\n");
		return 1;
	}
	read(fd, &retvalue, 4);
	if ((int)retvalue == 0){
                for( i = 0; i < 10000; i++){
                        heart = 0xefff0000;
                        write(fa, &heart, 4);
                        usleep(10);
			heart = 0xfeff0000;
                        write(fa, &heart, 4);
                        usleep(10);
			heart = 0xe0810000;
                        write(fa, &heart, 4);
                        usleep(10);
			heart = 0xe0c00000;
                        write(fa, &heart, 4);
                        usleep(10);

                }
        }

	else if ((int)retvalue == 1){
		for( i = 0; i < 10000; i++){
			heart = 0xefff0000;
                	write(fa, &heart, 4);
                	usleep(10);
		}	
	}
	else if ((int)retvalue == 2 ){
                for( i = 0; i < 10000; i++){
                        heart = 0xef87ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xfef80000;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xe0c90000;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 3 ){
                for( i = 0; i < 10000; i++){
                        heart = 0xefff0000;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xe0c9ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 4){
                for( i = 0; i < 10000; i++){
                        heart = 0xefffffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xfe8fffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xe088ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 5){
                for( i = 0; i < 10000; i++){
                        heart = 0xe0c9ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xfe8fffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xeff8ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }	
	else if ((int)retvalue == 6){
                for( i = 0; i < 10000; i++){
                        heart = 0xe0c4ffff;
	                write(fa, &heart, 4);
       		        usleep(10);
                	heart = 0xfeffffff;
                	write(fa, &heart, 4);
                	usleep(10);
                	heart = 0xeffcffff;
                	write(fa, &heart, 4);
                	usleep(10);
                }
        }
	else if ((int)retvalue == 7){
                for( i = 0; i < 10000; i++){
                        heart = 0xe081ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xefffffff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 8){
                for( i = 0; i < 10000; i++){
                        heart = 0xe0c9ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xeeffffff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 9){
                for( i = 0; i < 10000; i++){
                        heart = 0xe089ffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xfe8fffff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xefffffff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 10 ){
                for( i = 0; i < 10000; i++){
                        heart = 0xefffefff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xfeffefff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xe081efff;
                        write(fa, &heart, 4);
                        usleep(10);
			heart = 0xe0c0efff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 11){
                for( i = 0; i < 10000; i++){
                        heart = 0xeffff7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 12){
                for( i = 0; i < 10000; i++){
                        heart = 0xef87f7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xfef8f7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xe0c9f7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 13){
                for( i = 0; i < 10000; i++){
                        heart = 0xeffff7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xe0c9f7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ((int)retvalue == 14){
                for( i = 0; i < 10000; i++){
                        heart = 0xeffff7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xfe8ff7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                        heart = 0xe088f7ff;
                        write(fa, &heart, 4);
                        usleep(10);
                }
        }
	else if ( (int)retvalue == 15){
		for (i = 0; i< 10000; i++){ 
		heart = 0xf981f381;
		write(fa, &heart, 4);
                usleep(10);
		heart = 0xf682ed82;
                write(fa, &heart, 4);
                usleep(10);
		heart = 0xef84fe84;
                write(fa, &heart, 4);
                usleep(10);
		heart = 0xf788fd88;
                write(fa, &heart, 4);
                usleep(10);
		heart = 0xfb90fb90;
                write(fa, &heart, 4);
                usleep(10);
		heart = 0xfda0f7a0;
                write(fa, &heart, 4);
                usleep(10);
		heart = 0xfec0efc0;
                write(fa, &heart, 4);
                usleep(10);
		}
	}
	heart = 0x00000000;
        write(fa, &heart, 4);
	close(fd);
	close(fa);
	return retvalue;
}
