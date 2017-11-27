#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
	int stack = 0;
	char *TLEDurl = "/tled";
	int point = 0;

	while(1){
		if(stack>100000){
			TLEDurl = "/UUUU";
			stack = 0;
			point++;
		}
		printf("%s %d \n", TLEDurl, point);
		stack++;
		usleep(100);
	}

	return 0;
}