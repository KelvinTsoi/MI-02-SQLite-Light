/* 
 * File:   UtilClass.h
 * Author: CAI
 * 该类的主要功能为实现一些常用的函数功能
 * Created on 2014年9月12日, 下午6:26
 */

#ifndef UTILCLASS_H
#define	UTILCLASS_H


#include <stdint.h>
#include <string.h>
#include <string>
#include <dirent.h>

using namespace std;

/**
 *
 *该类的主要功能为实现一些常用的函数功能
 */
class UtilClass
{
public:
    /**
     * 构造、析构函数
     */
    UtilClass();
    UtilClass(const UtilClass& orig);
    virtual ~UtilClass();

    /**
     * 反转uint64类型
     * @param value：即将要反转的uint64类型
     * @return ：返回反转后的uint64
     */       
    static uint64_t ReversInt64(uint64_t value);
    

    
    /**
     * 获得/tmp目录下剩余的空间
     * @return：返回一个整数，以M为单位
     */       
    static int      Gettmpfreespace();
    
    /**
     * 解析GTM时间
     * @param gmtTime：gmt时间
     * @return
     */     
    static void     ParseGMTTime(char* gmtTime);
    static int      GetMonth(char* month);
    static int      GetTime(char* time, int& hour, int& min, int& sec);
    
    /**
     * 获取1970到现在经历的毫秒
     * @return ：返回经历的毫秒
     */    
    static unsigned long GetNowTimeMS();


     /**
     * base64编码
     * @param Data：即将要编码的数据  databyte: 长度
     * @return ：编码后的string对象
     */    
    static string   Base64Encode(const unsigned char* Data,int DataByte);
    
    
     /**
     * base64解码
     * @param Data：即将要解码的数据  databyte: 解码长度， OutByte：解码后的长度
     * @return ：编码后的string对象
     */      
    static string   Base64Decode(const char* Data, int DataByte, int& OutByte);
    
    
     /**
     * 64位整形数值转化为16个字符长度的16进制字面值，如果不够则左边0对齐
     * @param value：即将要转化的uint64数据  
     * @param str: 将转化后的uint64存入该字符串
     * @return 
     */    
    static void Int64toStr(uint64_t value, char* str);
   
    /**
     * 32个字符长度的16进制字面值字符串转化为16字节的整形数值。
     * @param src：32个字符长度的16进制字面值字符串  
     * @param dstValue: 16字节的整形数值
     */    
    static void Str32ToInt128(char *src, uint8_t dstValue[16]);
    
     /**
     * 让机器休眠一段时间
     * @param milliSeconds：即将要休眠的毫秒数  
     * @return 
     */        
    static int  MilliSecondSleep(int milliSeconds);

     /**
     * 根据程序名字找到对应的pid
     * @param procname：程序的名字  (输入)
     * @param foundpid: 找到对应的PID （输出）
     * @return 
     */    
     static int Find_pid_by_name(char* procname, int* foundpid);
     
      /**
     * 杀掉一个进程
     * @param procname：程序的名字
     * @return 
     */      
     static int Killpro(char* procname);
};

#endif	/* UTILCLASS_H */

