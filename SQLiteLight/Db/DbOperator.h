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
 * File:   DbOperator.h
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#ifndef DBOPERATOR_H
#define DBOPERATOR_H
#include "DbDefine.h"
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
#include <sys/wait.h>

#include "../sqlite3/sqlite3.h"

#include "../CAutoLock.h"

class DbOperator
{
public:

    /**
     * Constructor;
     */
    DbOperator();

    /**
     * Destructor;
     */
    virtual ~DbOperator();

    /**
     * @function: module initialization;
     * @parameter: dbFile date base save path;
     * @return： success return 0, others signify function error code;
     */
    virtual int Init(char* dbFile);

    /**
     * @function: create date table;
     * @return: success return 0, others signify function error code;
     */
    virtual int CreateTable() = 0;

    /**
     * @function: open date base;
     * @return: success return 0, others signify function error code;
     */
    virtual int Open();

    /**
     * @function: close date base;
     * @return: success return 0, others signify function error code;
     */
    virtual int Close();

    /**
     * @function: asynchronous clean up date base;
     * @return: success return 0, others signify function error code;
     */
    virtual int CleanAll();

    /**
     * @function: check the table date structure is correct or not;
     * @return: success return 0, others signify function error code;
     */
    virtual int CheckTable();

    /**
     * @function: clean up date base buffer free list;
     * @return: success return 0, others signify function error code;
     */
    int CleanUpQueue();

    /**
     * @function: counting records of table;
     * @parameter: amount sum of record in database;
     * @return: success return 0, others signify function error code;
     */
    virtual int Count(unsigned int &amount);

    /**
     * @function: check out the input date base is illegal or not;
     * @parameter: dbFilePath path of date base;
     * @value: success return 0, others signify function error code;
     */
    int IsDbFile(char *dbFilePath);


protected:

    /**
     * @function: execute SQL statement and return relative results;
     * @parameter: SQL statement string;
     * @return: success return 0, others signify function error code;
     */
    int ExecSql(char *sqlCmd);

    /**
     * @function: char* & unsigned int conversion;
     * @parameter: src char* type of data;
     * @parameter: dst unsigned int type of date;
     * @return: success return 0, others signify function error code;
     */
    void uintReadFromStr(const char *src, unsigned int *dst);

    /**
     * @function: char* & unsigned long long conversion;
     * @parameter: src char* type of data;
     * @parameter: dst unsigned long long type of data;
     * @return: success return 0, others signify function error code;
     */
    void ullintReadFromStr(const char *src, unsigned long long int *dst);

    /**
     * @function: char* & unsigned char conversion;
     * @parameter: src char* type of data;
     * @parameter: dst unsigned char type of data;
     * @return: success return 0, others signify function error code;
     */
    void ucharReadFromStr(const char *src, unsigned char * dst);

    /**
     * @function: check out unsigned char value is overflow(1 Byte signed char) or not；
     * @parameter: src unsigned char type of data;
     * @return: success return 0, others signify function error code;
     */
    char overflow1Bytes(unsigned char src);


    /**
     * @function: check out unsigned int value is overflow(4 Byte signed int) or not；
     * @parameter: src unsigned int type of date;
     * @return: success return 0, others signify function error code;
     */
    int overflow4Bytes(unsigned int src);

    /**
     * @function: check out unsigned long long value is overflow(8 Byte signed long long ) or not；
     * @parameter: src unsigned long long type of date;
     * @return: success return 0, others signify function error code;
     */
    long long int overflow8Bytes(unsigned long long int src);

protected:

    /**
     * sqlite date base handle;
     */
    sqlite3 *db;

    /**
     * path of data base;
     */
    char m_dbFile[100];

    /**
     * table name of data base;
     */
    char m_dbTableName[20];
};

#endif /* DBOPERATOR_H */

