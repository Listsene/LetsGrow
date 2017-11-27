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
#define SERVER_IP "192.168.43.36"

#define tlcd_TRUE   1
#define tlcd_FALSE    0
/******************************************************************************
*
*      TEXT LCD FUNCTION
*
******************************************************************************/
#define tlcd_CLEAR_DISPLAY    0x0001
#define tlcd_CURSOR_AT_HOME   0x0002

// Entry Mode set 

// Display on off
#define tlcd_DIS_DEF        0x0008
#define tlcd_DIS_LCD        0x0004
#define tlcd_DIS_CURSOR     0x0002
#define tlcd_DIS_CUR_BLINK    0x0001

// set DDRAM  address 
#define tlcd_SET_DDRAM_ADD_DEF  0x0080

// read bit
#define tlcd_BUSY_BIT     0x0080

#define tlcd_DDRAM_ADDR_LINE_1  0x0000
#define tlcd_DDRAM_ADDR_LINE_2  0x0040


#define tlcd_SIG_BIT_E      0x0400
#define tlcd_SIG_BIT_RW     0x0200
#define tlcd_SIG_BIT_RS     0x0100

#define LINE_NUM      2
#define COLUMN_NUM      16      


/***************************************************
read /write  sequence
write cycle 
RS,(R/W) => E (rise) => Data => E (fall) 

***************************************************/

int fd_keymx;
int fd_dipsw;
int fd_busled;
int fd_dotMatrix;
int fd_tlcd;


int create_tcp_socket();
// char *get_ip(char *host);
char *build_get_query(char *host, char *page);

char* Get_Client(char *pageTo);

char* getRequest();


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

char* getRequest(char* page){ // "/page"
	char *pg = page;
	char *res;
	res = Get_Client(pg);
	return res;
}

int Driver_Open_check(){
  fd_keymx = open("/dev/keymx", O_RDWR);
  fd_dipsw = open("/dev/dipsw", O_RDWR);
  fd_busled = open("/dev/busled", O_RDWR);
  fd_dotMatrix = open("/dev/Dot_matrix", O_RDWR);
  fd_tlcd = open("/dev/cntlcd",O_RDWR);


  if( fd_busled < 0 ){
    perror("driver open error.\n");
    return 1;
  }
  if ( fd_keymx < 0 ){
    perror("driver open error.\n");
    return 1;
  }
  if ( fd_dipsw < 0 ){
    perror("driver open error.\n");
    return 1;
  }
  if( fd_dotMatrix < 0 ){
    perror("driver 2 open error.\n");
    return 1;
  }
  if ( fd_tlcd < 0 ){
    perror("driver open error.\n");
    return 1;
  }

  return 0;
}

void write_BUSLED(){
  char* res = getRequest("/busled_sun");
  char insert;
  switch(res[0]){
    case '0':
      insert = 0x00;
      break;
    case '1':
      insert = 0x01;
      break;
    case '2':
      insert = 0x03;
      break;
    case '3':
      insert = 0x07;
      break;
    case '4':
      insert = 0x0F;
      break;
    case '5':
      insert = 0x1F;
      break;
    case '6':
      insert = 0x3F;
      break;
    case '7':
      insert = 0x7F;
      break;
    case '8':
      insert = 0xFF;
      break;
    default: 
      insert = 0x00;  
      break;
  }
  write(fd_busled, &insert, 1);
}

void viewDotMatrix(int *codeList, int len){

  int last = 0x00000000;
  for(int j = 0; j < 6000; j++){ // 총 3초
    for(int i = 0; i<len; i++){
      write(fd_dotMatrix, &codeList[i], 4);
      usleep(25);
    }
  }
  write(fd_dotMatrix, &last, 4);
}

