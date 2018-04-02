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
 * File:   DbFundation.h
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#include "DbFundation.h"

DbFundation::DbFundation()
{
    bzero(m_dbTableName, sizeof (m_dbTableName));
    sprintf(m_dbTableName, "happy_table");
}

DbFundation::~DbFundation()
{
}

int DbFundation::CreateTable()
{
    int ret = 0;

    char strCmd[256] = {0};

	sprintf(strCmd, "CREATE TABLE \"%s\" (\"seqence_number\" INT NOT NULL,\"data_type\" TINYINT,\"expire_time\" INT,\"data\" BIGINT,\"content\" TEXT(128), PRIMARY KEY ( \"seqence_number\" ));", m_dbTableName);

    ret = ExecSql(strCmd);
    if (ret != DATABASE_OK)
    {
    	printf("Table create error.\n");
        return DATABASE_TABLE_EXIST;
    }

    return DATABASE_OK;
}

int DbFundation::Add(DATA_S DataInfo)
{
    if( DataInfo.Sequence == 0 || DataInfo.Data == 0 )
    {
    	printf("Sequence Number or Data in this Info is illegal!\r\n");
        return DATABASE_ILLIGAL_DATA;
    }

    char strCmd[256] = {0};

    switch(m_tableInfo.struct_number)
    {
        case DATABASE_STRUCT_MEMBER:
            sprintf(strCmd, m_replaceRecordCommand,
                overflow4Bytes(DataInfo.Sequence),
                overflow1Bytes(DataInfo.Type),
                overflow4Bytes(DataInfo.ExpiryTime),
                overflow8Bytes(DataInfo.Data),
                DataInfo.Content);
            break;
        default:
            return DATABASE_TABLE_COMP_ERROR;
    }
       
    int ret = ExecSql(strCmd);

    if (ret != DATABASE_OK)
        return ret;

    return DATABASE_OK;
}

int DbFundation::Add(DATA_S DataInfo[], const int insertDataSetSize)
{
    int ret;

    ret = sqlite3_open(m_dbFile, &db);

    if (ret)
    {
    	printf( "Can't open database: %s\n", sqlite3_errmsg(db));
        return DATABASE_BLOCK;
    }

    printf("Large data insertion process...\n");

    sqlite3_exec(db, "PRAGMA synchronous = 0;", NULL, NULL, NULL);

    sqlite3_stmt *stmt1 = NULL;
    const char* beginSQL = "BEGIN TRANSACTION";
    if ((ret = sqlite3_prepare_v2(db, beginSQL, strlen(beginSQL), &stmt1, NULL)) != SQLITE_OK)
    {
        if (stmt1)
            sqlite3_finalize(stmt1);
        return ret;
    }
    if ((ret = sqlite3_step(stmt1)) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt1);
        return ret;
    }
    sqlite3_finalize(stmt1);

    sqlite3_stmt* stmt2 = NULL;

    if ((ret = sqlite3_prepare_v2(db, m_insertRecordCommand, strlen(m_insertRecordCommand), &stmt2, NULL)) != SQLITE_OK)
    {
        if (stmt2)
            sqlite3_finalize(stmt2);
        return ret;
    }

    for (int i = 0; i < insertDataSetSize; i++)
    {
        if( DataInfo[i].Sequence == 0 || DataInfo[i].Data == 0 )
        {
			printf("Sequence Number or Data in this Info is illegal!\r\n");
            continue;
        }
        
        switch(m_tableInfo.struct_number)
        {
            case DATABASE_STRUCT_MEMBER:
                sqlite3_bind_int(stmt2, 1, DataInfo[i].Sequence);
                sqlite3_bind_int(stmt2, 2, DataInfo[i].Type);
                sqlite3_bind_int(stmt2, 3, DataInfo[i].ExpiryTime);
                sqlite3_bind_int64(stmt2, 4, DataInfo[i].Data);
                sqlite3_bind_text(stmt2, 5, DataInfo[i].Content, -1, NULL);
                break;
            default:
                return DATABASE_TABLE_COMP_ERROR;
        }
        
        ret = sqlite3_step(stmt2);

        if (ret == SQLITE_CONSTRAINT)
        	;//LOG_E("KEY has already been here!\n");  
        else if (ret != SQLITE_DONE && ret != SQLITE_CONSTRAINT)
        {
            sqlite3_finalize(stmt2);
            return ret;
        }

        sqlite3_reset(stmt2);
    }

    sqlite3_finalize(stmt2);

    const char* commitSQL = "COMMIT";
    sqlite3_stmt* stmt3 = NULL;
    printf("Committing...\n");
    if ((ret = sqlite3_prepare_v2(db, commitSQL, strlen(commitSQL), &stmt3, NULL)) != SQLITE_OK)
    {
        if (stmt3)
            sqlite3_finalize(stmt3);
        return ret;
    }

    if ((ret = sqlite3_step(stmt3)) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt3);
        return ret;
    }

    sqlite3_finalize(stmt3);

    return ret;
}

