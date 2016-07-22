/* 
 * File:   CAutoLock.h
 * Author: CAI
 *
 * Created on 2012年4月14日, 下午4:57
 */

#ifndef CAUTOLOCK_H
#define	CAUTOLOCK_H
#include "ThreadMutex.h"

class CAutoLock
{
public:
        CAutoLock(ThreadMutex &mtx) : m_mtx(mtx)
        {
                m_mtx.lock();
        }
        ~CAutoLock()
        {
                m_mtx.unlock();
        }
protected:
        ThreadMutex &m_mtx;
};

#define AUTO_LOCK(mtx)  CAutoLock cal(mtx)

#endif	/* CAUTOLOCK_H */

