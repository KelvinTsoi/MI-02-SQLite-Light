/* 
 * File:   LOG_D.h
 * Author: CAI
 *
 * Created on 2014年9月12日, 下午2:27
 */

#ifndef LOG_D_H
#define	LOG_D_H
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#ifdef ANDROID
#include <android/log.h> 
#endif

#ifdef	__cplusplus
extern "C"
{
#endif


#define LOG_INFO_STR    "INFO"
#define LOG_DEBUG_STR   "DEBUG"
#define LOG_WARN_STR    "WARNING"
#define LOG_ERROR_STR   "ERROR"

#define PRO_NAME        "dbmanager"
//声明两个全局变量。（extern后复值就是定义变量，不赋值就是仅仅声明）   
extern int LOG_LEVEL;                   //日志级别可以从0-4，级别越低，日志权限越低
extern int SYSLOG_ENABLE;               //0x00: 表示不可用syslog  0x01: 表示syslog可用
extern const char* APPNAME;               //应用程序的名称
   
//syslog(LOG_DEBUG, "[Level:%s][Function:%s@ Line:%d]:" format, LOG_DEBUG_STR, __FUNCTION__,__LINE__, ##__VA_ARGS__); \

#ifndef LOG_D
#ifdef ANDROID

#define LOG_I( format, ...) \
    do { \
        if(LOG_LEVEL<4)  break;\
        fprintf(stderr, "[%s][%s][%s @line:%d] " format ,LOG_INFO_STR,__FILE__,__FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
        __android_log_print(ANDROID_LOG_INFO, PRO_NAME, "[%s][%s @line:%d]" format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define LOG_D( format, ...) \
    do { \
        if(LOG_LEVEL<3)  break;\
        fprintf(stderr, "[%s][%s][%s @line:%d] " format ,LOG_DEBUG_STR,__FILE__, __FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
        __android_log_print(ANDROID_LOG_DEBUG, PRO_NAME, "[%s][%s @line:%d]" format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        } while (0)

#define LOG_W( format, ...) \
    do { \
        if(LOG_LEVEL<2)  break;\
        fprintf(stderr, "[%s][%s][%s @line:%d] " format ,LOG_WARN_STR, __FILE__,__FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
        __android_log_print(ANDROID_LOG_WARN, PRO_NAME, "[%s][%s @line:%d]" format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)
           

#define LOG_E( format, ...) \
    do { \
        if(LOG_LEVEL<1)  break;\
        fprintf(stderr, "[%s][%s][%s @line:%d] " format ,LOG_ERROR_STR,__FILE__, __FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
        __android_log_print(ANDROID_LOG_ERROR, PRO_NAME, "[%s][%s @line:%d]" format, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
} while (0)

#else
#define LOG_I( format, ...) \
    do { \
        if(LOG_LEVEL<4)  break;\
        fprintf(stderr, "[%s][%s][%s][%s @line:%d] " format , \
            LOG_INFO_STR,APPNAME,__FILE__,__FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
            if (SYSLOG_ENABLE){syslog(LOG_DEBUG, "[%s][%s][%s][%s @line:%d] " format, LOG_INFO_STR, APPNAME,__FILE__,__FUNCTION__,__LINE__, ##__VA_ARGS__);} \
    } while (0)

#define LOG_D( format, ...) \
    do { \
        if(LOG_LEVEL<3)  break;\
        fprintf(stderr, "[%s][%s][%s][%s @line:%d] " format , \
            LOG_DEBUG_STR,APPNAME,__FILE__, __FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
            if (SYSLOG_ENABLE){syslog(LOG_DEBUG, "[%s][%s][%s][%s @line:%d] " format, LOG_DEBUG_STR,APPNAME, __FILE__,__FUNCTION__,__LINE__, ##__VA_ARGS__);} \
        } while (0)

#define LOG_W( format, ...) \
    do { \
        if(LOG_LEVEL<2)  break;\
        fprintf(stderr, "[%s][%s][%s][%s @line:%d] " format , \
            LOG_WARN_STR,APPNAME,__FILE__,__FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
            if (SYSLOG_ENABLE){syslog(LOG_DEBUG, "[%s][%s][%s][%s @line:%d] " format, LOG_WARN_STR, APPNAME,__FILE__,__FUNCTION__,__LINE__, ##__VA_ARGS__);} \
    } while (0)
           

#define LOG_E( format, ...) \
    do { \
        if(LOG_LEVEL<1)  break;\
        fprintf(stderr, "[%s][%s][%s][%s @line:%d] " format , \
            LOG_ERROR_STR,APPNAME,__FILE__, __FUNCTION__,  __LINE__, ##__VA_ARGS__ ); \
            if (SYSLOG_ENABLE){syslog(LOG_DEBUG, "[%s][%s][%s][%s @line:%d] " format, LOG_ERROR_STR, APPNAME,__FILE__,__FUNCTION__,__LINE__, ##__VA_ARGS__);} \
    } while (0)
#endif

#endif




#ifdef	__cplusplus
}
#endif

#endif	/* LOG_D_H */

