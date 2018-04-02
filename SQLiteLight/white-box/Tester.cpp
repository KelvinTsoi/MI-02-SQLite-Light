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
 * File:   Tester.cpp
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

 #include "Tester.h"

 bool permitToExecuteParallelTest = false;

Tester::Tester()
{
    thread_add = 0;
    thread_find = 0;
    thread_delete = 0;
    thread_backup = 0;
}

Tester::~Tester()
{
    thread_add = 0;
    thread_find = 0;
    thread_delete = 0;
    thread_backup = 0;
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

            ret = MLevelMenu(Scmd[i]);
            if (ret < 0)
                break;
        }
    };

    return 0;
}

int Tester::ALevelMenu(const char *cmd)
{
    Stime startime;
    Stime endtime;
    char pcmd[128]="";
    int ret = 0;

    while(1)
    {
        ShowAssistInfo();
        memset(pcmd, 0, 128);
        gets(pcmd);

        if (pcmd[0] == 'b')
            return 0;

        if (pcmd[0] == '1')
        {
            printf("Automatic Insertion's Quantity: ");
            memset(pcmd, 0, sizeof (pcmd));
            gets(pcmd);
            Gettime_usec(&startime);

            for (int zz = 1; zz <= atol(pcmd); zz++)
            {
                DATA_S iData;
                iData.Sequence = CreateRandomSerialNumber();
                iData.ExpiryTime = GetNowTime();
                iData.Data = CreateRandomIdentificationNumber();
                iData.Type = zz % 2 ? 0x01 : 0x00;
                sprintf(iData.Content, "AutoSet");
                ret = DbManagment::GetInstance()->Add(iData);
                if (ret != 0)
                {
                    PRI_TIME_TM("Automatic Insertion Aborted! Return Code[%d]\r\n", ret);
                    BLOCK();
                    return -1;
                }
            }
            PRI_TIME_TM("Automatic Insertion Complete! Return Code[%d]\r\n", ret);
            BLOCK();
        }
        else if (pcmd[0] == '2')
        {
            printf("Automatic Module Testing Cycle-Times: ");
            memset(pcmd, 0, sizeof (pcmd));
            gets(pcmd);
            strcpy(rptfile.cnt, pcmd);

            for(int zz = 1; zz <= atoi(pcmd); zz++)
            {
                DATA_S iData;
                iData.Sequence = CreateRandomSerialNumber();
                iData.ExpiryTime = GetNowTime();
                iData.Data = CreateRandomIdentificationNumber();
                iData.Type = zz % 2 ? 0x01 : 0x00;
                sprintf(iData.Content, "AutoSet");
                Gettime_usec(&startime);
                ret = DbManagment::GetInstance()->Add(iData);
                if (ret != 0)
                {
                    printf("Add error.\r\n");
                    return -1;
                }
                PRI_TIME_TM("Insert Single Record Complete! Return Code[%d].\r\n", ret);
                GET_TIME(rptfile.atime);

                iData.ExpiryTime = GetNowTime()+1;
                Gettime_usec(&startime);
                ret = DbManagment::GetInstance()->Add(iData);
                if (ret != 0)
                {
                    printf("Add error.\r\n");
                    return -1;
                }
                PRI_TIME_TM("Update Single Record Complete! Return Code[%d]\r\n", ret);
                GET_TIME(rptfile.mtime);

                Gettime_usec(&startime);
                ret = DbManagment::GetInstance()->Delete(iData.Sequence);
                if (ret != 0)
                {
                    printf("Delete error.\r\n");
                    return -1;
                }
                PRI_TIME_TM("Delete Single Record Complete! Return Code[%d]\r\n", ret);
                GET_TIME(rptfile.dtime);

                Gettime_usec(&startime);
                ret = DbManagment::GetInstance()->ManulBak();
                if (ret != 0)
                {
                    printf("ManulBak error %d\r\n", ret);
                    return -1;
                }
                PRI_TIME_TM("Manual Backup Complete! Return Code[%d]\r\n", ret);
                GET_TIME(rptfile.btime);

                system("clear");

                ret = CreatReport();
                if (ret != 0)
                {
                    printf("Create Report Error %d\n", ret);
                    return -1;
                }
            }
        }
        else if (pcmd[0] == '3')
        {
            ParallelStress();
        }
        else
        {
            printf("Unvalidated Input:\n");
            continue;
        }

        if (ret != 0)
        {
            printf("%s error status[%d]\n", cmd, ret);
            printf("any key to continue:");
            Getch();
        }
    }

    return 0;
}

