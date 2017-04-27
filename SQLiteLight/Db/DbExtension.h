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
 * File:   DbExtension.h
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#ifndef _DBEXTENSION_H
#define _DBEXTENSION_H
#include "DbOperator.h"

class DbExtension
{
public:

	/**
	 * Constructor;
	 */
    DbExtension();

    /**
     * Destructor;
     */
    virtual ~DbExtension();

    /**
     * @function: module initialization;
     * @parameter: dbFile storage path of memory data base;
     * @parameter: dbFileBak storage path of back up data base;
     * @parameter: interval timing backup[Auto back up process](from memory data base to back up data base);
     * @return: success return 0, others signify function error code;
     */
    virtual int Init(char* dbFile, char* dbFileBak, unsigned int interval);

    /**
     * function: reverse backup(from back up data base to memory data base);
     * return: success return 0, others signify function error code;
     */
    virtual int LoadLocalDB();

    /**
     * @function: replace back up data base by download data base from cloud;
     * @parameter: datFile storage path of download data base;
     * @return: success return 0, others signify function error code;
     */
    int ReplaceDbFile(char* datFile);

    /**
     * @function: obstructed manual back up (from memory data base to back up data base)
     * @return: success return 0, others signify function error code;
     */
    virtual int ManulBak();

    /**
     * @function: stop auto back up process;
     * @return: success return 0, others signify function error code;
     */
    int StopAutoBak();

    /**
     * @function: start auto back up process;
     * @return: success return 0, others signify function error code;
     */
    int StartAutoBak();

    /**
     * @function: recovery of resources;
     * @return: success return 0, others signify function error code;
     */
    virtual int UnInit();

    /**
     * @function: copy source file;
     * @parameter: srcFilePath storage path of source file;
     * @parameter: dstFilePath storage path of clone file;
     * @return: success return 0, others signify function error code;
     */
    int CopyFile(char *srcFilePath, char *dstFilePath);

    /**
     * @function: asynchronous clean up date base;
     * @return: success return 0, others signify function error code;
     */
    int CleanAll();

    /**
     * @function: compare current data base and back up data base according file size and md5 value;
     * @parameter: dbFileSize current data base file size;
     * @parameter: md5Value current data base file md5 value;
     * @return: @return: both data base are same return 0, others signify function error code;
     */
    int CompareToBakDb(long long int dbFileSize, unsigned char md5Value[]);

    /**
     * @function: get current data base file size(Bytes);
     * @parameter: dbFilePath storage path of current data base;
     * @return: file size(Bytes);
     */
    static long long int GetDbFileSize(char *dbFilePath);

    /**
     * @function: calculate data base file md5 value;
     * @parameter: dbFilePath storage path of current data base;
     * @parameter: md5Value current data base file md5 value(16 Bytes array);
     * @return: success return 0, others signify function error code;
     */
    static int GetDbFileMD5(char *dbFilePath, unsigned char md5Value[]);

protected:

    /**
     * memory data base;
     */
    DbOperator *m_pDbBase;

    /**
     * back up data base;
     */
    DbOperator *m_pDbBakBase;

    /**
     * storage path of dat file;
     */
    char m_datFile[100];

    /**
     * storage path of memory data base;
     */
    char m_dbFile[100];

    /**
     * storage path of back up data base;
     */
    char m_dbFileBak[100];

    /**
     * Auto back up process conditional variable[true=>start, false=>stop];
     */
    bool bAutoBakSwitch;

    /**
     *  initialization of mutex lock;
     */
    ThreadMutex mtx;

    /**
     * back up data base file md5 value(16 Bytes array);
     */
    unsigned char m_bakDbFileMD5Value[16];

    /**
     * back up data base file size;
     */
    long long int m_bakDbFileSize;


    /**
     * auto back up process interval time;
     */
    unsigned int m_bakupInterval;

private:

    /**
     * @function: thread function, fire up auto back up process;
     * @parameter: arg parameter transfer to thread function;
     */
    static void *AutoBakThreadCallback(void *arg);

    /**
     * @function: auto back up process function;
     */
    void* AutoBakThreadFunc();

    /**
     * module is initialized or not[0 signify uninitialized, 1 signify initialized];
     */
    bool m_bInit;

    /**
     * id of auto back up thread;
     */
    pthread_t m_bakupThreadId;
};

#endif /* _DBEXTENSION_H */

