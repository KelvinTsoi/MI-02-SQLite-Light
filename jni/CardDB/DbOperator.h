/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DbOperator.h
 * Author: CAI
 *
 * Created on 2016年5月22日, 上午11:40
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

#include "sqlite3/sqlite3.h"


#include "CAutoLock.h"
#include "ThreadMutex.h"

#include "LOG_D.h"

class DbOperator
{
public:
    /**
     * 构造函数
     */
    DbOperator();

    /**
     * 析构函数
     */
    virtual ~DbOperator();

    /**
     * @函数功能：初始化
     * @参数 dbFile 数据库的保存路径
     * @返回值： 成功返回0，大于0表示错误代码
     */
    virtual int Init(char* dbFile);

    /**
     * @函数功能：打开数据库
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    virtual int CreateTable() = 0;

    /**
     * @函数功能：打开数据库
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    virtual int Open();

    /**
     * @函数功能：关闭数据库
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    virtual int Close();

    /**
     * @函数功能：数据表中异步删除所有记录;
     * @返回值 函数错误编码；
     */
    virtual int CleanAll();

    /**
     * @函数功能：检查数据里的表结构是否符合本程序要求
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    virtual int CheckTable();

    /**
     * @函数功能：清空数据库空闲列表；
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    int CleanUpQueue();

    /**
     * @函数功能：获取数据表里的记录数
     * @返回值： 获取数据表里的记录数
     */
    virtual int Count();


    /**
     * 检查输入的文件是否是一个合法的数据库db文件。
     * @param dbFilePath 数据库db文件的路径
     * @return 返回0表示是合法数据库db文件，返回大于0的数表示错误代码
     */
    int IsDbFile(char *dbFilePath);


protected:

    /**
     * @函数功能：执行指定的SQL语句并返回相应结果
     * @参数 arg：给予执行函数的参数地址;
     * @返回值 函数错误编码；
     */
    int ExecSql(char *sqlCmd);

    /**
     * 函数功能：char*类型转换成unsigned int类型
     * @参数 arg：给予执行函数的参数地址;
     * @返回值 函数错误编码；
     */
    void uintReadFromStr(const char *src, unsigned int *dst);

    /**
     * 函数功能：char*类型转换成unsigned long long类型
     * @参数 arg：给予执行函数的参数地址;
     * @返回值 函数错误编码；
     */
    void ullintReadFromStr(const char *src, unsigned long long int *dst);

    /**
     * @函数功能：char*类型转换成unsigned char类型
     * @参数 arg：给予执行函数的参数地址;
     * @返回值 函数错误编码；
     */
    void ucharReadFromStr(const char *src, unsigned char * dst);

    /**
     * 函数功能：1字节有符号整形数据溢出检测；
     * @参数 src：1字节无符号整形数据；
     * @返回值 函数错误编码； 
     */
    char overflow1Bytes(unsigned char src);


    /**
     * 函数功能：4字节有符号整形数据溢出检测；
     * @参数 src：4字节无符号整形数据；
     * @返回值 函数错误编码； 
     */
    int overflow4Bytes(unsigned int src);

    /**
     * 函数功能：8字节有符号整形数据溢出检测；
     * @参数 src：8字节无符号整形数据；
     * @返回值 函数错误编码； 
     */
    long long int overflow8Bytes(unsigned long long int src);

protected:

    /**
     * 数据库句柄
     */
    sqlite3 *db;

    /**
     * 数据库文件全路径
     */
    char m_dbFile[100];

    /**
     * 数据库表
     */
    char m_dbTableName[20];
};

#endif /* DBOPERATOR_H */

