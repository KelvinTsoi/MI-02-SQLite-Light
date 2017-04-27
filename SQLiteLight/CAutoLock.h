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
 * File:   CAutoLock.h
 * Author: CAI
 * Created on 2016/7/29, 9:58pm
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

