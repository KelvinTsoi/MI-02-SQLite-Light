/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IcCard_DbExtension.cpp
 * Author: CAI
 * 
 * Created on 2016年6月26日, 下午10:00
 */

#include "IcCard_DbExtension.h"
extern "C"
{
#include "md5.h"
}

IcCard_DbExtension::IcCard_DbExtension()
{
    m_bakupThreadId = NULL;
    m_pDbBase = NULL;
    m_pDbBakBase = NULL;
    bzero(m_bakDbFileMD5Value, sizeof (m_bakDbFileMD5Value));
    bzero(m_dbFile, sizeof (m_dbFile));
    bzero(m_dbFileBak, sizeof (m_dbFileBak));
    m_bakDbFileSize = 0;
    bAutoBakSwitch = false;
    m_bInit = false;
}

IcCard_DbExtension::~IcCard_DbExtension()
{
}

int IcCard_DbExtension::Init(char* dbFile, char* dbFileBak, unsigned int interval)
{
    AUTO_LOCK(mtx);
    if (m_bInit)
        return DATABASE_OK;

    m_bInit = false;

    int ret;
    m_bakupInterval = interval;
    strcpy(m_dbFile, dbFile);
    strcpy(m_dbFileBak, dbFileBak);


    // 初始化flash里的备份数据库文件
    ret = m_pDbBakBase->Init(m_dbFileBak);
    if (ret != DATABASE_OK)
        return ret;

    ret = m_pDbBakBase->Open();
    if (ret != DATABASE_OK)
        return ret;

    ret = m_pDbBakBase->CheckTable();
    if (ret != DATABASE_TABLE_EXIST)
    {
        ret = m_pDbBakBase->CreateTable();
        if (ret != DATABASE_OK)
            return ret;
    }
    m_pDbBakBase->Close(); //仅做备份用，所以关闭

    //计算备份数据库db文件的MD5值
    ret = GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);
    if (ret != 0)
        return DATABASE_ERROR;
    //计算备份数据库db文件的大小
    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

    // 初始化内存的数据库文件
    ret = m_pDbBase->Init(m_dbFile);
    if (ret != DATABASE_OK)
        return ret;

    ret = m_pDbBase->Open();
    if (ret != DATABASE_OK)
        return ret;

    ret = m_pDbBase->CheckTable();
    if (ret != DATABASE_TABLE_EXIST)
    {
        ret = m_pDbBase->CreateTable();
        if (ret != DATABASE_OK)
            return ret;
    }

    //启动定时备份线程。这里只是启动线程，什么时候开始备份由变量bAutoBakSwitch决定
    bAutoBakSwitch = false;
    ret = pthread_create(&m_bakupThreadId, NULL, IcCard_DbExtension::AutoBakThreadCallback, this);
    if (ret != 0)
    {
        LOG_E("Thread creation failed.\n");
        return DATABASE_PTHREAD;
    }

    m_bInit = true;
    return DATABASE_OK;
}

int IcCard_DbExtension::LoadLocalDB()
{
    AUTO_LOCK(mtx);
    if (!m_bInit)
        return DATABASE_NOT_INIT;
    int ret;
    m_pDbBase->CleanUpQueue();
    m_pDbBase->Close();


    CopyFile(m_dbFileBak, m_dbFile); //把flash里备份的db文件拷贝到内存里

    //重新打开内存的数据db文件
    ret = m_pDbBase->Open();
    if (ret != DATABASE_OK)
        return ret;
    return DATABASE_OK;
}

int IcCard_DbExtension::ReplaceDbFile(char* dbFilePath)
{
    AUTO_LOCK(mtx);

    if (dbFilePath == NULL)
        return DATABASE_FILE_PATH_ILLEGAL;

    int ret;
    ret = m_pDbBase->IsDbFile(dbFilePath); //检查判断dbFilePath指定的db文件是否是真的db数据库文件。
    if (ret != 0)
        return DATABASE_ILLIGAL_DATA;

    //程序跑到这里，说明dbFilePath确实是一个合法的数据库db文件了。用它覆盖掉flash里的db文件。
    char cmd[200] = {0};
    sprintf(cmd, "mv %s %s", dbFilePath, m_dbFileBak);
    int status = system(cmd);
    if (status == -1 || WIFEXITED(status) == 0)
        printf("execute [%s] failed.\n", cmd);

    //重新计算备份数据库db文件的大小
    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

    //重新计算备份数据库db文件的MD5值
    ret = GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);
    if (ret != 0)
        return DATABASE_ERROR;

    m_pDbBase->Close();

    CopyFile(m_dbFileBak, m_dbFile); //把flash里备份的db文件拷贝到内存里
    //重新打开内存的数据db文件
    ret = m_pDbBase->Open();
    if (ret != DATABASE_OK)
        return ret;

    return DATABASE_OK;
}