void write_DOT_MATRIX(int val){
  // URL 편집 (val_keymx 전송) -> soil/1, soil/2, soil/3~나머지 전달
  char buf[3] = {0,};
  char data[13] = "/dot_soil/";
  sprintf(buf, "%d", val);
  strcat(data, buf);

  printf("dotMatrix Request %s\n", data);

  char *res = getRequest(data);
  int resCode = atoi(res);
  int *codeList;
  int i=4;

  printf("dotMatrix Response code %d\n", resCode);

  switch(resCode){
    case 0:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xefff0000;
      codeList[1] = 0xfeff0000;
      codeList[2] = 0xe0810000;
      codeList[3] = 0xe0c00000;
      break;
    case 1:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xefff0000;
      codeList[1] = 0xefff0000;
      codeList[2] = 0xefff0000;
      codeList[3] = 0xefff0000;
      break;
    case 2:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xfef80000;
      codeList[1] = 0xef87ffff;
      codeList[2] = 0xe0c90000;
      codeList[3] = 0xef87ffff;
      break;
    case 3:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xefff0000;
      codeList[1] = 0xe0c9ffff;
      codeList[2] = 0xefff0000;
      codeList[3] = 0xe0c9ffff;
      break;
    case 4:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xefffffff;
      codeList[1] = 0xfe8fffff;
      codeList[2] = 0xe088ffff;
      codeList[3] = 0xfe8fffff;
      break;
    case 5:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xe0c9ffff;
      codeList[1] = 0xfe8fffff;
      codeList[2] = 0xeff8ffff;
      codeList[3] = 0xfe8fffff;
      break;
    case 6:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xe0c4ffff;
      codeList[1] = 0xfeffffff;
      codeList[2] = 0xeffcffff;
      codeList[3] = 0xfeffffff;
      break;
    case 7:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xe081ffff;
      codeList[1] = 0xefffffff;
      codeList[2] = 0xe081ffff;
      codeList[3] = 0xefffffff;
      break;
    case 8:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xe0c9ffff;
      codeList[1] = 0xeeffffff;
      codeList[2] = 0xe0c9ffff;
      codeList[3] = 0xeeffffff;
      break;
    case 9:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xe089ffff;
      codeList[1] = 0xfe8fffff;
      codeList[2] = 0xefffffff;
      codeList[3] = 0xfe8fffff;
      break;
    case 10:
      printf("**");
      codeList = (int*)malloc(sizeof(int) * 7);
      codeList[0] = 0xf981f381;
      codeList[1] = 0xf682ed82;
      codeList[2] = 0xef84fe84;
      codeList[3] = 0xf788fd88;
      codeList[4] = 0xfb90fb90;
      codeList[5] = 0xfda0f7a0;
      codeList[6] = 0xfec0efc0;
      i = 7;
      break;
    default:
      codeList = (int*)malloc(sizeof(int) * 4);
      codeList[0] = 0xefff0000;
      codeList[1] = 0xfeff0000;
      codeList[2] = 0xe0810000;
      codeList[3] = 0xe0c00000;
      break;
  }
  viewDotMatrix(codeList, i);

  // 여기까지 정상 동작 확인
}

void write_OLED(){
  char* res = getRequest("/oled_condition");
  int count = atoi(res); // "1~100"
  char *fileName;
  if(count <= 25){
    fileName = "rainy.jpg.img";
  }else if(count <= 50){
    fileName = "blur.jpg.img";
  }else if(count <= 75){
    fileName = "sunny2.png.img";
  }else{
    fileName = "sunny.png.img";
  }
  // oled_function(fileName); 
}

int tlcd_IsBusy(void){
  unsigned short wdata, rdata;

  wdata = tlcd_SIG_BIT_RW;
  write(fd_tlcd ,&wdata,2);

  wdata = tlcd_SIG_BIT_RW | tlcd_SIG_BIT_E;
  write(fd_tlcd ,&wdata,2);

  read(fd_tlcd,&rdata ,2);

  wdata = tlcd_SIG_BIT_RW;
  write(fd_tlcd,&wdata,2);

  if (rdata &  tlcd_BUSY_BIT)
    return tlcd_TRUE;

  return tlcd_FALSE;
}

