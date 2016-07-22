/* 
 * File:   UtilClass.cpp
 * Author: CAI
 * 该类的主要功能为实现一些常用的函数功能
 * Created on 2014年9月12日, 下午6:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>


#include <linux/sockios.h>
#include <errno.h>

#include <net/if_arp.h>
#include <sys/statfs.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "UtilClass.h"
#include "LOG_D.h"



UtilClass::UtilClass()
{
}

UtilClass::UtilClass(const UtilClass& orig)
{
}

UtilClass::~UtilClass()
{
}

uint64_t UtilClass::ReversInt64(uint64_t value)
{
    unsigned char buf[8];
    memcpy(buf, &value, 8);
    uint64_t tmp;
    for (int i = 0; i < 4; ++i)
    {
        char ch = buf[i];
        buf[i] = buf[7 - i];
        buf[7 - i] = ch;
    }
    memcpy(&tmp, buf, 8);
    return tmp;
}


int UtilClass::Gettmpfreespace()
{
    struct statfs diskInfo;
    int tmpspaceMB = 0x00;

    statfs("/tmp", &diskInfo);
    unsigned long long blocksize = diskInfo.f_bsize; //每个block里包含的字节数
    unsigned long long totalsize = blocksize * diskInfo.f_blocks; //总的字节数，f_blocks为block的数目
    LOG_D("Total_size = %llu B = %llu KB = %llu MB = %llu GB\n",
            totalsize, totalsize >> 10, totalsize >> 20, totalsize >> 30);

    unsigned long long freeDisk = diskInfo.f_bfree * blocksize; //剩余空间的大小
    unsigned long long availableDisk = diskInfo.f_bavail * blocksize; //可用空间大小
    LOG_D("Disk_free = %llu MB = %llu GB\nDisk_available = %llu MB = %llu GB\n",
            freeDisk >> 20, freeDisk >> 30, availableDisk >> 20, availableDisk >> 30);
    tmpspaceMB = (int) (availableDisk >> 20);
    return tmpspaceMB;

}

void UtilClass::ParseGMTTime(char* gmtTime)
{
    int i = 0x00, pos = 0x00;
    if (strlen(gmtTime) == 0x00)
        return;


    for (i = 0; i < strlen(gmtTime); i++)
    {
        char ch = gmtTime[i];
        if (ch >= '0' && ch <= '9')
        {
            pos = i;
            break;
        }
    }

    //日
    char date[4] = {0x00};
    int ndate = 0x00;
    for (i = pos; i < strlen(gmtTime); i++)
    {

        char ch = gmtTime[i];
        if (ch < '0' || ch > '9')
        {
            pos = i;
            ndate = atoi(date);
            break;
        }
        date[i - pos] = ch;
    }

    //月
    char mon[10] = {0x00};
    int nmon = 0x00;
    for (i = pos; i < strlen(gmtTime); i++)
    {
        char ch = gmtTime[i];
        if (ch != ' ')
        {
            pos = i;
            break;
        }
    }
    //月
    for (i = pos; i < strlen(gmtTime); i++)
    {
        char ch = gmtTime[i];
        if (ch == ' ')
        {
            pos = i;
            nmon = UtilClass::GetMonth(mon);
            break;
        }
        mon[i - pos] = ch;
    }


    //年
    char year[6] = {0x00};
    int nYear = 0x00;
    for (i = pos; i < strlen(gmtTime); i++)
    {
        char ch = gmtTime[i];
        if (ch >= '0' && ch <= '9')
        {
            pos = i;
            break;
        }
    }

    for (i = pos; i < strlen(gmtTime); i++)
    {
        char ch = gmtTime[i];
        if (ch == ' ')
        {
            pos = i;
            nYear = atoi(year);
            break;
        }

        year[i - pos] = ch;
    }



    char sztime[10] = {0x00};
    //时间
    for (i = pos; i < strlen(gmtTime); i++)
    {
        char ch = gmtTime[i];
        if (ch >= '0' && ch <= '9')
        {
            pos = i;
            break;
        }
    }

    for (i = pos; i < strlen(gmtTime); i++)
    {
        char ch = gmtTime[i];
        if (ch == ' ')
        {
            pos = i;
            break;
        }

        sztime[i - pos] = ch;
    }

    int hour = 0, min = 0, sec = 0;
    UtilClass::GetTime(sztime, hour, min, sec);

    char timeSet[20] = {0x00};
    sprintf(timeSet, "%02d%02d%02d%02d%d", nmon, ndate, hour, min, nYear);

    char cmd[100] = {0x00};
    sprintf(cmd, "date -u %s", timeSet);

    system(cmd);

    char cmd_second[100] = {0x00};
    sprintf(cmd_second, "date -u %02d:%02d:%02d", hour, min, sec);
    system(cmd_second);


/*
    putenv((char*)"TZ=CST-8");
    usleep(1000 * 50);

    //system("/mnt/mtd/reboot_ctrl.sh");

    time_t t1;
    struct tm *gmt, *area;

    t1 = time(NULL);
    area = localtime(&t1);
    gmt = gmtime(&t1);

    usleep(1000);
*/


}

