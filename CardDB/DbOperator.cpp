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
 * File:   DbOperator.cpp
 * Author: CAI
 * Created on 2016/7/29, 9:58pm
 */

#include "DbOperator.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

DbOperator::DbOperator()
{
    bzero(m_dbFile, sizeof (m_dbFile));
    bzero(m_dbTableName, sizeof (m_dbTableName));
    db = NULL;
}

DbOperator::~DbOperator()
{
}

int DbOperator::Init(char* dbFilePath)
{
    bzero(m_dbFile, sizeof (m_dbFile));
    if (dbFilePath != NULL)
        memcpy(m_dbFile, dbFilePath, strlen(dbFilePath));
    else
        return DATABASE_FILE_PATH_ILLEGAL;
    return DATABASE_OK;
}

int DbOperator::Open()
{
    if (m_dbFile == NULL)
        return DATABASE_FILE_PATH_ILLEGAL;
    int ret = sqlite3_open(m_dbFile, &db);
    if (ret)
    {
        printf("Open database [%s] failed.Error msg is %s.\n", m_dbFile, sqlite3_errmsg(db));
        sqlite3_close(db);
        db = NULL;
        return DATABASE_BLOCK;
    }
    return DATABASE_OK;
}

int DbOperator::Close()
{
    if (db == NULL)
        return DATABASE_NOT_OPEN;
    sqlite3_close(db);
    db = NULL;
    return DATABASE_OK;
}

int DbOperator::CheckTable()
{
    int row = 0;
    int column = 0;
    char **dbResult;
    char *zErrMsg = 0;

    char strsql[100] = {0};
    sprintf(strsql, "SELECT \"%s\" FROM sqlite_master WHERE type='table';", m_dbTableName);

    int ret = sqlite3_get_table(db, strsql, &dbResult, &row, &column, &zErrMsg);
    if (ret)
    {
        printf("Open database [%s] failed.Error msg is %s.\n", m_dbFile, sqlite3_errmsg(db));
        return DATABASE_BLOCK;
    }

    if (row != 0)
    {
        for (int i = 0; i < (row + 1) * column; i++)
        {
            if (strcmp(dbResult[i], (const char*) m_dbTableName) == 0)
                return DATABASE_TABLE_EXIST;
        }
    }

    return DATABASE_TABLE_NOT_EXIST;
}

int DbOperator::ExecSql(char *sqlCmd)
{
    char *zErrMsg = 0;
    int ret;

    ret = sqlite3_exec(db, sqlCmd, NULL, NULL, &zErrMsg);

    if (ret != SQLITE_OK)
    {
        return DATABASE_EXECUTION_FAILED;
    }

    return DATABASE_OK;
}

int DbOperator::CleanAll()
{
    int ret;

    char sqlCmd[200] = {0};
    sprintf(sqlCmd, "DELETE FROM %s;", m_dbTableName);
    ret = ExecSql(sqlCmd);

    if (ret == DATABASE_EXECUTION_FAILED)
        return ret;

    char sqlCmdClear[20] = {0};
    sprintf(sqlCmdClear, "VACUUM;");
    ret = ExecSql(sqlCmdClear);

    return DATABASE_OK;
}

void DbOperator::uintReadFromStr(const char *src, unsigned int *dst)
{
    int basement = 1;
    int length = 0;
    *dst = 0;
    const char *tmp = src;
    int dstTmp = 0;
    int startPoint = 0;
    const char *minus = "-";

    while (*tmp++ != '\0')
        length++;

    if (src[0] == minus[0])
        startPoint = 1;

    basement = pow(10.0, double(length - 1 - startPoint));

    for (int i = startPoint; i < length; i++)
    {
        dstTmp += (src[i] - DECIMAL_BASE) * basement;
        basement /= 10;
    }

    if (startPoint == 1)
        dstTmp *= -1;

    *dst = dstTmp;
}

void DbOperator::ullintReadFromStr(const char *src, unsigned long long *dst)
{
    unsigned long long basement = 1;
    int length = 0;
    *dst = 0;
    const char *tmp = src;
    long long int dstTmp = 0;
    int startPoint = 0;
    const char *minus = "-";

    while (*tmp++ != '\0')
        length++;

    if (src[0] == minus[0])
        startPoint = 1;

    basement = pow(10.0, double(length - 1 - startPoint));

    for (int i = startPoint; i < length; i++)
    {
        dstTmp += (src[i] - DECIMAL_BASE) * basement;
        basement /= 10;
    }

    if (startPoint == 1)
        dstTmp *= -1;

    *dst = dstTmp;
}

