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

#define SERVER_PORT 3000
#define USERAGENT "HTMLGET 1.0"
#define SERVER_IP "192.168.203.1"

int fd_keymx;
int fd_dipsw;
int fd_busled;

int create_tcp_socket();
// char *get_ip(char *host);
char *build_get_query(char *host, char *page);

char* Get_Client(char *pageTo);

char* getRequest_Keymx();


char* Get_Client(char *pageTo){
  struct sockaddr_in *remote;
  int sock;
  int tmpres;
  char *ip = SERVER_IP;
  char *get;
  char buf[BUFSIZ+1];
  char *parseResult;

  sock = create_tcp_socket();
  //fprintf(stderr, "IP is %s\n", ip);
  remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
  remote->sin_family = AF_INET;
  tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
  if( tmpres < 0)
  {
    perror("Can't set remote->sin_addr.s_addr");
    exit(1);
  }else if(tmpres == 0)
  {
    fprintf(stderr, "%s is not a valid IP address\n", ip);
    exit(1);
  }
  remote->sin_port = htons(SERVER_PORT);

  if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0){
    perror("Could not connect");
    exit(1);
  }
  get = build_get_query(ip, pageTo);

  //Send the query to the server
  int sent = 0;
  while(sent < strlen(get))
  {
    tmpres = send(sock, get+sent, strlen(get)-sent, 0);
    if(tmpres == -1){
      perror("Can't send query");
      exit(1);
    }
    sent += tmpres;
  }
  //now it is time to receive the page
  memset(buf, 0, sizeof(buf));
  int htmlstart = 0;
  char * htmlcontent;
  while((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0){
    if(htmlstart == 0)
    {
      /* Under certain conditions this will not work.
      * If the \r\n\r\n part is splitted into two messages
      * it will fail to detect the beginning of HTML content
      */
      htmlcontent = strstr(buf, "\r\n\r\n");
      if(htmlcontent != NULL){
        htmlstart = 1;
        htmlcontent += 4;
      }
    }else{
      htmlcontent = buf;
    }
    if(htmlstart){
      parseResult = (char*)malloc(strlen(htmlcontent));
      memcpy(parseResult, htmlcontent, strlen(htmlcontent));
    }
    memset(buf, 0, tmpres);
  }
  if(tmpres < 0)
  {
    perror("Error receiving data");
  }
  free(get);
  free(remote);
  close(sock);
  return parseResult;
}

int create_tcp_socket(){
  int sock;
  if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    perror("Can't create TCP socket");
    exit(1);
  }
  return sock;
}

char *build_get_query(char *host, char *page){
  char *query;
  char *getpage = page;
  char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
  if(getpage[0] == '/'){
    getpage = getpage + 1;
    //fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
  }
  // -5 is to consider the %s %s %s in tpl and the ending \0
  query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
  sprintf(query, tpl, getpage, host, USERAGENT);
  return query;
}

char* getRequest_Keymx(){
	char *page = "/";
	char *result;
	result = Get_Client(page);
	return result;
}
int checkFD(){
  if( fd_busled < 0 ){
    perror("driver open error.\n");
    return 1;
  }
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
}

int main(int argc, char **argv) {
	
	int prev_keymx;
	int val_keymx;

	int prev_dipsw_tlcdUse;
	int val_dipsw;

	fd_keymx = open("/dev/keymx", O_RDWR);
	fd_dipsw = open("/dev/dipsw", O_RDWR);
  fd_busled = open("/dev/busled", O_RDWR);

  if(checkFD())
    return 1;  

  int OLED_count = 0;
  int TLCD_default = 0;
  int TLCD_water = 0;
  int tlcd_use = 0; // 0 default, 1 water
	while(1){
    char *result;
		read(fd_keymx,&val_keymx,4);
		read(fd_dipsw,&val_dipsw,4);
		val_dipsw &= 0x1;
		val_keymx &= 0xF;


    OLED_count++;
    if(OLED_count > 120000){ // 주기 1분 정도
      // 최적 환경과 얼마나 적합한지 보여주기 (사진)
      OLED_count = 0;
    }

    if(prev_dipsw_tlcdUse == 0){ // default TLCD
      TLCD_default++;
      if(TLCD_default > 120000){ // 주기 1분 정도
        // 온도 습도 정보 최신화
        TLCD_default = 0;
      }
    }else{ // water TLCD
      TLCD_water++;
      if(TLCD_water > 120000){ // 주기 1분 정도
        // 물 정보 최신화
        // ChanBin's Work // Water Split, Buzzer
        TLCD_water = 0;
      }
    }


		if(val_dipsw == 1 && prev_dipsw != val_dipsw){
			prev_dipsw_tlcdUse = 1; // 동작을 한번만 수행해라, // TLCD에 물의 양 Show
      TLCD_water = 120000; // Do While (1회 선 수행)
			printf("dipsw state : %X\n", val_dipsw);
		}else if(val_dipsw == 0 && prev_dipsw != val_dipsw){
			prev_dipsw_tlcdUse = 0; // 동작을 한번만 수행해라, // TLCD에 기본 정보 출력      
      TLCD_default = 120000; // Do While (1회 선 수행)
      // 햋빛 뷰 동작
			printf("dipsw state : %X\n", val_dipsw);
		}

		if(val_keymx!=0 && prev_keymx != val_keymx){
			prev_keymx = val_keymx; // 동작을 1번만 수행해라
			printf("keymx state : %X\n", val_keymx);
      result = getRequest_Keymx();
      printf("Communication Data : %s\n", result);
		}
		// printf("\n-----------------\n\n");

    // if (Touch the Screen) then 
    // Change View

		usleep(100);
	}	
	close(fd_keymx);
	close(fd_dipsw);
	return 0;	
}
