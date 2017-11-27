#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

pthread_t threads[1];
void *thread_main(void *);

int main(void)
{
	char *TLEDurl = "/tled";
	int rc;
	int status;
	
	printf("pid=%d\n", getpid());

	// for (i = 0; i < 5; i++)
	// {	
	// 	done[i] = 0;
	// 	pthread_create(&threads[i], NULL, &thread_main, (void *)i);
	// 	printf("%d, %d\n", i, threads[i]);
	// }

	pthread_create(&threads[0], NULL, &thread_main, (void *)TLEDurl);
	
	rc = pthread_join(threads[0], (void **)&status); /* detach thead에서는 사용할 필요 없다. */
	if (rc == 0){
		printf("Completed join with thread 0 status= %d\n", status);
	}
	else{
		printf("ERROR; return code from pthread_join() is %d, thread 0\n", rc);
 		return -1;
	}
	int i;
	for(i=0;i<100;i++){
		printf("This is Main\n");
		usleep(100);
	}

	return 0;
}

void *thread_main(void *arg)
{
	char *url = (char*)arg;
	int i;

	pthread_detach(pthread_self()); /* 쓰레드 분리 */

	printf("thread: %s, %d\n", url, getpid());

	for(i=0;i<100;i++){
		printf("Thread YYYYYYYYYYYYYYYYYYYYY %s", url);
		usleep(200);
	}
	pthread_exit((void *) 0);
}