int Tester::MLevelMenu(const char *cmd)
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
        DATA_S iData;
		INPUT_INFO_SEQENCE();
        iData.Sequence = atol(pcmd);
        INPUT_INFO_TYPE();
        iData.Type = atol(pcmd);
        INPUT_INFO_EXPIRY();
        iData.ExpiryTime = atol(pcmd);
   		INPUT_INFO_DATA();
        iData.Data = atol(pcmd);
        INPUT_INFO_CONTENT();
        sprintf(iData.Content, pcmd);
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Add(iData);
        PRI_TIME_TM("Insert Single Record Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "8"))
    {
        INPUT_INFO_SEQENCE();
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Delete(atol(pcmd));
        PRI_TIME_TM("Delete Single Record Complete! Return Code[%d]\r\n", ret);
        BLOCK();
    }
    else if (0 == strcmp(cmd, "9"))
    {
        INPUT_INFO_SEQENCE();
        Gettime_usec(&startime);
        DATA_S iData;
        ret = DbManagment::GetInstance()->FindBySeqenceNumber(atol(pcmd), &iData);
        PRI_TIME_TM("Find Single Record Complete! Return Code[%d]\r\n", ret);
        if (ret != 0)
        {
            BLOCK();
            return -1;
        }
        else
        {
            printf("Record at this time:\r\n");
            printf("Seqence Number = %d  Data Type = %d Expire Time = %d Data = %lld ",
               iData.Sequence, iData.Type, iData.ExpiryTime, iData.Data);
            if(iData.Content != NULL)
            {
            	printf("Content = %s\r\n", iData.Content);
            }
            else
            {
            	printf("Content = NULL\r\n");
            }
        }
        BLOCK();
    }
    else if (0 == strcmp(cmd, "10"))
    {
        printf("Numbers of Insert Records: ");
        memset(pcmd, 0, sizeof (pcmd));
        gets(pcmd);
        count = atoi(pcmd);
        DATA_S icData[1024];
        memset(icData, 0, sizeof(icData));
        for(z = 0; z <count; z++)
        {
            printf("\r\n-------%d--------\r\n", z+1);

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
        unsigned int SeqenceNumber[1024];
        memset(SeqenceNumber, 0, sizeof(SeqenceNumber));
        while(count-- > 0)
        {
            printf("\r\n-------%d--------\r\n", z+1);
			INPUT_INFO_SEQENCE();
            SeqenceNumber[z] = atol(pcmd);
            z++;
        };

        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Delete(SeqenceNumber, count);
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
        sprintf(pbufcmd, "sqlite3 %s \"select count(seqence_number) from happy_table;\"", DB_TMP);
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
        unsigned int amount = 0;
        ret = DbManagment::GetInstance()->CountBySeqenceNumber(amount);
        PRI_TIME_TM("Counting Records in DataBase Complete! Return Code[%d]\r\n", ret);
        if (ret != 0)
        {
            BLOCK();
            return -1;
        }
        else
        {
            printf("Records Amount: %u\r\n", amount);
        }
        BLOCK();
    }
    else if (0 == strcmp(cmd, "16"))
    {
        system("clear");
        ret = ALevelMenu(cmd);
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
        strcpy(sum_buf, "\tData Size/Ten Thousand\tInsert(s)/Millisecond\tDelete(s)/Millisecond\tUpdate(s)/Millisecond\tBackUp/Millisecond:\r\n");
    }

    sprintf(sreport, "%s%6.6s\t\t\t%10.10d\t\t\t%10.10d\t\t\t%10.10d\t\t\r%10.10d \n ", sum_buf, rptfile.cnt, rptfile.atime, rptfile.dtime, rptfile.mtime, rptfile.btime);
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
    printf("\t15 Invoke CountBySerialNumber\r\n");
    printf("\t16 Auxiliary\r\n");
    printf("Commander: ");

    return;
}

void Tester::ShowAssistInfo(void)
{
    printf("\r\nAuxiliary Options\r\n");
    printf("\tb  Back to Parent Directory\r\n");
    printf("\t1  Automatic Insertion\r\n");
    printf("\t2  Automatic Testing Module and Generating Report\r\n");
    printf("\t3  Parallel Automatic Testing Module\r\n");
    printf("Commander: ");

    return;
}

unsigned int Tester::CreateRandomIdentificationNumber(void)
{
    srand(GetNowUs());
    return (unsigned int)rand();
}

unsigned int Tester::CreateRandomSerialNumber(void)
{
    srand(GetNowUs());
    return (unsigned int)((unsigned long long)RAND_LIMIT*rand()/(RAND_MAX));
}

unsigned int Tester::GetNowTime(void)
{
    struct timeval clk0;
    struct timezone tz;
    gettimeofday(&clk0, &tz);
    return clk0.tv_sec;
}

unsigned int Tester::GetNowUs(void)
{
    struct timeval clk0;
    struct timezone tz;
    gettimeofday(&clk0, &tz);
    return clk0.tv_usec;
}

void Tester::ParallelStress(void)
{
    permitToExecuteParallelTest = true;

    act.sa_handler = Ouch;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);

    pthread_create(&thread_add, NULL, RunParallelAddingThreadProc, (void *)this);
    pthread_create(&thread_find, NULL, RunParallelFindingThreadProc, (void *)this);
    pthread_create(&thread_delete, NULL, RunParallelDeletingThreadProc, (void *)this);
    pthread_create(&thread_backup, NULL, RunParallelBakingThreadProc, (void *)this);

    pthread_join(thread_add, NULL);
    pthread_join(thread_find, NULL);
    pthread_join(thread_delete, NULL);
    pthread_join(thread_backup, NULL);

    return;
}