int IcCard_DbExtension::UnInit()
{
    AUTO_LOCK(mtx);
    int ret;
    m_bInit = false; //定时备份线程检测到这个变量为false，就会自动结束。
    while (m_bakupThreadId != NULL)//说明定时备份的线程还没结束。需要等它结束才可以关闭数据库。否则可能出错。
    {
        usleep(100000);
    }

    //避免备份线程结束的过程中若进行数据插入的情况，
    //清空数据库空闲队列。
    m_pDbBase->CleanUpQueue();
    ret = m_pDbBase->Close();

    CopyFile(m_dbFile, m_dbFileBak); //把内存里的db文件拷贝到flash里去

    //重新计算备份数据库db文件的大小
    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

    //重新计算备份数据库db文件的MD5值
    GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);

    if (ret != DATABASE_OK)
        return ret;

    return DATABASE_OK;
}

int IcCard_DbExtension::ManulBak()
{
    AUTO_LOCK(mtx);
    if (!m_bInit)
        return DATABASE_NOT_INIT;
    int ret;

    //清空数据库空闲队列。
    m_pDbBase->CleanUpQueue();
    m_pDbBase->Close();

    CopyFile(m_dbFile, m_dbFileBak); //把内存里的db文件拷贝到flash里去
    //重新计算备份数据库db文件的大小
    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

    //重新计算备份数据库db文件的MD5值
    GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);

    m_pDbBase->Open(); //重新打开内存数据库文件  

    return DATABASE_OK;
}

int IcCard_DbExtension::CleanAll()
{
    AUTO_LOCK(mtx);
    if (!m_bInit)
        return DATABASE_NOT_INIT;
    m_pDbBase->CleanAll();

    //存放在flash里的备份数据库平时是不打开的
    m_pDbBakBase->Open();
    m_pDbBakBase->CleanAll();
    m_pDbBakBase->Close();
    return DATABASE_OK;
}

/**
 * 拷贝文件
 * @param srcFilePath 拷贝源文件名的全路径
 * @param dstFilePath 拷贝目的文件名全路径
 * @return 成功返回0，返回大于0的数表示错误代码
 */