int UtilClass::GetMonth(char* month)
{
    int ret = 0x00;
    if (strcmp(month, "Jan") == 0x00)
    {
        ret = 1;
    }
    else if (strcmp(month, "Feb") == 0x00)
    {
        ret = 2;
    }
    else if (strcmp(month, "Mar") == 0x00)
    {
        ret = 3;
    }
    else if (strcmp(month, "Apr") == 0x00)
    {
        ret = 4;
    }
    else if (strcmp(month, "May") == 0x00)
    {
        ret = 5;
    }
    else if (strcmp(month, "Jun") == 0x00)
    {
        ret = 6;
    }
    else if (strcmp(month, "Jul") == 0x00)
    {
        ret = 7;
    }
    else if (strcmp(month, "Aug") == 0x00)
    {
        ret = 8;
    }
    else if (strcmp(month, "Sep") == 0x00)
    {
        ret = 9;
    }
    else if (strcmp(month, "Oct") == 0x00)
    {
        ret = 10;
    }
    else if (strcmp(month, "Nov") == 0x00)
    {
        ret = 11;
    }
    else if (strcmp(month, "Dec") == 0x00)
    {
        ret = 12;
    }
    return ret;
}

int UtilClass::GetTime(char* time, int& hour, int& min, int& sec)
{
    int i = 0x00, pos = 0x00;
    if (strlen(time) == 0x00)
        return 0x00;

    char szhour[3] = {0x00};
    char szmin[3] = {0x00};
    char szsec[3] = {0x00};

    for (i = 0; i < strlen(time); i++)
    {
        char ch = time[i];
        if (ch == ':')
        {
            pos = i;
            hour = atoi(szhour);
            break;
        }

        szhour[i] = ch;
    }
    ++pos;

    for (i = pos; i < strlen(time); i++)
    {
        char ch = time[i];
        if (ch == ':')
        {
            pos = i;
            min = atoi(szmin);
            break;
        }

        szmin[i - pos] = ch;
    }

    ++pos;

    strcpy(szsec, time + pos);
    sec = atoi(szsec);

    return 0x00;


}

unsigned long UtilClass::GetNowTimeMS()
{
    struct timeval clk0;
    struct timezone tz;
    unsigned long ms = 0;

    gettimeofday(&clk0, &tz);
    ms = clk0.tv_sec * 1000 + clk0.tv_usec / 1000;
    return ms;
}





string UtilClass::Base64Encode(const unsigned char* Data, int DataByte)
{
    //编码表
    const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    //返回值
    string strEncode;
    unsigned char Tmp[4] = {0};
    int LineLength = 0;
    for (int i = 0; i < (int) (DataByte / 3); i++)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        Tmp[3] = *Data++;
        strEncode += EncodeTable[Tmp[1] >> 2];
        strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
        strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
        strEncode += EncodeTable[Tmp[3] & 0x3F];
        if (LineLength += 4, LineLength == 76)
        {
            strEncode += "\r\n";
            LineLength = 0;
        }
    }
    //对剩余数据进行编码
    int Mod = DataByte % 3;
    if (Mod == 1)
    {
        Tmp[1] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
        strEncode += "==";
    }
    else if (Mod == 2)
    {
        Tmp[1] = *Data++;
        Tmp[2] = *Data++;
        strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
        strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
        strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
        strEncode += "=";
    }

    return strEncode;
}

string UtilClass::Base64Decode(const char* Data, int DataByte, int& OutByte)
{
    //解码表
    const char DecodeTable[] ={
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        62, // '+'
        0, 0, 0,
        63, // '/'
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
        0, 0, 0, 0, 0, 0,
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };
    //返回值
    string strDecode;
    int nValue;
    int i = 0;
    while (i < DataByte)
    {
        if (*Data != '\r' && *Data != '\n')
        {
            nValue = DecodeTable[*Data++] << 18;
            nValue += DecodeTable[*Data++] << 12;
            strDecode += (nValue & 0x00FF0000) >> 16;
            OutByte++;
            if (*Data != '=')
            {
                nValue += DecodeTable[*Data++] << 6;
                strDecode += (nValue & 0x0000FF00) >> 8;
                OutByte++;
                if (*Data != '=')
                {
                    nValue += DecodeTable[*Data++];
                    strDecode += nValue & 0x000000FF;
                    OutByte++;
                }
            }
            i += 4;
        }
        else// 回车换行,跳过
        {
            Data++;
            i++;
        }
    }
    LOG_D("%s \n", strDecode.c_str());
    return strDecode;
}


 void UtilClass::Int64toStr(uint64_t value, char* str)
 {
    char temp[20] = {0x00};
    sprintf(temp, "%llu", value);
    int strLen = strlen(temp);

    if (strLen < 16)
    {
        for (int i = 0; i < 16 - strLen; ++i)
        {
            *str = '0';
            ++str;
        }
    }

    strcpy(str, temp);
}

