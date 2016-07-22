/* 
 * File:   ThreadMutex.h
 * Author: CAI
 *
 * Created on 2012年4月14日, 下午4:48
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

