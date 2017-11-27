#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char* sp = "1";
	printf("%c\n", sp[0]);

	char buf[3];
	char data[9] = "/soil/";
	int val = 1;
	sprintf(buf, "%d", val);
	printf("%s\n", buf);
	strcat(data, buf);

	printf("%s %d\n", data, val);

	// val=12;
	// sprintf(buf, "%d", val);
	// soil_Url = strcat("/soil/", buf);

	// printf("%s\n", soil_Url);
	// res를 가지고 DOT Matrix 수정
	return 0;
}