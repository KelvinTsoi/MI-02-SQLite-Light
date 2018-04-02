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
 * Created on 2017/4/27, 10:00pm
 */

#include "DbOperator.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

extern const char *g_getTableStructTemplate;
extern const char *g_replaceRecordTemplate;
extern const char *g_insertRecordTemplate;
extern const char *g_updateRecordTemplate;
extern const char *g_deleteRecordTemplate;

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
        printf("Open database [%s] failed.Error msg is %s.\r\n", m_dbFile, sqlite3_errmsg(db));
        sqlite3_close(db);
        db = NULL;
        return DATABASE_BLOCK;
    }
    
    GetDbInfo();
    CommandGenerator();
    
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

    char strsql[128] = {0};
    sprintf(strsql, "SELECT \"%s\" FROM sqlite_master WHERE type='table';", m_dbTableName);

    int ret = sqlite3_get_table(db, strsql, &dbResult, &row, &column, &zErrMsg);
    if (ret)
    {
        printf("Open database [%s] failed.Error msg is %s.\r\n", m_dbFile, sqlite3_errmsg(db));
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

int DbOperator::ReLoadInfo()
{
    GetDbInfo();
    CommandGenerator();
    return DATABASE_OK;
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

    char sqlCmd[256] = {0};
    sprintf(sqlCmd, "DELETE FROM %s;", m_dbTableName);
    ret = ExecSql(sqlCmd);

    if (ret == DATABASE_EXECUTION_FAILED)
        return ret;

    char sqlCmdClear[32] = {0};
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
        printf("Vacuum Command Failed.\r\n");
        return DATABASE_EXECUTION_FAILED;
    }

    return DATABASE_OK;
}

