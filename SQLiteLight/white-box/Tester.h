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
#include <signal.h>

#include "../sqlite3/sqlite3.h"

#include "../Db/DbDefine.h"
#include "../Db/DbManagment.h"

#include "../CAutoLock.h"
#include "../ThreadMutex.h"

#define DB_TMP "/tmp/Lightning.db"
#define DB_BAK "Lightning_bak.db"
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


//Switch to Control Parallel Testing Module
extern bool permitToExecuteParallelTest;

//latency time in parallel testing module
#define PAUSETIME   50000

//coefficient to limit the rand range
#define RAND_LIMIT  32768

//characters transition
#define cti(cc)  (cc-('0'-0))

//debugging information for parallel testing module
#define PRI_TIME_TMOFRT(format, ...) \
Gettime_usec(&endtime); \
printf(""format"", ##__VA_ARGS__); \
if (endtime.usec >= startime.usec) \
    printf("Time-Consuming = %d.%ds\n", endtime.sec - startime.sec, endtime.usec - startime.usec); \
else \
    printf("Time-Consuming = %d.%d\n", endtime.sec - startime.sec-1, 1000000+endtime.usec - startime.usec);

//debugging information
#define PRI_TIME_TM(format, ...) \
system("clear");    \
Gettime_usec(&endtime); \
printf(""format"", ##__VA_ARGS__); \
if (endtime.usec >= startime.usec) \
    printf("Time-Consuming = %d.%ds\n", endtime.sec - startime.sec, endtime.usec - startime.usec); \
else \
    printf("Time-Consuming = %d.%d\n", endtime.sec - startime.sec-1, 1000000+endtime.usec - startime.usec);

//voluntary waiting
#define BLOCK() \
printf("Any key to continue"); \
Getch(); \
system("clear");

//get milliseconds
#define GET_TIME(zz) \
if (endtime.usec >= startime.usec) \
        zz = (endtime.sec - startime.sec)*1000+(endtime.usec - startime.usec)/1000; \
else  \
        zz = (endtime.sec - startime.sec-1)*1000+(1000000+endtime.usec - startime.usec)/1000;

//reduction input code
#define INPUT_INFO_SEQENCE() \
printf("4 Bytes Serial Number in Data: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

//reduction input code
#define INPUT_INFO_EXPIRY() \
printf("Data expiration date\r\nthe minutes passing from 1970-01-01 00：00：00 tile now: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

//reduction input code
#define INPUT_INFO_TYPE() \
printf("Data type\r\n0x01 signify as white list data, 0x02 signify as black list data: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

//reduction input code
#define INPUT_INFO_DATA() \
printf("8 Bytes Identification Number in Data: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

//reduction input code
#define INPUT_INFO_CONTENT() \
printf("128 Bytes String Content in Data: "); \
memset(pcmd, 0, sizeof(pcmd)); \
gets(pcmd);

class Tester
{
public:

    /**
     * Summary: Class Tester's Constructor
     */
    Tester();

    /**
     * Summary: Class Tester's Destructor
     */
    ~Tester();

    /**
     * Summary: White-Box Module Entrance
     * Return: Return code has not any practical significance
     */
    int Process(void);

private:

    /**
     * Summary: Main Menu, which control nearly all interface about DataBase
     * Parameters:
     *  cmd: Command from console
     * Return: Return zero if function success, other values signify function error code
     */
    int MLevelMenu(const char *cmd);

    /**
     * Summary: Assistance Menu, which content Automatic testing and Parallel testing Module
     * Parameters:
     *  cmd: Command from console
     * Return: Return zero if function success, other values signify function error code
     */
    int ALevelMenu(const char *cmd);

    /**
     * Summary: Main Menu Print Info
     */
    void ShowHelpInfo(void);

    /**
     * Summary: Main Menu Print Info
     */
    void ShowAssistInfo(void);

private:

    /**
     * Summary: Parallel Testing Module Entrance
     */
    void ParallelStress(void);

    /**
     * Summary: Callback function for Parallel Adding Module
     * Parameters:
     *  param: Used for transmit current Instance
     * Return: Return code has not any practical significance
     */
    static void* RunParallelAddingThreadProc(void* param);

    /**
     * Summary: Callback function for Parallel Finding Module
     * Parameters:
     *  param: Used for transmit current Instance
     * Return: Return code has not any practical significance
     */
    static void* RunParallelFindingThreadProc(void* param);

    /**
     * Summary: Callback function for Parallel Deleting Module
     * Parameters:
     *  param: Used for transmit current Instance
     * Return: Return code has not any practical significance
     */
    static void* RunParallelDeletingThreadProc(void* param);

    /**
     * Summary: Callback function for Parallel Baking Module
     * Parameters:
     *  param: Used for transmit current Instance
     * Return: Return code has not any practical significance
     */
    static void* RunParallelBakingThreadProc(void* param);

    /**
     * Summary: Executive function for Parallel Adding Module
     */
    void ParallelAdding(void);

    /**
     * Summary: Executive function for Parallel Finding Module
     */
    void ParallelFinding(void);

    /**
     * Summary: Executive function for Parallel Deleting Module
     */
    void ParallelDeleting(void);

    /**
     * Summary: Executive function for Parallel Baking Module
     */
    void ParallelBaking(void);

private:

    /**
     * Summary: Grab input from console
     * Return: Return zero if function success, other values signify function error code
     */
    int Getch(void);

    /**
     * Summary: Get Current Second and Millisecond
     * Parameters:
     *  tm: Structure used for store Second and Millisecond
     */
    void Gettime_usec(Stime *tm);


    /**
     * Summary: Get Current Second
     * Return: Return Current Second
     */
    unsigned int GetNowTime(void);


    /**
     * Summary: Get Current Microsecond
     * Return: Return Current Microsecond
     */
    unsigned int GetNowUs(void);


    /**
     * Summary: Create report for Automatic Testing Module
     * Return: Return zero if function success, other values signify function error code
     */
    int CreatReport(void);


    /**
     * Summary: Get random Serial Number
     * Return: Calculated Serial Number
     */
    unsigned int CreateRandomSerialNumber(void);


    /**
     * Summary: Get random Identification Number
     * Return: Calculated Identification Number
     */
    unsigned int CreateRandomIdentificationNumber(void);


    /**
     * Summary: Transform md5 value from String To Char[]
     * Parameters:
     *  src: source string store md5 value
     *  dstVaule: md5 value store in 16Bytes array
     */
    void str32ToInt128(char *src, unsigned char dstVaule[16]);


    /**
     * Summary: Signal Trigger
     * Parameters:
     *  sig: semaphore
     */
    static void Ouch(int sig);

private:

    //Structure used for store execution time from Automatic Testing Module
    ReportFile rptfile;

    //Adding Thread for Parallel Testing Module
    pthread_t thread_add;

    //Finding Thread for Parallel Testing Module
    pthread_t thread_find;

    //Deleting Thread for Parallel Testing Module
    pthread_t thread_delete;

    //Backup Thread for Parallel Testing Module
    pthread_t thread_backup;

    //Semaphore
    struct sigaction act;
};


#endif // _TESTER_H
