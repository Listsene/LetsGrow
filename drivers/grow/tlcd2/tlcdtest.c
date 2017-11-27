

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

static int fd;

int main()
{
	setDDRAMAddr(1, 1);
	usleep(2000);
	writeStr("temp: ");
	*/
	close(fd);
	
	return 0;
}