int IcCard_DbExtension::CopyFile(char *srcFilePath, char *dstFilePath)
{
    int from_fd, to_fd;
    int bytes_read, bytes_write;
    char buffer[BUFFER_SIZE];
    char *ptr;

    if ((from_fd = open(srcFilePath, O_RDONLY)) == -1)
    {
        LOG_E("Open %s Error:%s\n", srcFilePath, strerror(errno));
        return DATABASE_BLOCK;
    }

    if ((to_fd = open(dstFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
    {
        LOG_E("Open %s Error:%s\n", dstFilePath, strerror(errno));
        return DATABASE_BLOCK;
    }

    LOG_E("Ready for Writing!\n");

    while (bytes_read = read(from_fd, buffer, BUFFER_SIZE))
    {
        // 读取数据库临时文件至缓冲区
        if ((bytes_read == -1)&&(errno != EINTR))
            break;
        else if (bytes_read > 0)
        {
            ptr = buffer;
            // 将读取到的缓冲区域写进备份数据库文件中
            while (bytes_write = write(to_fd, ptr, bytes_read))
            {
                if ((bytes_write == -1)&&(errno != EINTR))
                    break;
                else if (bytes_write == bytes_read)
                    break;
                else if (bytes_write > 0)
                {
                    ptr += bytes_write;
                    bytes_read -= bytes_write;
                }
            }
            if (bytes_write == -1)
                break;
        }
    }
    LOG_E("Finished Writing!\n");

    close(from_fd);
    close(to_fd);

    return DATABASE_OK;
}

int IcCard_DbExtension::CompareToBakDb(long long int dbFileSize, unsigned char md5Value[])
{
    AUTO_LOCK(mtx);

    if (m_bakDbFileSize != dbFileSize)
        return 1; //大小不同，说明两个数据库db文件肯定不同。

    if (memcmp(md5Value, m_bakDbFileMD5Value, sizeof (m_bakDbFileMD5Value)) != 0)
        return 2; //MD5值不同，说明两个数据库db文件肯定不同。

    return DATABASE_OK;
}

int IcCard_DbExtension::StopAutoBak(void)
{
    // 停止数据库文件自动备份功能
    bAutoBakSwitch = false;
    return DATABASE_OK;
}

int IcCard_DbExtension::StartAutoBak()
{
    AUTO_LOCK(mtx);
    if (!m_bInit)
        return DATABASE_NOT_INIT;
    bAutoBakSwitch = true;
    return DATABASE_OK;
}

void* IcCard_DbExtension::AutoBakThreadCallback(void *arg)
{
    IcCard_DbExtension *ptr = (IcCard_DbExtension *) arg;
    ptr->AutoBakThreadFunc();
    pthread_exit(NULL);
}

void* IcCard_DbExtension::AutoBakThreadFunc()
{
    int ret;
    long int fileSize = 0;
    unsigned char tmpMD5Value[16];
    unsigned int count = 0;
    unsigned int maxcount = 0;
    maxcount = m_bakupInterval * 5; //200毫秒循环一次，要5次才算是1秒

    while (m_bInit)
    {
        if (bAutoBakSwitch) // 要停止自动备功能，只需要设置此变量为0即可。      
        {
            count++;
            if (count > maxcount)//count经过5从哦就是1秒数
            {
                count = 0;

                //计算内存数据库db文件的大小
                fileSize = GetDbFileSize(m_dbFile);

                //计算内存数据库db文件的MD5值
                bzero(tmpMD5Value, sizeof (tmpMD5Value));
                ret = GetDbFileMD5(m_dbFile, tmpMD5Value);
                if (ret != 0)
                    continue;

                ret = CompareToBakDb(fileSize, tmpMD5Value);
                if (ret != 0)//内存数据库db文件和flash里的数据库db文件不一致，所以需要同步
                {
                    ManulBak(); //调用手动备份数据库函数，进行一次数据库备份操作
                }
            }
        }
        else
        {
            count = 0;
        }

        usleep(200000); //每200毫秒检测一次线程是否需要结束
    }

    m_bakupThreadId = NULL; //线程结束，要把它设置为NULL
}

/**
 * @函数功能：获取数据库文件的文件大小的字节数
 * @返回值： 数据库文件的文件大小的字节数
 */
long long int IcCard_DbExtension::GetDbFileSize(char *dbFilePath)
{
    if (access(dbFilePath, F_OK) != 0)//等于0表示文件存在。不存在会返回-1
    {
        return 0; //文件不存在。
    }
    /*
        fstream m_file;
        m_file.open(dbFilePath, ios::in | ios::binary);
        m_file.seekg(0x00, ios::end);
        int size = m_file.tellg();   
        m_file.close();
        return size;
     */

    long int size = 0;
    struct stat statbuff;
    if (stat(dbFilePath, &statbuff) >= 0)
        size = statbuff.st_size;
    return size;
}

/**
 * 计算数据文件的MD5值
 * @param md5 16字节数组，保存了计算出来的数据文件的MD5值。调用前需要申请内存。
 * @return 成功返回0，返回大于0的数表示错误代码
 */
int IcCard_DbExtension::GetDbFileMD5(char *dbFilePath, unsigned char md5Value[])
{
    FILE *fp = fopen(dbFilePath, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (fp == NULL)
    {
        fprintf(stderr, "fopen %s failed\n", dbFilePath);
        return DATABASE_FILE_PATH_ILLEGAL;
    }

    MD5Init(&mdContext);
    while ((bytes = fread(data, 1, 1024, fp)) != 0)
    {
        MD5Update(&mdContext, data, bytes);
    }
    MD5Final(&mdContext, md5Value);

    fclose(fp);
    return DATABASE_OK;
}