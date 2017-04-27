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
 * File:   DbDefine.h
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#ifndef DBDEFINE_H
#define DBDEFINE_H

// Function Error Code:
#define DATABASE_OK                   0     /* SQLite Function Correct */
#define DATABASE_NOT_INIT            99     /* Uninitialized */
#define DATABASE_FILE_PATH_ILLEGAL  100     /* Illegal file path */
#define DATABASE_BLOCK              101     /* Cannot open database */
#define DATABASE_TABLE_NOT_EXIST    102     /* Table inexistent, does n't conform to requirements of execution*/
#define DATABASE_TABLE_EXIST        103     /* Table existence, cannot create*/
#define DATABASE_EXECUTION_FAILED   104     /* SQL command execution failed */
#define DATABASE_BUSY               105     /* Date base be in using */
#define DATABASE_ERROR              106     /* SQLite Function ERROR */
#define DATABASE_OPEN               107     /* Read database file error */
#define DATABASE_PTHREAD            108     /* Thread creation failed */
#define DATABASE_NOT_OPEN           109     /* Date base has not been open */
#define DATABASE_KEY_NOT_HERE       110     /* Primary key does not exist */
#define DATABASE_ILLIGAL_DATA       111     /* Illegal card date */
#define DATABASE_ILLEGAL_DB_FILE    201     /* Object file is not data base */
#define NUMBER_OVERFLOW             300     /* Date length overflow */

// Position of data in query results queue;
#define POSITION_SERIALNUMBER       4      /* Card Serial Number */
#define POSITION_CARDID             5      /* Card Identification Number */
#define POSITION_CARDTYPE           6      /* Card Type */
#define POSITION_EXPIRYDATE         7      /* Card Expiration Date */

// String conversion cardinal number;
#define DECIMAL_BASE                48

// Date base file reading buffer size;
#define BUFFER_SIZE                 4096

/**
 * Card info structure:
 * 4 Bytes Serial Number + 8 Bytes Identification Number
 */
typedef struct
{
    /**
     * 4 Bytes Serial Number in Card
     */
    unsigned int SerialNumber;

    /**
     * 8 Bytes Identification Number in Card
     */
    unsigned long long CardID;

    /**
     * Card type, 0x01 signify as white list card, 0x02 signify as black list card;
     */
    unsigned char CardType;

    /**
     * Card expiration date, the minutes passing from 1970-01-01 00：00：00 tile now;
     */
    unsigned int ExpiryTime;

} CARD_S;


#endif /* DBDEFINE_H */

