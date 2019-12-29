#ifndef SD_H
#define SD_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils/cmdline.h"
#include "utils/uartstdio.h"
#include "third_party/fatfs/src/ff.h"
#include "third_party/fatfs/src/diskio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "grlib/grlib.h"

void SD_Init(void);
int SD_Test(void);

#define PATH_BUF_SIZE           80
#define CMD_BUF_SIZE            64
#define FRESULT_ENTRY(f)        { (f), (#f) }
#define NUM_FRESULT_CODES (sizeof(g_sFresultStrings) / sizeof(tFresultString))
	

static char g_cCwdBuf[PATH_BUF_SIZE] = "/";
static char g_cTmpBuf[PATH_BUF_SIZE];
static char g_cCmdBuf[CMD_BUF_SIZE];

static FATFS g_sFatFs={0};
static DIR g_sDirObject;
static FILINFO g_sFileInfo;
static FIL g_sFileObject;


int Cmd_help(int argc, char *argv[]);
int Cmd_ls(int argc, char *argv[]);
int Cmd_cd(int argc, char *argv[]);
int Cmd_pwd(int argc, char *argv[]);
int Cmd_cat(int argc, char *argv[]);



typedef struct
{
    FRESULT fresult;
    char *pcResultStr;
}
tFresultString;




#endif
