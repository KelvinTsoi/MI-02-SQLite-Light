/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DbDefine.h
 * Author: CAI
 *
 * Created on 2016年5月18日, 下午1:58
 */

#ifndef DBDEFINE_H
#define DBDEFINE_H

// 函数错误编码；
#define DATABASE_OK                   0     /* SQLite OK */
#define DATABASE_NOT_INIT            99     /* 未初始化 */
#define DATABASE_FILE_PATH_ILLEGAL  100     /* 文件路径非法 */
#define DATABASE_BLOCK              101     /* 无法打开数据库文件 */
#define DATABASE_TABLE_NOT_EXIST    102     /* 数据表不存在，不符合执行要求 */
#define DATABASE_TABLE_EXIST        103     /* 数据表已存在，无法新建 */
#define DATABASE_EXECUTION_FAILED   104     /* SQL执行语句执行失败 */
#define DATABASE_BUSY               105     /* 数据库正在被占用 */
#define DATABASE_ERROR              106     /* SQLite ERROR */
#define DATABASE_OPEN               107     /* 读数据库文件错误 */
#define DATABASE_PTHREAD            108     /* 线程创建错误 */
#define DATABASE_NOT_OPEN           109     /* 数据库没打开 */
#define DATABASE_KEY_NOT_HERE       110     /* 查找的主键不存在 */
#define DATABASE_ILLIGAL_DATA       111     /* 非法卡数据（卡号或序列号为零） */
#define DATABASE_ILLEGAL_DB_FILE    201     /* db文件不是数据库文件 */
#define NUMBER_OVERFLOW             300     /* 数据长度溢出 */


// 数据在查询队列中的保存位置；
#define POSITION_SERIALNUMBER       4      /* 卡序列号 */
#define POSITION_CARDTYPE           5      /* 卡类型 */
#define POSITION_EXPIRYDATE         6      /* 有效截止日期 */
#define POSITION_CARDID             7      /* 卡号 */

// 字符串转换基数；
#define DECIMAL_BASE                48      

// 数据库文件读取缓冲区大小；
#define BUFFER_SIZE                 4096

//卡信息基础数据结构；
typedef struct
{
    /**
     * 从平台下载卡信息的dat文件类型。
     * 0x01表示4字节序列号+8字节ID的卡类型，0x02表示4字节序列号的卡类型，0x03表示8字节序列号的卡类型。
     */
    unsigned char DatFileType; 
    
    /**
     * 卡类型。0x01表示白名单，0x02表示黑名单。
     */
    unsigned char CardType; 
    
    /**
     * IC卡序列号。
     */
    unsigned int SerialNumber; 
    
    /**
     * 有限截止时间。自1970-01-01 00：00：00经过的分钟数
     */
    unsigned int ExpiryTime; 
    
    /**
     * IC卡扇区里的ID。
     */
    unsigned long long CardID; 
    
} CARD_S;


#endif /* DBDEFINE_H */