int DbOperator::Count(unsigned int &count)
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
        printf("Open database [%s] failed.Error msg is %s.\r\n", m_dbFile, sqlite3_errmsg(db));
        return DATABASE_BLOCK;
    }

    if (row != 0)
    {
        if (column != 0)
        {
            char*szcount = dbResult[1];
            sscanf(szcount, "%d", &count);
            return 0;
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

int DbOperator::GetDbInfo()
{
    char strCmd[200] = {0};
    memset(strCmd, 0x00, sizeof(strCmd));
    sprintf(strCmd, g_getTableStructTemplate, m_dbTableName);

    char *zErrMsg = 0;
    m_tableInfo.struct_number = 0;
    int ret = sqlite3_exec(db, strCmd, loadTableInfoCallBack, (void *)this, &zErrMsg);
    if (ret != SQLITE_OK)
    {
        printf("Get database info Error, Code: %d, Result: %s\r\n", ret, zErrMsg);
        return 1;
    }

    /*
    printf("File Name: %s Table Name: %s Table Member Count: %d\r\n", m_dbFile, m_dbTableName, m_tableInfo.struct_number);
    for(int i = 0; i < m_tableInfo.struct_number; i++)
    {
        printf("Line %d: %d, %s, %s, %d, %s, %d\r\n",
               i, m_tableInfo.struct_info[i].s_cid, m_tableInfo.struct_info[i].s_name,
               m_tableInfo.struct_info[i].s_type, m_tableInfo.struct_info[i].s_notNull,
               m_tableInfo.struct_info[i].s_defaultValue, m_tableInfo.struct_info[i].s_pk);
    }
    */
    return 0;
}

int DbOperator::loadTableInfoCallBack(void* param, int nCount, char** pValue, char** pName)
{
    DbOperator* pDbOperator = (DbOperator*) param;
    pDbOperator->loadTableInfo(nCount, pValue, pName);
    return 0;
}

int DbOperator::loadTableInfo(int nCount, char** pValue, char** pName)
{
    m_tableInfo.struct_info[m_tableInfo.struct_number].s_cid = atoi(pValue[0]);
    memset(m_tableInfo.struct_info[m_tableInfo.struct_number].s_name, 0x00, sizeof(m_tableInfo.struct_info[m_tableInfo.struct_number].s_name));
    strcpy(m_tableInfo.struct_info[m_tableInfo.struct_number].s_name, pValue[1]);
    memset(m_tableInfo.struct_info[m_tableInfo.struct_number].s_type, 0x00, sizeof(m_tableInfo.struct_info[m_tableInfo.struct_number].s_type));
    strcpy(m_tableInfo.struct_info[m_tableInfo.struct_number].s_type, pValue[2]);
    m_tableInfo.struct_info[m_tableInfo.struct_number].s_notNull = atoi(pValue[3]);
    memset(m_tableInfo.struct_info[m_tableInfo.struct_number].s_defaultValue, 0x00, sizeof(m_tableInfo.struct_info[m_tableInfo.struct_number].s_defaultValue));
    if(pValue[4] != NULL)
    {
        strcpy(m_tableInfo.struct_info[m_tableInfo.struct_number].s_defaultValue, pValue[4]);
    }
    else
    {
        strcpy(m_tableInfo.struct_info[m_tableInfo.struct_number].s_defaultValue, "NULL");
    }
    m_tableInfo.struct_info[m_tableInfo.struct_number].s_pk = atoi(pValue[5]);
    m_tableInfo.struct_number++;
    return 0;
 }

int DbOperator::CommandGenerator()
{
    if(m_tableInfo.struct_number == 0)
    {
        return 1;
    }
    
    char content1[128] = {0x00};
    char content2[128] = {0x00};

    memset(content1, 0x00, sizeof(content1));
    memset(content2, 0x00, sizeof(content2));
    for(int i = 0; i < m_tableInfo.struct_number; i++)
    {
        if(i != 0)
        {
            strcat(content1, ",");
            strcat(content2, ",");
        }
        strcat(content1, m_tableInfo.struct_info[i].s_name);

        if(strcmp(m_tableInfo.struct_info[i].s_type, "INT") == 0 || strcmp(m_tableInfo.struct_info[i].s_type, "TINYINT") == 0
                || strcmp(m_tableInfo.struct_info[i].s_type, "BIT") == 0)
        {
            strcat(content2, "%d");
        }
        else if(strcmp(m_tableInfo.struct_info[i].s_type, "BIGINT") == 0)
        {
            strcat(content2, "%lld");
        }
        else if(strncmp(m_tableInfo.struct_info[i].s_type, "TEXT", 4) == 0)
        {
            strcat(content2, "\"%s\"");
        }
    }
    sprintf(m_replaceRecordCommand, g_replaceRecordTemplate, m_dbTableName, content1, content2);

    memset(content1, 0x00, sizeof(content1));
    memset(content2, 0x00, sizeof(content2));
    for(int i = 0; i < m_tableInfo.struct_number; i++)
    {
        if(i != 0)
        {
            strcat(content1, ",");
        }
        strcat(content1, "?");
    }
    sprintf(m_insertRecordCommand, g_insertRecordTemplate, m_dbTableName, content1);
   
    memset(content1, 0x00, sizeof(content1));
    memset(content2, 0x00, sizeof(content2));
    for(int i = 0; i < m_tableInfo.struct_number; i++)
    {
        if(i == 0)
        {
            strcat(content2, m_tableInfo.struct_info[i].s_name);
            strcat(content2, "=");
            if(strcmp(m_tableInfo.struct_info[i].s_type, "INT") == 0 || strcmp(m_tableInfo.struct_info[i].s_type, "TINYINT") == 0
                    || strcmp(m_tableInfo.struct_info[i].s_type, "BIT") == 0)
            {
                strcat(content2, "%d");
            }
            else if(strcmp(m_tableInfo.struct_info[i].s_type, "BIGINT") == 0)
            {
                strcat(content2, "%lld");
            }
            else if(strncmp(m_tableInfo.struct_info[i].s_type, "TEXT", 4) == 0)
            {
                strcat(content2, "\"%s\"");
            }

            if(m_tableInfo.struct_number == 1)
            {
                strcat(content1, m_tableInfo.struct_info[i].s_name);
                strcat(content1, "=");
                if(strcmp(m_tableInfo.struct_info[i].s_type, "INT") == 0 || strcmp(m_tableInfo.struct_info[i].s_type, "TINYINT") == 0
                        || strcmp(m_tableInfo.struct_info[i].s_type, "BIT") == 0)
                {
                    strcat(content1, "%d");
                }
                else if(strcmp(m_tableInfo.struct_info[i].s_type, "BIGINT") == 0)
                {
                    strcat(content1, "%lld");
                }
                else if(strncmp(m_tableInfo.struct_info[i].s_type, "TEXT", 4) == 0)
                {
                    strcat(content1, "\"%s\"");
                }
            }
        }
        else
        {
            if(i != 1)
            {
                strcat(content1, ",");
            }
            strcat(content1, m_tableInfo.struct_info[i].s_name);
            strcat(content1, "=");
            if(strcmp(m_tableInfo.struct_info[i].s_type, "INT") == 0 || strcmp(m_tableInfo.struct_info[i].s_type, "TINYINT") == 0
                    || strcmp(m_tableInfo.struct_info[i].s_type, "BIT") == 0)
            {
                strcat(content1, "%d");
            }
            else if(strcmp(m_tableInfo.struct_info[i].s_type, "BIGINT") == 0)
            {
                strcat(content1, "%lld");
            }
            else if(strncmp(m_tableInfo.struct_info[i].s_type, "TEXT", 4) == 0)
            {
                strcat(content1, "\"%s\"");
            }
        }
    }
    sprintf(m_updateRecordCommand, g_updateRecordTemplate, m_dbTableName, content1, content2);

    memset(content1, 0x00, sizeof(content1));
    strcat(content1, m_tableInfo.struct_info[0].s_name);
    strcat(content1, "=");
    if(strcmp(m_tableInfo.struct_info[0].s_type, "INT") == 0 || strcmp(m_tableInfo.struct_info[0].s_type, "TINYINT") == 0
            || strcmp(m_tableInfo.struct_info[0].s_type, "BIT") == 0)
    {
        strcat(content1, "%d");
    }
    else if(strcmp(m_tableInfo.struct_info[0].s_type, "BIGINT") == 0)
    {
        strcat(content1, "%lld");
    }
    else if(strncmp(m_tableInfo.struct_info[0].s_type, "TEXT", 4) == 0)
    {
        strcat(content1, "\"%s\"");
    }
    sprintf(m_deleteRecordCommand, g_deleteRecordTemplate, m_dbTableName, content1);

    return 0;
}
