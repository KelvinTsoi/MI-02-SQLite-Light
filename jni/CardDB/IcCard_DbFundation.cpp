/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IcCard_Db.cpp
 * Author: CAI
 * 
 * Created on 2016年5月23日, 下午7:32
 */

#include "IcCard_DbFundation.h"

IcCard_DbFundation::IcCard_DbFundation()
{
    bzero(m_dbTableName, sizeof (m_dbTableName));
    sprintf(m_dbTableName, "card_table");
}

IcCard_DbFundation::~IcCard_DbFundation()
{
}

/**
 * @函数功能：创建数据表
 * @返回值：成功返回0，返回大于0的数表示错误代码
 */
int IcCard_DbFundation::CreateTable()
{
    int ret = 0;

    char strCmd[200] = {0};

    sprintf(strCmd, "CREATE TABLE \"%s\" (\"serial_num\" INT NOT NULL,\"card_type\" TINYINT,\"expiry_date\" INT, \"card_id\" BIGINT,PRIMARY KEY ( \"serial_num\" ));", m_dbTableName);

    ret = ExecSql(strCmd);
    if (ret != DATABASE_OK)
    {
        LOG_E("Table create error.\n");
        return DATABASE_TABLE_EXIST;
    }

    return DATABASE_OK;
}

/**
 * 函数功能：数据库记录插入函数；
 * @参数 CardInfo：卡数据集结构体；
 * @返回值 函数错误编码；
 */
int IcCard_DbFundation::Add(CARD_S CardInfo)
{
    if( CardInfo.SerialNumber == 0 || CardInfo.CardID == 0 )
    {
        LOG_E("SerialNumber or CardID in this card is illigal!(SerialNumber == 0x00 || CardID == 0x00)\n");
        return DATABASE_ILLIGAL_DATA;
    }
    
    char strCmd[200] = {0};
 
    sprintf(strCmd, "REPLACE INTO %s ( serial_num,card_type,expiry_date,card_id ) VALUES ( %d,%d,%d,%lld );",
            m_dbTableName, overflow4Bytes(CardInfo.SerialNumber),overflow1Bytes(CardInfo.CardType), 
            overflow4Bytes(CardInfo.ExpiryTime),overflow8Bytes(CardInfo.CardID));

    int ret = ExecSql(strCmd);
    
    if (ret != DATABASE_OK)
        return ret;

    return DATABASE_OK;
}

/**
 * 函数功能：ic_card_table数据表异步批量记录插入函数；
 * @参数 CardInfo：卡数据集结构体数组，size：卡数据集结构体数组大小；
 * @返回值 函数错误编码；
 */
int IcCard_DbFundation::Add(CARD_S CardInfo[], const int insertDataSetSize)
{
    int ret;

    ret = sqlite3_open(m_dbFile, &db);

    if (ret)
    {
        LOG_E( "Can't open database: %s\n", sqlite3_errmsg(db));
        return DATABASE_BLOCK;
    }

    LOG_E("Large data insertation process...\n");

    sqlite3_exec(db, "PRAGMA synchronous = 0;", NULL, NULL, NULL);

    //显示开启事物；
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

    //构建基于绑定变量的插入数据；
    const char* insertSQL = "INSERT INTO card_table VALUES(?,?,?,?)";
    sqlite3_stmt* stmt2 = NULL;

    if ((ret = sqlite3_prepare_v2(db, insertSQL, strlen(insertSQL), &stmt2, NULL)) != SQLITE_OK)
    {
        if (stmt2)
            sqlite3_finalize(stmt2);
        return ret;
    }

    for (int i = 0; i < insertDataSetSize; i++)
    {
        if( CardInfo[i].SerialNumber == 0 || CardInfo[i].CardID == 0 )
        {
            LOG_E("SerialNumber or CardID in this card is illigal!(SerialNumber == 0x00 || CardID == 0x00)\n");
            continue;
        }
        
        sqlite3_bind_int(stmt2, 1, CardInfo[i].SerialNumber);
        sqlite3_bind_int(stmt2, 2, CardInfo[i].CardType);
        sqlite3_bind_int(stmt2, 3, CardInfo[i].ExpiryTime);
        sqlite3_bind_int64(stmt2, 4, CardInfo[i].CardID);

        ret = sqlite3_step(stmt2);

        if (ret == SQLITE_CONSTRAINT)
            //LOG_E("KEY has already been here!\n");
            ;
        else if (ret != SQLITE_DONE && ret != SQLITE_CONSTRAINT)
        {
            sqlite3_finalize(stmt2);
            return ret;
        }

        sqlite3_reset(stmt2);
    }

    sqlite3_finalize(stmt2);

    //提交事物；
    const char* commitSQL = "COMMIT";
    sqlite3_stmt* stmt3 = NULL;
    LOG_E("Commiting...\n");
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

/**
 * 函数功能：数据库记录删除函数；
 * @参数 CardInfo：卡数据集结构体；
 * @返回值 函数错误编码；
 */
int IcCard_DbFundation::Delete(unsigned int serialNumber)
{
    int ret;

    // 删除过程；
    char strCmd[200] = {0};
    sprintf(strCmd, "DELETE FROM %s WHERE serial_num =%d;", m_dbTableName, overflow4Bytes(serialNumber));
    ret = ExecSql(strCmd);
    
    if(ret != DATABASE_OK)
        return ret;

    // 清空数据库空闲列表；
    //char sqlCmdClear[20] = {0};
    //sprintf(sqlCmdClear, "VACUUM;");
    //ret = ExecSql(sqlCmdClear);

    return ret;
}

/**
 * 函数功能：数据库记录更新函数；
 * @参数 CardInfo：卡数据集结构体；
 * @返回值 函数错误编码；
 */
int IcCard_DbFundation::Update(CARD_S CardInfo)
{
    char strCmd[200] = {0};
    sprintf(strCmd, "UPDATE %s SET card_type =%d, expiry_data =%d, card_id =%lld WHERE serial_num =%d;"
            , m_dbTableName, overflow1Bytes(CardInfo.CardType), overflow4Bytes(CardInfo.ExpiryTime), overflow8Bytes(CardInfo.CardID), overflow4Bytes(CardInfo.SerialNumber));
    return ExecSql(strCmd);
}

/**
 * 函数功能：数据库记录查询函数；
 * @参数 serialNumber：需要查询记录的序列号索引，CardInfo：卡数据集结构体；
 * @返回值 函数错误编码；
 */
int IcCard_DbFundation::FindBySerialNumber(unsigned int serialNumber, CARD_S *CardInfo)
{
    char strCmd[200] = {0};

    sprintf(strCmd, "SELECT * FROM \"%s\" WHERE serial_num =%d;", m_dbTableName, overflow4Bytes(serialNumber));
    char *zErrMsg = 0;
    int row = 0;
    int column = 0;
    char **dbResult;
    sqlite3_get_table(db, strCmd, &dbResult, &row, &column, &zErrMsg);
    if (row == 0 && column == 0)
    {
        LOG_E("This Serial Number doesn't exist in %s!\n", strCmd);
        return DATABASE_KEY_NOT_HERE;
    }
    else
    {
        uintReadFromStr(dbResult[POSITION_SERIALNUMBER], &CardInfo->SerialNumber);
        ucharReadFromStr(dbResult[POSITION_CARDTYPE], &CardInfo->CardType);
        uintReadFromStr(dbResult[POSITION_EXPIRYDATE], &CardInfo->ExpiryTime);
        ullintReadFromStr(dbResult[POSITION_CARDID], &CardInfo->CardID);
    }
    sqlite3_free_table(dbResult);
    return DATABASE_OK;
}