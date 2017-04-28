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
 * File:   main.cpp
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

 #include "Tester.h"


Tester::Tester()
{
    ;
}

Tester::~Tester()
{
    ;
}

int Tester::Process(void)
{
    int ret = 0;
    char pcmd[128] = "";
    int i = 0, j = 0;
    unsigned char errinput = 0;
    char Scmd[32][32];
    while (1)
    {
        ShowHelpInfo();
        memset(pcmd, 0, sizeof (pcmd));
        gets(pcmd);

        if (pcmd[0] == 'q')
            return 0;

        if (pcmd[0] == 'c')
        {
            system("clear");
            continue;
        }

        memset(Scmd, 0, sizeof (Scmd));
        j = 0;
        i = 0;
        while (pcmd[i] != '\0')
        {
            if (pcmd[i] != ' ')
            {
                if (pcmd[i + 1] == ' ' || pcmd[i + 1] == '\0')
                {
                    if (cti(pcmd[i]) < 1 || cti(pcmd[i]) > 9)
                    {
                        errinput = 1;
                        break;
                    }
                    Scmd[j][0] = pcmd[i];
                    j++;
                }
                else
                {
                    if (pcmd[i + 2] == ' ' || pcmd[i + 2] == '\0')
                    {
                        if (cti(pcmd[i]) != 1 || cti(pcmd[i + 1]) > 15)
                        {
                            errinput = 2;
                            break;
                        }
                        Scmd[j][0] = pcmd[i];
                        Scmd[j][1] = pcmd[i + 1];
                        j++;
                        i++;
                    }
                    else
                    {
                        errinput = 3;
                        break;
                    }
                }
            }

            i++;
        }

        if (errinput != 0)
        {
            printf("Unvalidated Input:%d\r\n", errinput);
            errinput = 0;
            continue;
        }

        for (i = 0; i < 32; i++)
        {
            if (Scmd[i][0] == '\0')
                break;

            ret = parseNDCmdForDb(Scmd[i]);
            if (ret < 0)
                break;
        }
    };

    return 0;
}

int Tester::parseNDAssistDb(const char *cmd)
{
    //wait for auto tester
    return 0;
}