int tlcd_writeCmd(unsigned short cmd){
  unsigned short wdata ;

  if ( tlcd_IsBusy())
    return tlcd_FALSE;

  wdata = cmd;
  write(fd_tlcd ,&wdata,2);

  wdata = cmd | tlcd_SIG_BIT_E;
  write(fd_tlcd ,&wdata,2);

  wdata = cmd ;
  write(fd_tlcd ,&wdata,2);

  return tlcd_TRUE;
}

int tlcd_setDDRAMAddr(int x , int y){
  unsigned short cmd = 0;
  if(tlcd_IsBusy())  {
    perror("setDDRAMAddr busy error.\n");
    return tlcd_FALSE;
  }

  if ( y == 1 )  {
    cmd = tlcd_DDRAM_ADDR_LINE_1 +x;
  }
  else if(y == 2 )  {
    cmd = tlcd_DDRAM_ADDR_LINE_2 +x;
  }
  else
    return tlcd_FALSE;

  if ( cmd >= 0x80)
    return tlcd_FALSE;
  
  if (!tlcd_writeCmd(cmd | tlcd_SET_DDRAM_ADD_DEF))  {
    perror("tlcd_setDDRAMAddr error\n");
    return tlcd_FALSE;
  }
  usleep(1000);
  return tlcd_TRUE;
}

int tlcd_displayMode(int bCursor, int bCursorblink, int blcd){
  unsigned short cmd  = 0;
  if ( bCursor)
    cmd = tlcd_DIS_CURSOR;
  if (bCursorblink )
    cmd |= tlcd_DIS_CUR_BLINK;
  if ( blcd )
    cmd |= tlcd_DIS_LCD;
  if (!tlcd_writeCmd(cmd | tlcd_DIS_DEF))
    return tlcd_FALSE;

  return tlcd_TRUE;
}

int tlcd_writeCh(unsigned short ch){
  unsigned short wdata =0;

  if ( tlcd_IsBusy())
    return tlcd_FALSE;

  wdata = tlcd_SIG_BIT_RS | ch;
  write(fd_tlcd ,&wdata,2);

  wdata = tlcd_SIG_BIT_RS | ch | tlcd_SIG_BIT_E;
  write(fd_tlcd ,&wdata,2);

  wdata = tlcd_SIG_BIT_RS | ch;
  write(fd_tlcd ,&wdata,2);
  usleep(1000);
  return tlcd_TRUE;
}

int tlcd_writeStr(char* str){
  unsigned char wdata;
  int i;
  for(i =0; i < strlen(str) ;i++ )
  {
    if (str[i] == '_')
      wdata = (unsigned char)' ';
    else
      wdata = str[i];
    tlcd_writeCh(wdata);
  }
  return tlcd_TRUE;
}

int tlcd_clearScreen(int nline){
  int i;
  if (nline == 0)
  {
    if(tlcd_IsBusy())
    { 
      perror("clearScreen error\n");
      return tlcd_FALSE;
    }
    if (!tlcd_writeCmd(tlcd_CLEAR_DISPLAY))
      return tlcd_FALSE;
    return tlcd_TRUE;
  }
  else if (nline == 1)
  { 
    tlcd_setDDRAMAddr(0,1);
    for(i = 0; i <= COLUMN_NUM ;i++ )
    {
      tlcd_writeCh((unsigned char)' ');
    } 
    tlcd_setDDRAMAddr(0,1);

  }
  else if (nline == 2)
  { 
    tlcd_setDDRAMAddr(0,2);
    for(i = 0; i <= COLUMN_NUM ;i++ )
    {
      tlcd_writeCh((unsigned char)' ');
    } 
    tlcd_setDDRAMAddr(0,2);
  }
  return tlcd_TRUE;
}
int tlcd_functionSet()
{
  unsigned short cmd = 0x0038; // 5*8 dot charater , 8bit interface , 2 line

  if (!writeCmd(cmd))
    return FALSE;
  return TRUE;
}

