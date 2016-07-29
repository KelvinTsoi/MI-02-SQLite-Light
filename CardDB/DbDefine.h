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
 * Created on 2016/7/29, 9:58pm
 */

#ifndef DBDEFINE_H
#define DBDEFINE_H

// Function Error Code；
#define DATABASE_OK                   0     /* SQLite Function Correct */
#define DATABASE_NOT_INIT            99     /* Uninitialized */
#define DATABASE_FILE_PATH_ILLEGAL  100     /* Illegal file path */
#define DATABASE_BLOCK              101     /* Cannot open database */
#define DATABASE_TABLE_NOT_EXIST    102     /* Table inexistence, doesn't conform to requirements of execution*/
#define DATABASE_TABLE_EXIST        103     /* Table existence, cannot create*/
#define DATABASE_EXECUTION_FAILED   104     /* SQL command execution failed */
#define DATABASE_BUSY               105     /* Date base be in using */
#define DATABASE_ERROR              106     /* SQLite Function ERROR */
#define DATABASE_OPEN               107     /* Read database file error */
#define DATABASE_PTHREAD            108     /* Thread creation failed */
#define DATABASE_NOT_OPEN           109     /* Date base hasn't been open */
#define DATABASE_KEY_NOT_HERE       110     /* Primary key doesn't exist */
#define DATABASE_ILLIGAL_DATA       111     /* Illegal card date */
#define DATABASE_ILLEGAL_DB_FILE    201     /* Object file is not data base */
#define NUMBER_OVERFLOW             300     /* Date length overflow */


// 数据在查询队列中的保存位置；
#define POSITION_SERIALNUMBER       4      /* Card serial number */
#define POSITION_CARDTYPE           5      /* Card type */
#define POSITION_EXPIRYDATE         6      /* Card expiration date */
#define POSITION_CARDID             7      /* Card number */

// String conversion cardinal number;
#define DECIMAL_BASE                48      

// Date base file reading buffer size;
#define BUFFER_SIZE                 4096

// Card info structure；
typedef struct
{
    /**
     * Type of dat file download from cloud;
     * 0x01 signify 4 Bytes serial number + 8 Bytes number;
     */
    unsigned char DatFileType; 
    
    /**
     * Card type, 0x01 signify white list，0x02 signify black list;
     */
    unsigned char CardType; 
    
    /**
     * Card serial number。
     */
    unsigned int SerialNumber; 
    
    /**
     * Card expiration date, the minutes passing from 1970-01-01 00：00：00 tile now;
     */
    unsigned int ExpiryTime; 
    
    /**
     * Card number。
     */
    unsigned long long CardID; 
    
} CARD_S;


#endif /* DBDEFINE_H */