void* Tester::RunParallelAddingThreadProc(void* param)
{
    Tester* pthis = (Tester*) param;
    pthis->ParallelAdding();
    return NULL;
}

void* Tester::RunParallelFindingThreadProc(void* param)
{
    Tester* pthis = (Tester*) param;
    pthis->ParallelFinding();
    return NULL;
}

void* Tester::RunParallelDeletingThreadProc(void* param)
{
    Tester* pthis = (Tester*) param;
    pthis->ParallelDeleting();
    return NULL;
}

void* Tester::RunParallelBakingThreadProc(void* param)
{
    Tester* pthis = (Tester*) param;
    pthis->ParallelBaking();
    return NULL;
}

void Tester::ParallelAdding(void)
{
    int ret = 0;
    int z = 10000;
    Stime startime;
    Stime endtime;

    DATA_S iData;
    iData.ExpiryTime = GetNowTime();
    iData.Data = CreateRandomIdentificationNumber();
    iData.Type = z % 2 ? 0x01 : 0x00;
    sprintf(iData.Content, "AutoTest");

    while (permitToExecuteParallelTest)
    {
        Gettime_usec(&startime);
        iData.Sequence = z++;
        ret = DbManagment::GetInstance()->Add(iData);
        PRI_TIME_TMOFRT("Insert Single Record Complete! Return Code[%d]\r\n", ret);
        usleep(PAUSETIME);
    };

    return;
}

void Tester::ParallelFinding(void)
{
    int ret = 0;
    int z = 10003;
    Stime startime;
    Stime endtime;

    DATA_S iData;
    while (permitToExecuteParallelTest)
    {
        z++;
        Gettime_usec(&startime);
        memset(&iData, 0, sizeof (DATA_S));
        ret = DbManagment::GetInstance()->FindBySeqenceNumber(z, &iData);
        PRI_TIME_TMOFRT("Find Single Record Complete! Return Code[%d]\r\n", ret);
        usleep(PAUSETIME);
    };

    return;
}

void Tester::ParallelDeleting(void)
{
    int ret = 0;
    int z = 10002;
    Stime startime;
    Stime endtime;

    while (permitToExecuteParallelTest)
    {
        z++;
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->Delete(z);
        PRI_TIME_TMOFRT("Delete Single Record Complete! Return Code[%d]\r\n", ret);
        usleep(PAUSETIME);
    };

    return;
}

void Tester::ParallelBaking(void)
{
    int ret = 0;
    Stime startime;
    Stime endtime;

    while (permitToExecuteParallelTest)
    {
        Gettime_usec(&startime);
        ret = DbManagment::GetInstance()->ManulBak();
        PRI_TIME_TMOFRT("Manual Backup Complete! Return Code[%d]\r\n", ret);
        usleep(PAUSETIME);
    };

    return;
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

void Tester::Ouch(int sig)
{
    if(permitToExecuteParallelTest)
    {
        permitToExecuteParallelTest = false;
        printf("\r\n\r\n"
               "<***Receive Suspend Signal!***>\r\n"
                "<***Ready for Shut Down Module***>\r\n"
                "\r\n");
    }
    return;
}
