#include "CardDB/IcCard_DbManagment.h"
#include "UtilClass.h"

int LOG_LEVEL = 4; //日志级别可以从0-4，级别越低，日志权限越低
int SYSLOG_ENABLE = 0; //0x00: 表示不可用syslog  0x01: 表示syslog可用
const char* APPNAME = "DbManager"; //应用程序的名称

int main(int argc, char **argv)
{
    IcCard_DbManagment::GetInstance()->Init("n4_id8.db", "n4_id8_bak.db", 60);

    char*strMd5 = "B3F1A2E4A46D8C98E0251AD48BB46934";
    unsigned char md5Value[16] = {0};
    UtilClass::Str32ToInt128(strMd5, md5Value);

    if (IcCard_DbManagment::GetInstance()->CompareToBakDb(5326, md5Value) != 0)
        printf("Now, it's time to download\n");

    IcCard_DbManagment::GetInstance()->ManulBak();

    IcCard_DbManagment::GetInstance()->Init("n4_id8_bak.db", "n4_id8_bak_addition.db", 60);


    if (IcCard_DbManagment::GetInstance()->CompareToBakDb(5326, md5Value) != 0)
        printf("Now, it's time to download\n");

    return 0;
}
