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
 * File:   DbExtension.cpp
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#include "DbExtension.h"
extern "C"
{
	#include "../md5/md5.h"
}

DbExtension::DbExtension()
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

DbExtension::~DbExtension()
{
}

int DbExtension::Init(char* dbFile, char* dbFileBak, unsigned int interval)
{
    AUTO_LOCK(mtx);
    if (m_bInit)
        return DATABASE_OK;

    m_bInit = false;

    int ret;
    m_bakupInterval = interval;
    strcpy(m_dbFile, dbFile);
    strcpy(m_dbFileBak, dbFileBak);

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
        m_pDbBakBase->ReLoadInfo();
    }
    m_pDbBakBase->Close();

    ret = GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);
    if (ret != 0)
        return DATABASE_ERROR;

    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

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
        m_pDbBase->ReLoadInfo();
    }

    bAutoBakSwitch = false;
    ret = pthread_create(&m_bakupThreadId, NULL, DbExtension::AutoBakThreadCallback, this);
    if (ret != 0)
    {
        printf("Thread creation failed.\n");
        return DATABASE_PTHREAD;
    }

    m_bInit = true;
    return DATABASE_OK;
}

int DbExtension::LoadLocalDB()
{
    AUTO_LOCK(mtx);
    
    if (!m_bInit)
    {
        return DATABASE_NOT_INIT;
    }
    
    int ret;
    m_pDbBase->CleanUpQueue();
    m_pDbBase->Close();

    CopyFile(m_dbFileBak, m_dbFile);

    ret = m_pDbBase->Open();
    if (ret != DATABASE_OK)
    {
        return ret;
    }
    return DATABASE_OK;
}

int DbExtension::ReplaceDbFile(char* dbFilePath)
{
    AUTO_LOCK(mtx);

    if (dbFilePath == NULL)
    {
        return DATABASE_FILE_PATH_ILLEGAL;
    }

    int ret;
    ret = m_pDbBase->IsDbFile(dbFilePath);
    if (ret != 0)
    {
        return DATABASE_ILLIGAL_DATA;
    }

    char cmd[256] = {0};
    sprintf(cmd, "mv %s %s", dbFilePath, m_dbFileBak);
    int status = system(cmd);
    if (status == -1 || WIFEXITED(status) == 0)
    {
        printf("execute [%s] failed.\r\n", cmd);
    }

    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

    ret = GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);
    if (ret != 0)
    {
        return DATABASE_ERROR;
    }

    m_pDbBase->Close();

    CopyFile(m_dbFileBak, m_dbFile);
    
    ret = m_pDbBase->Open();
    if (ret != DATABASE_OK)
    {
        return ret;
    }

    return DATABASE_OK;
}

int DbExtension::UnInit()
{
    AUTO_LOCK(mtx);
    int ret;
    m_bInit = false;
    while (m_bakupThreadId != NULL)
    {
        usleep(100000);
    }

    m_pDbBase->CleanUpQueue();
    ret = m_pDbBase->Close();

    CopyFile(m_dbFile, m_dbFileBak);

    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

    GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);

    if (ret != DATABASE_OK)
    {
        return ret;
    }

    return DATABASE_OK;
}

int DbExtension::ManulBak()
{
    AUTO_LOCK(mtx);
    if (!m_bInit)
    {
        return DATABASE_NOT_INIT;
    }

    m_pDbBase->CleanUpQueue();
    m_pDbBase->Close();

    CopyFile(m_dbFile, m_dbFileBak);

    m_bakDbFileSize = GetDbFileSize(m_dbFileBak);

    GetDbFileMD5(m_dbFileBak, m_bakDbFileMD5Value);

    m_pDbBase->Open();

    return DATABASE_OK;
}

int DbExtension::CleanAll()
{
    AUTO_LOCK(mtx);
    
    if (!m_bInit)
    {
        return DATABASE_NOT_INIT;
    }
    
    m_pDbBase->CleanAll();
    m_pDbBakBase->Open();
    m_pDbBakBase->CleanAll();
    m_pDbBakBase->Close();
    
    return DATABASE_OK;
}

int DbExtension::CopyFile(char *srcFilePath, char *dstFilePath)
{
    int from_fd, to_fd;
    int bytes_read, bytes_write;
    char buffer[BUFFER_SIZE];
    char *ptr;

    if ((from_fd = open(srcFilePath, O_RDONLY)) == -1)
    {
        printf("Open %s Error:%s\n", srcFilePath, strerror(errno));
        return DATABASE_BLOCK;
    }

    if ((to_fd = open(dstFilePath, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1)
    {
        printf("Open %s Error:%s\n", dstFilePath, strerror(errno));
        return DATABASE_BLOCK;
    }

    printf("Ready for Writing!\n");

    while ((bytes_read = read(from_fd, buffer, BUFFER_SIZE))>0)
    {
        if ((bytes_read == -1)&&(errno != EINTR))
            break;
        else if (bytes_read > 0)
        {
            ptr = buffer;
            while ((bytes_write = write(to_fd, ptr, bytes_read))>0)
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
    printf("Finished Writing!\n");

    close(from_fd);
    close(to_fd);

    return DATABASE_OK;
}

int DbExtension::CompareToBakDb(long long int dbFileSize, unsigned char md5Value[])
{
    AUTO_LOCK(mtx);

    if (m_bakDbFileSize != dbFileSize)
    {
        return 1;
    }

    if (memcmp(md5Value, m_bakDbFileMD5Value, sizeof (m_bakDbFileMD5Value)) != 0)
    {
        return 2;
    }

    return DATABASE_OK;
}

int DbExtension::StopAutoBak(void)
{
    bAutoBakSwitch = false;
    return DATABASE_OK;
}

int DbExtension::StartAutoBak()
{
    AUTO_LOCK(mtx);
    
    if (!m_bInit)
    {
        return DATABASE_NOT_INIT;
    }
    
    bAutoBakSwitch = true;
    return DATABASE_OK;
}

void* DbExtension::AutoBakThreadCallback(void *arg)
{
    DbExtension *ptr = (DbExtension *) arg;
    ptr->AutoBakThreadFunc();
    pthread_exit(NULL);
}

void* DbExtension::AutoBakThreadFunc()
{
    int ret;
    long int fileSize = 0;
    unsigned char tmpMD5Value[16];
    unsigned int count = 0;
    unsigned int maxcount = 0;
    maxcount = m_bakupInterval * 5;

    while (m_bInit)
    {
        if (bAutoBakSwitch)
        {
            count++;
            if (count > maxcount)
            {
                count = 0;

                fileSize = GetDbFileSize(m_dbFile);

                bzero(tmpMD5Value, sizeof (tmpMD5Value));
                ret = GetDbFileMD5(m_dbFile, tmpMD5Value);
                if (ret != 0)
                    continue;

                ret = CompareToBakDb(fileSize, tmpMD5Value);
                if (ret != 0)
                {
                    ManulBak();
                }
            }
        }
        else
        {
            count = 0;
        }

        usleep(200000);
    }

    m_bakupThreadId = NULL;

    return NULL;
}

long long int DbExtension::GetDbFileSize(char *dbFilePath)
{
    if (access(dbFilePath, F_OK) != 0)
    {
        return 0;
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
    {
        size = statbuff.st_size;
    }
    return size;
}

int DbExtension::GetDbFileMD5(char *dbFilePath, unsigned char md5Value[])
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