/**
 * 32个字符长度的16进制字面值字符串转化为16字节的整形数值。
 * @param src：32个字符长度的16进制字面值字符串  
 * @param dstValue: 16字节的整形数值
 */
void UtilClass::Str32ToInt128(char *src, uint8_t dstVaule[16])
{
    unsigned int position_s = 0;
    unsigned int position_p = 0;

    while (position_p != 32)
    {
        // 0xa0
        if (src[position_s] >= '0' && src[position_s] <= '9')
        {
            dstVaule[position_p] = ((unsigned char) src[position_s] - 48) * 16;
            position_s++;
        }
        else if (src[position_s] >= 'A' && src[position_s] <= 'F')
        {
            dstVaule[position_p] = ((unsigned char) src[position_s] - 55) * 16;
            position_s++;
        }
        else if (src[position_s] >= 'a' && src[position_s] <= 'f')
        {
            dstVaule[position_p] = ((unsigned char) src[position_s] - 87) * 16;
            position_s++;
        }

        // 0xaa = 0x0a + 0xa0
        if (src[position_s] >= '0' && src[position_s] <= '9')
        {
            dstVaule[position_p] += (unsigned char) src[position_s] - 48;
            position_s++;
        }
        else if (src[position_s] >= 'A' && src[position_s] <= 'F')
        {
            dstVaule[position_p] += (unsigned char) src[position_s] - 55;
            position_s++;
        }
        else if (src[position_s] >= 'a' && src[position_s] <= 'f')
        {
            dstVaule[position_p] += (unsigned char) src[position_s] - 87;
            position_s++;
        }

        position_p++;
    }
}
    
int UtilClass::MilliSecondSleep(int milliSeconds)
{
    struct timespec shortWait;
    struct timespec remainder = {0, 0};


    shortWait.tv_sec = milliSeconds / 1000;
    shortWait.tv_nsec = (milliSeconds % 1000) * 1000 * 1000;
    nanosleep(&shortWait, &remainder);

    return (remainder.tv_sec * 1000 + remainder.tv_nsec / 1000000 +
            ((remainder.tv_nsec % 1000000) ? 1 : 0));
}

/**
 * 根据程序名字找到对应的pid
 * @param procname：程序的名字  (输入)
 * @param foundpid: 找到对应的PID （输出）
 * @return 
 */
int UtilClass::Find_pid_by_name(char* procname, int* foundpid)
{
    int ret = -1;
    DIR *dir;
    struct dirent *d;
    int pid, i;
    char *s;
    int pnlen;

    i = 0;
    foundpid[0] = 0;
    pnlen = strlen(procname);

    /* Open the /proc directory. */
    dir = opendir("/proc");
    if (!dir)
    {
        LOG_D("Cannot open /proc");
        return ret;
    }

    /* Walk through the directory. */
    while ((d = readdir(dir)) != NULL)
    {

        char exe [PATH_MAX + 1];
        char path[PATH_MAX + 1];
        int len;
        int namelen;

        /* See if this is a process */
        if ((pid = atoi(d->d_name)) == 0) continue;

        snprintf(exe, sizeof (exe), "/proc/%s/exe", d->d_name);
        if ((len = readlink(exe, path, PATH_MAX)) < 0)
            continue;
        path[len] = '\0';

        /* Find ProcName */
        s = strrchr(path, '/');
        if (s == NULL) continue;
        s++;

        /* we don't need small name len */
        namelen = strlen(s);
        if (namelen < pnlen) continue;

        if (!strncmp(procname, s, pnlen))
        {
            /* to avoid subname like search proc tao but proc taolinke matched */
            if (s[pnlen] == ' ' || s[pnlen] == '\0')
            {
                foundpid[i] = pid;
                i++;
                ret = 0x00;
            }
        }
    }

    foundpid[i] = 0;
    closedir(dir);
    return ret;
}

/**
 * 杀掉一个进程
 * @param procname：程序的名字
 * @return 
 */
int UtilClass::Killpro(char* procname)
{
    int ret = 0x00;
    int i = 0x00, rv = 0x00, pid_t[128] = {0x00};

    rv = Find_pid_by_name(procname, pid_t);
    if (rv == 0)
    {
        for (i = 0; pid_t[i] != 0; i++)
        {
            char shell_cmd[24] = {0x00};
            sprintf(shell_cmd, "kill -9 %d", pid_t[i]);
            system(shell_cmd);
        }
    }
    LOG_D("kill process. name:[%s], ret:[%d]\n", procname, ret);
    return ret;    
}