void DbOperator::ucharReadFromStr(const char *src, unsigned char * dst)
{
    int basement = 1;
    int length = 0;
    *dst = 0;
    const char *tmp = src;
    char dstTmp = 0;
    int startPoint = 0;
    const char *minus = "-";

    while (*tmp++ != '\0')
        length++;

    if (src[0] == minus[0])
        startPoint = 1;

    basement = pow(10.0, double(length - 1 - startPoint));

    for (int i = startPoint; i < length; i++)
    {
        dstTmp += (src[i] - DECIMAL_BASE) * basement;
        basement /= 10;
    }

    if (startPoint == 1)
        dstTmp *= -1;

    *dst = dstTmp;
}

long long int DbOperator::overflow8Bytes(unsigned long long int src)
{
    long long int tmp;
    if ((tmp = src) < 0)
        return tmp;
    return src;
}

int DbOperator::overflow4Bytes(unsigned int src)
{
    int tmp;
    if ((tmp = src) < 0)
        return tmp;
    return src;
}

char DbOperator::overflow1Bytes(unsigned char src)
{
    char tmp;
    if ((tmp = src) < 0)
        return tmp;
    return src;
}

int DbOperator::CleanUpQueue()
{
    int ret = 0;

    char strCmd[200] = {0};

    sprintf(strCmd, "VACUUM;");

    ret = ExecSql(strCmd);
    if (ret != DATABASE_OK)
    {
        printf("Vacuum Command Failed\n");
        return DATABASE_EXECUTION_FAILED;
    }

    return DATABASE_OK;
}

int DbOperator::Count()
{
    int row = 0;
    int column = 0;
    char **dbResult;
    char *zErrMsg = 0;

    char strsql[100] = {0};
    sprintf(strsql, "SELECT count(*) FROM %s;", m_dbTableName);

    int ret = sqlite3_get_table(db, strsql, &dbResult, &row, &column, &zErrMsg);
    if (ret)
    {
        printf("Open database [%s] failed.Error msg is %s.\n", m_dbFile, sqlite3_errmsg(db));
        return DATABASE_BLOCK;
    }

    if (row != 0)
    {
        if (column != 0)
        {
            char*szcount = dbResult[0];
            int count = 0;
            sscanf(szcount, "%d", &count);
            return count;
        }
    }

    return 0;
}

int DbOperator::IsDbFile(char *dbFilePath)
{
    if (dbFilePath == NULL)
        return DATABASE_FILE_PATH_ILLEGAL;

    int ret;

#if 0
    int ret;

    ret = Init(dbFilePath);
    if (ret != DATABASE_OK)
        return ret;

    ret = Open();
    if (ret != DATABASE_OK)
        return ret;

    ret = CheckTable();
    if (ret != DATABASE_TABLE_EXIST)
    {
        return ret;
    }
    Close();
#endif

    sqlite3 *dbReplace = NULL;

    ret = sqlite3_open(dbFilePath, &dbReplace);
    if (ret)
    {
        printf("Open database [%s] failed.Error msg is %s.\n", dbFilePath, sqlite3_errmsg(dbReplace));
        sqlite3_close(dbReplace);
        dbReplace = NULL;
        return DATABASE_BLOCK;
    }

    int row = 0;
    int column = 0;
    char **dbResult;
    char *zErrMsg = 0;

    char strsql[100] = {0};
    sprintf(strsql, "SELECT \"%s\" FROM sqlite_master WHERE type='table';", m_dbTableName);

    ret = sqlite3_get_table(dbReplace, strsql, &dbResult, &row, &column, &zErrMsg);

    if (row != 0)
    {
        for (int i = 0; i < (row + 1) * column; i++)
        {
            if (strcmp(dbResult[i], (const char*) m_dbTableName) == 0)
            {
                sqlite3_close(dbReplace);
                dbReplace = NULL;
                return DATABASE_OK;
            }
        }
    }

    sqlite3_close(dbReplace);
    dbReplace = NULL;
    return DATABASE_TABLE_NOT_EXIST;
}

