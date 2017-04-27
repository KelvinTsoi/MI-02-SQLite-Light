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
    sprintf(m_dbTableName, "card_table");
}

DbFundation::~DbFundation()
{
}

int DbFundation::CreateTable()
{
    int ret = 0;

    char strCmd[200] = {0};

    sprintf(strCmd, "CREATE TABLE \"%s\" (\"serial_num\" INT NOT NULL,\"card_id\" BIGINT,\"card_type\" TINYINT, \"expiry_date\" INT,PRIMARY KEY ( \"serial_num\" ));", m_dbTableName);

    ret = ExecSql(strCmd);
    if (ret != DATABASE_OK)
    {
    	printf("Table create error.\n");
        return DATABASE_TABLE_EXIST;
    }

    return DATABASE_OK;
}

int DbFundation::Add(CARD_S CardInfo)
{
    if( CardInfo.SerialNumber == 0 || CardInfo.CardID == 0 )
    {
    	printf("SerialNumber or CardID in this card is illegal!(SerialNumber == 0x00 || CardID == 0x00)\n");
        return DATABASE_ILLIGAL_DATA;
    }

    char strCmd[200] = {0};

    sprintf(strCmd, "REPLACE INTO %s ( serial_num,card_id,card_type,expiry_date ) VALUES ( %d,%lld,%d,%d );",
            m_dbTableName,
            overflow4Bytes(CardInfo.SerialNumber),
            overflow8Bytes(CardInfo.CardID),
            overflow1Bytes(CardInfo.CardType),
            overflow4Bytes(CardInfo.ExpiryTime)
    );

    int ret = ExecSql(strCmd);

    if (ret != DATABASE_OK)
        return ret;

    return DATABASE_OK;
}

int DbFundation::Add(CARD_S CardInfo[], const int insertDataSetSize)
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

    char insertSQL[128] = {0x00};
    sprintf(insertSQL, "INSERT INTO %s VALUES(?,?,?,?)", m_dbTableName);
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
        	printf("SerialNumber or CardID in this card is illegal!(SerialNumber == 0x00 || CardID == 0x00)\n");
            continue;
        }

        sqlite3_bind_int(stmt2, 1, CardInfo[i].SerialNumber);
        sqlite3_bind_int64(stmt2, 2, CardInfo[i].CardID);
        sqlite3_bind_int(stmt2, 3, CardInfo[i].CardType);
        sqlite3_bind_int(stmt2, 4, CardInfo[i].ExpiryTime);


        ret = sqlite3_step(stmt2);

        if (ret == SQLITE_CONSTRAINT)
        {
            //if there is same data in list...
        }
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

int DbFundation::Delete(unsigned int serialNumber)
{
    int ret;
    char strCmd[200] = {0};
    sprintf(strCmd, "DELETE FROM %s WHERE serial_num =%d;", m_dbTableName, overflow4Bytes(serialNumber));
    ret = ExecSql(strCmd);
    return ret;
}

int DbFundation::Update(CARD_S CardInfo)
{
    char strCmd[200] = {0};
    sprintf(strCmd, "UPDATE %s SET card_id =%lld, card_type =%d, expiry_data =%d WHERE serial_num =%d;"
            , m_dbTableName, overflow8Bytes(CardInfo.CardID), overflow1Bytes(CardInfo.CardType), overflow4Bytes(CardInfo.ExpiryTime), overflow4Bytes(CardInfo.SerialNumber));
    return ExecSql(strCmd);
}

int DbFundation::FindBySerialNumber(unsigned int serialNumber, CARD_S *CardInfo)
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
    	printf("This Serial Number does not exist in %s!\n", strCmd);
        return DATABASE_KEY_NOT_HERE;
    }
    else
    {
        uintReadFromStr(dbResult[POSITION_SERIALNUMBER], &CardInfo->SerialNumber);
        ullintReadFromStr(dbResult[POSITION_CARDID], &CardInfo->CardID);
        ucharReadFromStr(dbResult[POSITION_CARDTYPE], &CardInfo->CardType);
        uintReadFromStr(dbResult[POSITION_EXPIRYDATE], &CardInfo->ExpiryTime);
    }
    sqlite3_free_table(dbResult);
    return DATABASE_OK;
}

int DbFundation::CountBySerialNumber(unsigned int &amount)
{
    return Count(amount);
}
