
#define TRUE		1
#define FALSE		0

#define SUCCESS		0
#define FAIL		1

static int  fd;

#define DRIVER_NAME		"/dev/cntlcd"
/******************************************************************************
*
*      TEXT LCD FUNCTION
*
******************************************************************************/
#define CLEAR_DISPLAY		0x0001
#define CURSOR_AT_HOME		0x0002

// Entry Mode set 
#define MODE_SET_DEF		0x0004
#define MODE_SET_DIR_RIGHT	0x0002
#define MODE_SET_SHIFT		0x0001

// Display on off
#define DIS_DEF				0x0008
#define DIS_LCD				0x0004
#define DIS_CURSOR			0x0002
#define DIS_CUR_BLINK		0x0001

// shift
#define CUR_DIS_DEF			0x0010
#define CUR_DIS_SHIFT		0x0008
#define CUR_DIS_DIR			0x0004

// set DDRAM  address 
#define SET_DDRAM_ADD_DEF	0x0080

// read bit
#define BUSY_BIT			0x0080
#define DDRAM_ADD_MASK		0x007F


#define DDRAM_ADDR_LINE_1	0x0000
#define DDRAM_ADDR_LINE_2	0x0040


#define SIG_BIT_E			0x0400
#define SIG_BIT_RW			0x0200
#define SIG_BIT_RS			0x0100

/***************************************************
read /write  sequence
write cycle
RS,(R/W) => E (rise) => Data => E (fall)

***************************************************/
int IsBusy(void);
int writeCmd(unsigned short cmd);

int setDDRAMAddr(int x, int y);

int displayMode(int bCursor, int bCursorblink, int blcd);

int writeCh(unsigned short ch);


int setCursorMode(int bMove, int bRightDir);

int functionSet(void);

int writeStr(char* str);

#define LINE_NUM			2
#define COLUMN_NUM			16	

int clearScreen(int nline);


void doHelp(void);



#define CMD_TXT_WRITE		0
#define CMD_CURSOR_POS		1
#define CMD_CEAR_SCREEN		2
