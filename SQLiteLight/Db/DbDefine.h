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
#define DATABASE_ILLIGAL_DATA       111     /* Illegal data */
#define DATABASE_TABLE_COMP_ERROR   112     /* Data Base construction matching error */
#define DATABASE_ILLEGAL_DB_FILE    201     /* Object file is not data base */
#define NUMBER_OVERFLOW             300     /* Data length overflow */

// String conversion cardinal number;
#define DECIMAL_BASE                48

// Date base file reading buffer size;
#define BUFFER_SIZE                 4096

// Date struct members
#define DATABASE_STRUCT_MEMBER		5 


/**
 * Data info structure:
 * 4 Bytes Sequence Number + 1 Bytes Data Type + 4 Bytes Data's Period of Validity +
 * 8 Bytes Identification Number + 128 Bytes Content
 */
typedef struct
{
    /**
     * 4 Bytes sequence number
     */
    unsigned int Sequence;

    /** 
     * 1 Bytes data type
     */
    unsigned char Type;

    /**
     * 4 Bytes data's period of validity
     */
    unsigned int ExpiryTime;

    /**
     * 8 Bytes data
     */
    unsigned long long Data;

    /**
     * 128 Bytes Content
     */
    char Content[128];

} DATA_S;


#endif /* DBDEFINE_H */

