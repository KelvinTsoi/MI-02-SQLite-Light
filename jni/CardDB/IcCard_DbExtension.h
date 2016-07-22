/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IcCard_DbExtension.h
 * Author: CAI
 *
 * Created on 2016年6月26日, 下午10:00
 */

#ifndef ICCARD_DBEXTENSION_H
#define ICCARD_DBEXTENSION_H
#include "DbOperator.h"

class IcCard_DbExtension
{
public:
    
    IcCard_DbExtension();
    
    virtual ~IcCard_DbExtension();

    /**
     * @函数功能：初始化
     * @参数 dbFile 当前使用的数据库全路径
     * @参数 dbFileBak 备份数据库全路径
     * @参数 interval 定时把内存数据同步到flash的间隔。单位秒。
     * @返回值 成功返回0
     */
    virtual int Init(char* dbFile, char* dbFileBak, unsigned int interval);

    /**
     * @函数功能:数据库文件逆备份,即把保存在flash里的db文件加载到内存。
     * @返回值: 成功返回0;
     */
    virtual int LoadLocalDB();

    /**
     * 函数功能：用从平台下载的db文件替换掉保存在flash里的db文件
     * @参数 datFile：从平台下载的保存了卡数据的db文件的全路径
     * @返回值 成功返回0，返回大于0的数表示错误代码
     */
    int ReplaceDbFile(char* datFile);

 
    /**
     * @函数功能:手动同步一次,阻塞方式;
     * @返回值：成功返回0； 
     */
    virtual int ManulBak();


    /**
     * @函数功能:停止自动同步
     * @返回值: 成功返回0
     */
    int StopAutoBak();


    /**
     * 开始自动同步,每隔interval秒同步一次。interval在init函数里设定。
     * @return 成功返回0
     */
    int StartAutoBak();

    /**
     * @函数功能:回收资源;
     * @返回值: 成功返回0;
     */
    virtual int UnInit();


    /**
     * 拷贝文件
     * @param srcFilePath 拷贝源文件名的全路径
     * @param dstFilePath 拷贝目的文件名全路径
     * @return 成功返回0，返回大于0的数表示错误代码
     */
    int CopyFile(char *srcFilePath, char *dstFilePath);

    /**
     * 函数功能：数据库清空函数；
     * @返回值 成功返回0，返回大于0的数表示错误代码
     */
    int CleanAll();


    /**
     * 根据db文件大小和MD5值，判断是否与flash里的备份数据库一致
     * @param dbFileSize 需要比较的的数据库db文件大小
     * @param md5Value 需要比较的的数据库db文件MD5值
     * @return 0表示两个数据库一致。函数错误编码，查阅DbDefine.h文件；
     */
    int CompareToBakDb(long long int dbFileSize, unsigned char md5Value[]);


    /**
     * @函数功能：获取数据库文件的文件大小的字节数
     * @返回值： 数据库文件的文件大小的字节数
     */
    static long long int GetDbFileSize(char *dbFilePath);


    /**
     * 计算数据文件的MD5值
     * @param dbFilePath 数据库db文件的路径
     * @param md5Value 16字节数组，保存了计算出来的数据文件的MD5值。调用前需要申请内存。
     * @return 成功返回0，返回大于0的数表示错误代码
     */
    static int GetDbFileMD5(char *dbFilePath, unsigned char md5Value[]);


protected:
    /**
     * 放在flash里给增删改查用的数据库
     */
    DbOperator *m_pDbBase;

    /**
     * 备份的数据库
     */
    DbOperator *m_pDbBakBase;
    
    /**
     * dat文件路径
     */
    char m_datFile[100];

    /**
     * 放在flash里给增删改查用的数据库文件路径
     */
    char m_dbFile[100];

    /**
     * 备份的数据库文件路径
     */
    char m_dbFileBak[100];

    /**
     * 开停自动备份条件变量.true为定时备份，false为停止定时备份
     */
    bool bAutoBakSwitch;

    /**
     *  互斥锁类初始化
     */
    ThreadMutex mtx;


    /**
     * flash里的备份数据库db文件的MD5的值。16字节的16进制数。
     */
    unsigned char m_bakDbFileMD5Value[16];

    /**
     * flash里的备份数据库db文件的大小。
     */
    long long int m_bakDbFileSize;


    /**
     * 自动把内存数据库备份到flash的间隔。单位是秒。
     */
    unsigned int m_bakupInterval;
private:

    /**
     * 函数功能：线程执行函数，启动数据库文件自动备份功能；
     * @参数 arg：传递给线程执行函数的参数；
     * @返回值：8字节整形数据
     */
    static void *AutoBakThreadCallback(void *arg);

    /**
     * 自动备份数据库的线程的实际执行函数
     * @return 
     */
    void* AutoBakThreadFunc();

    /**
     * 模块是否已经初始化。0表示没初始化，1表示已经初始化。
     */
    bool m_bInit;

    /**
     * 定时备份线程的ID
     */
    pthread_t m_bakupThreadId;
};

#endif /* ICCARD_DBEXTENSION_H */