int DbFundation::Delete(unsigned int SequenceNumber)
{
    int ret;

    char strCmd[256] = {0};
    
    sprintf(strCmd, m_deleteRecordCommand, overflow4Bytes(SequenceNumber));
    ret = ExecSql(strCmd);
    
    if(ret != DATABASE_OK)
        return ret;

    /*
    char sqlCmdClear[20] = {0};
    sprintf(sqlCmdClear, "VACUUM;");
    ret = ExecSql(sqlCmdClear);
    */ 

    return ret;
}

int DbFundation::Update(DATA_S DataInfo)
{
    char strCmd[256] = {0};
    
    switch(m_tableInfo.struct_number)
    {
        case DATABASE_STRUCT_MEMBER:
            sprintf(strCmd, m_updateRecordCommand,
                    overflow1Bytes(DataInfo.Type), 
                    overflow4Bytes(DataInfo.ExpiryTime), 
                    overflow8Bytes(DataInfo.Data), 
                    DataInfo.Content,
                    overflow4Bytes(DataInfo.Sequence));
            break;
        default:
            return DATABASE_TABLE_COMP_ERROR;
    }

    return ExecSql(strCmd);
}

int DbFundation::FindBySeqenceNumber(unsigned int SequenceNumber, DATA_S *DataInfo)
{
    memset(DataInfo->Content, 0x00, sizeof(DataInfo->Content));
    char strCmd[256] = {0};
    sprintf(strCmd, "SELECT * FROM \"%s\" WHERE seqence_number =%d;", m_dbTableName, overflow4Bytes(SequenceNumber));
    char *zErrMsg = 0;
    int row = 0;
    int column = 0;
    char **dbResult;
    sqlite3_get_table(db, strCmd, &dbResult, &row, &column, &zErrMsg);
    if (row == 0 && column == 0)
    {
    	printf("This Sequence Number does not exist in %s!\n", strCmd);
        return DATABASE_KEY_NOT_HERE;
    }
    else
    {
        for(int i = 0; i < m_tableInfo.struct_number; i++)
        {
            if(strcmp(dbResult[i], m_tableInfo.struct_info[i].s_name)==0)
            {
                switch(i)
                {
                    case 0:
                        uintReadFromStr(dbResult[i+m_tableInfo.struct_number], &DataInfo->Sequence);
                        break;
                    case 1:
                        ucharReadFromStr(dbResult[i+m_tableInfo.struct_number], &DataInfo->Type);
                        break;
                    case 2:
                        uintReadFromStr(dbResult[i+m_tableInfo.struct_number], &DataInfo->ExpiryTime);
                        break;
                    case 3:
                        ullintReadFromStr(dbResult[i+m_tableInfo.struct_number], &DataInfo->Data);
                        break;
                    case 4:
                        sprintf(DataInfo->Content, dbResult[i+m_tableInfo.struct_number]);
                        break;
                    default:
                        continue;
                }
            }
            else
            {
                break;
            }
        }
    }
    sqlite3_free_table(dbResult);
    return DATABASE_OK;
}

int DbFundation::CountBySeqenceNumber(unsigned int &amount)
{
    return Count(amount);
}
