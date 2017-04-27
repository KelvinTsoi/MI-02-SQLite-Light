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
 * File:   ThreadMutex.h
 * Author: CAI
 * Created on 2016/7/29, 9:58pm
 */

#ifndef THREADMUTEX_H
#define	THREADMUTEX_H
#include <pthread.h>

//线程互斥量
class ThreadMutex
{
public:
        ThreadMutex()
        {
                pthread_mutex_init(&mtx,NULL);
        }

        ~ThreadMutex()
        {
                pthread_mutex_destroy( &mtx );
        }

        inline void lock()
        {
                pthread_mutex_lock( &mtx );
        }

        inline void unlock()
        {
                pthread_mutex_unlock( &mtx );
        }
private:
        pthread_mutex_t mtx;

}; 


#endif	/* THREADMUTEX_H */

