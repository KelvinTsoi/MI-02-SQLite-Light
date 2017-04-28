/**************************************************************************
**
**	The author disclaims copyright to this source code.
** 	In place of a legal notice, here is a bless in:
**
**	May you do good and not evil.
**	May you find forgiveness for yourself and forgive others.
**	May you share freely, never taking more than you give.
**
*************************************************************************/



/*
 * File:   Tester.h
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#ifndef _TESTER_H
#define _TESTER_H

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "../sqlite3/sqlite3.h"

#include "../Db/DbDefine.h"
#include "../Db/DbManagment.h"

#include "../CAutoLock.h"
#include "../ThreadMutex.h"

#define DB_TMP "/tmp/Lightning.db"
#define DB_BAK "/data/Lightning_bak.db"
#define REPORT_FILE_NAME "report.log"

typedef struct
{
    int sec;
    int usec;
} Stime;

typedef struct
{
    char cnt[20];
    int atime;
    int dtime;
    int mtime;
    int btime;
} ReportFile;


#define cti(cc)  (cc-('0'-0))

#define PRI_TIME_TM(format, ...) \
system("clear");    \
Gettime_usec(&endtime); \
printf(""format"", ##__VA_ARGS__); \
if (endtime.usec >= startime.usec) \
    printf("Time-Consuming = %d.%ds\n", endtime.sec - startime.sec, endtime.usec - startime.usec); \
else \
    printf("Time-Consuming = %d.%d\n", endtime.sec - startime.sec-1, 1000000+endtime.usec - startime.usec);

#define BLOCK() \
printf("Any key to continue"); \
Getch(); \
system("clear");

#define GET_TIME(zz) \
if (endtime.usec >= startime.usec) \
        zz = (endtime.sec - startime.sec)*1000+(endtime.usec - startime.usec)/1000; \
else  \
        zz = (endtime.sec - startime.sec-1)*1000+(1000000+endtime.usec - startime.usec)/1000;

#define INPUT_CARD_SERINUM() \
printf("4 Bytes Serial Number in Card: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

#define INPUT_CARD_ID() \
printf("8 Bytes Identification Number in Card: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

#define INPUT_CARD_EXPIRY() \
printf("Card expiration date\r\nthe minutes passing from 1970-01-01 00：00：00 tile now: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

#define INPUT_CARD_TYPE() \
printf("Card type\r\n0x01 signify as white list card, 0x02 signify as black list card: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

class Tester
{
public:

    Tester();

    ~Tester();

    int Process(void);

private:

    void ShowHelpInfo(void);

    void ShowAssistInfo(void);

    int Getch(void);

    void str32ToInt128(char *src, unsigned char dstVaule[16]);

    void Gettime_usec(Stime *tm);

    int CreatReport(void);

    int parseNDCmdForDb(const char *cmd);

    int parseNDAssistDb(const char *cmd);

private:
    ReportFile rptfile;
};


#endif // _TESTER_H