int Tester::parseNDCmdForDb(const char *cmd)
{
    int ret = 0;
    unsigned int count = 0;
    unsigned int z = 0;
    Stime startime;
    Stime endtime;
    char pcmd[128] = "";

    if (cmd == NULL)
        return -1;

    if (0 == strcmp(cmd, "1"))
    {
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Init((char*)DB_TMP, (char*)DB_BAK, 10);
        PRI_TIME_TM("Initialization Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "2"))
    {
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->UnInit();
        PRI_TIME_TM("Padlock Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "3"))
    {
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->LoadLocalDB();
        PRI_TIME_TM("Inverse Backup Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "4"))
    {
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->StartAutoBak();
        PRI_TIME_TM("Start Automatic Synchronization! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "5"))
    {
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->StopAutoBak();
        PRI_TIME_TM("Stop Automatic Synchronization! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "6"))
    {
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->ManulBak();
        PRI_TIME_TM("Manual Backup Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "7"))
    {
        CARD_S icData;
        INPUT_CARD_SERINUM();
        icData.SerialNumber = atol(pcmd);
        INPUT_CARD_ID();
        icData.CardID = atol(pcmd);
        INPUT_CARD_EXPIRY();
        icData.ExpiryTime = atol(pcmd);
        INPUT_CARD_TYPE();
        icData.CardType = atol(pcmd);
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Add(icData);
        PRI_TIME_TM("Insert Single Record Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "8"))
    {
        INPUT_CARD_SERINUM();
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Delete(atol(pcmd));
        PRI_TIME_TM("Delete Single Record Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "9"))
    {
        INPUT_CARD_SERINUM();
        Gettime_usec(&startime);
        CARD_S icData;
        ret = DbManagment::GetInstance()->FindBySerialNumber(atol(pcmd), &icData);
        PRI_TIME_TM("Find Single Record Complete! Return Code[%d]\r\n", ret);
        if (ret != 0)
        {
            BLOCK();
            return -1;
        }
        else
        {
            printf("Record at this time:\r\n");
            printf("SerialNumber=%d CardID=%lld ExpiryTime=%d CardType=%d\r\n",
               icData.SerialNumber, icData.CardID, icData.ExpiryTime, icData.CardType);
        }
        BLOCK();
    }
    else if (0 == strcmp(cmd, "10"))
    {
        printf("Numbers of Insert Records: ");
        memset(pcmd, 0, sizeof (pcmd));
        gets(pcmd);
        count = atoi(pcmd);
        CARD_S icData[1024];
        memset(icData, 0, sizeof(icData));
        for(z = 0; z <count; z++)
        {
            printf("\r\n-------%d--------\r\n", z+1);
            INPUT_CARD_SERINUM();
            icData[z].SerialNumber = atol(pcmd);
            INPUT_CARD_ID();
            icData[z].CardID = atol(pcmd);
            INPUT_CARD_EXPIRY();
            icData[z].ExpiryTime = atol(pcmd);
            INPUT_CARD_TYPE();
            icData[z].CardType = atol(pcmd);
        }
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Add(icData, count);
        PRI_TIME_TM("Insert Multiple Record Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "11"))
    {
        printf("Numbers of Delete Records: ");
        memset(pcmd, 0, sizeof (pcmd));
        gets(pcmd);
        count = atoi(pcmd);
        z = 0;
        unsigned int serinum[1024];
        memset(serinum, 0, sizeof(serinum));
        while(count-- > 0)
        {
            printf("\r\n-------%d--------\r\n", z+1);
            INPUT_CARD_SERINUM();
            serinum[z] = atol(pcmd);
            z++;
        };

        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Delete(serinum, count);
        PRI_TIME_TM("Delete Multiple Record Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "12"))
    {
        printf("Input Alternate Database Path: ");
        memset(pcmd, 0, sizeof (pcmd));
        gets(pcmd);
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->ReplaceDbFile(pcmd);
        PRI_TIME_TM("Replace DataBase Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "13"))
    {
        printf("To Clean up Report 'y'or'n' ? : ");
        if (Getch() == 'y')
        {
            if (access(REPORT_FILE_NAME, R_OK) == 0)
            {
                char cCmd[256] = "";
                sprintf(cCmd, "rm -rf %s", REPORT_FILE_NAME);
                system(cCmd);
                printf("Clear up report.log\r\n");
            }
        }
        system("clear");
        char pbufcmd[256] = "";
        sprintf(pbufcmd, "sqlite3 %s \"select count(serial_num) from table_card;\"", DB_TMP);
        system(pbufcmd);
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->CleanAll();
        PRI_TIME_TM("Clean up DataBase Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "14"))
    {
        unsigned char md5Value[16] = {0};

        memset(md5Value, 0, sizeof(md5Value));
        printf("Provide MD5 Value: ");
        memset(pcmd, 0, sizeof (pcmd));
        gets(pcmd);
        str32ToInt128(pcmd, md5Value);

        printf("Provide File Size: ");
        memset(pcmd, 0, sizeof (pcmd));
        gets(pcmd);
        unsigned int filesize = 0;
        filesize = atol(pcmd);
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->CompareToBakDb(filesize, md5Value);
        PRI_TIME_TM("Compare the Similarity Between Tmp & Bak Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "15"))
    {
        system("clear");
        //ret = parseNDAssistDb(cmd);
    }
    else
    {
        ret = -1;
    }

    if (ret != 0)
    {
        printf("%s error status[%d]\n", cmd, ret);
        BLOCK();
    }

    return 0;
}

void Tester::str32ToInt128(char *src, unsigned char dstVaule[16])
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

    dstVaule[16]=0x00;
}

void Tester::Gettime_usec(Stime *tm)
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);

    tm->sec = 0;
    tm->usec = 0;
    tm->sec = tv.tv_sec;
    tm->usec = tv.tv_usec;
}

int Tester::CreatReport(void)
{
    char config_linebuf[256];
    char sum_buf[1024 * 20] = "";
    char sreport[1024 * 20] = "";
    FILE *f;

    if (access(REPORT_FILE_NAME, R_OK) == 0)
    {
        f = fopen(REPORT_FILE_NAME, "r+");
        if (f == NULL)
        {
            printf("OPEN file FALID\n");
            return 0;
        }

        fseek(f, 0, SEEK_SET);
        while (fgets(config_linebuf, 256, f) != NULL)
        {
            strcat(sum_buf, config_linebuf);

            if (fgetc(f) == EOF)
                break;

            fseek(f, -1, SEEK_CUR);
            memset(config_linebuf, 0, sizeof (config_linebuf));
        }

        strcat(sum_buf, "\n");
        fclose(f);
    }
    else
    {
        strcpy(sum_buf, " Data Size/Ten Thousand  Insert(s)/Millisecond   Delete(s)/Millisecond  Update(s)/Millisecond  BackUp/Millisecond:\r\n");
    }

    sprintf(sreport, "%s%6.6s      %10.10d       %10.10d      %10.10d      %10.10d \n ", sum_buf, rptfile.cnt, rptfile.atime, rptfile.dtime, rptfile.mtime, rptfile.btime);
    printf("\n%s\n", sreport);

    FILE *fp;
    fp = fopen(REPORT_FILE_NAME, "w+");
    if (fp == NULL)
    {
        printf("open %s failed\n", REPORT_FILE_NAME);
        return -1;
    }
    fseek(fp, 0, SEEK_SET);
    fputs(sreport, fp);
    fclose(fp);

    return 0;
}

int Tester::Getch(void)
{
    struct termios tm, tm_old;
    int fd = STDIN_FILENO, c;
    if(tcgetattr(fd, &tm) < 0)
        return -1;
    tm_old = tm;
    cfmakeraw(&tm);
    if(tcsetattr(fd, TCSANOW, &tm) < 0)
        return -1;
    c = fgetc(stdin);
    if(tcsetattr(fd, TCSANOW, &tm_old) < 0)
        return -1;
    return c;
}

void Tester::ShowHelpInfo(void)
{
    printf("\r\nTesting Options\r\n");
    printf("\tq  Quit\r\n");
    printf("\tc  Clear Screen\r\n");
    printf("\t1  Invoke Init\r\n");
    printf("\t2  Invoke Destroy\r\n");
    printf("\t3  Invoke LoadLocalDB\r\n");
    printf("\t4  Invoke StartAutoBak\r\n");
    printf("\t5  Invoke StopAutoBak\r\n");
    printf("\t6  Invoke ManulBak\r\n");
    printf("\t7  Invoke Add(single)\r\n");
    printf("\t8  Invoke Del(single)\r\n");
    printf("\t9  Invoke Find\r\n");
    printf("\t10 Invoke Add(multi)\r\n");
    printf("\t11 Invoke Del(multi)\r\n");
    printf("\t12 Invoke ReplaceDbFile\r\n");
    printf("\t13 Invoke CleanAll\r\n");
    printf("\t14 Invoke CompareToBakDb\r\n");
    printf("\t15 Auxiliary\r\n");
    printf("Commander: ");

    return;
}