void write_tlcd(char* str_Line1, char* str_Line2) {
  printf("TLCD input : %s %s\n", str_Line1, str_Line2);
  tlcd_clearScreen(0);
  tlcd_displayMode(1, 1, tlcd_TRUE);
  tlcd_setDDRAMAddr(0,1);
  usleep(2000);
  tlcd_writeStr(str_Line1);

  tlcd_setDDRAMAddr(0,2);
  usleep(2000);
  tlcd_writeStr(str_Line2);
}

void write_water_tlcd(){
  char* res = getRequest("/tlcd_water");// response "xx%"

  printf("TLCD RESPONSE : %s\n", res);

  char* str1 = "WATER AMOUNT";
  char str2[20] = "water: ";
  
  strcat(str2, res);
  write_tlcd(str1, str2);
}

void write_default_tlcd(){
  char input1[10] = {0,};
  char input2[10] = {0,};

  char* res = getRequest("/tlcd_default");// response "18C/xx%"

  printf("TLCD RESPONSE : %s\n", res);
  
  char *ptr = strtok(res, "/");
  strcpy(input1, ptr);

  ptr = strtok(NULL, "/");
  strcpy(input2, ptr);

  char str1[20] = "temp: ";
  char str2[20] = "hum: ";

  strcat(str1, input1);
  strcat(str2, input2);

  write_tlcd(str1, str2);
}

int main(int argc, char **argv) {
	
	int prev_keymx;
	int val_keymx;

	int prev_dipsw_tlcdUse;
	int val_dipsw;

  if(Driver_Open_check())
    return 1;  

  int OLED_count = 0;
  int TLCD_default = 0;
  int TLCD_water = 0;

  tlcd_functionSet();
  
	while(1){    
		read(fd_keymx,&val_keymx,4);
		read(fd_dipsw,&val_dipsw,4);
		val_dipsw &= 0x1;
		val_keymx &= 0xF;

    OLED_count++;
    if(OLED_count > 120000){ // 주기 1분 정도
      // 최적 환경과 얼마나 적합한지 보여주기 (사진)
      // write_OLED();

      OLED_count = 0;
    }

    if(prev_dipsw_tlcdUse == 0){ // default TLCD
      TLCD_default++;
      if(TLCD_default > 120000){ // 주기 1분 정도
        // 온도 습도 정보 최신화
        write_default_tlcd();

        TLCD_default = 0;
      }
    }else{ // water TLCD
      TLCD_water++;
      if(TLCD_water > 120000){ // 주기 1분 정도
        // 물 정보 최신화
        write_water_tlcd();
        // Water Split, Buzzer
        
        TLCD_water = 0;
      }
    }

		if(val_dipsw == 1 && prev_dipsw_tlcdUse != val_dipsw){
			prev_dipsw_tlcdUse = 1; // 동작을 한번만 수행해라, // TLCD에 물의 양 Show
      TLCD_water = 120000; // Do While (1회 선 수행)
			printf("dipsw state : %X\n", val_dipsw);
		}else if(val_dipsw == 0 && prev_dipsw_tlcdUse != val_dipsw){
			prev_dipsw_tlcdUse = 0; // 동작을 한번만 수행해라, // TLCD에 기본 정보 출력      
      TLCD_default = 120000; // Do While (1회 선 수행)

      write_BUSLED();

			printf("dipsw state : %X\n", val_dipsw);
		}

		if(val_keymx!=0 && prev_keymx != val_keymx){
			prev_keymx = val_keymx; // 동작을 1번만 수행해라
      printf("keymx state : %X\n", val_keymx);
      
      write_DOT_MATRIX(val_keymx); // 토양 상태 Dot Matrix로 출력

      // printf("Communication Data : %s\n", res);
		}
		// printf("\n-----------------\n\n");

    // if (Touch the Screen) then 
    // Change View

		usleep(100);
	}	
	close(fd_keymx);
	close(fd_dipsw);
  close(fd_busled);
  close(fd_tlcd);
  close(fd_dotMatrix);
	return 0;	
}
