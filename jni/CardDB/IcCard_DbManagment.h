/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IcCard_DbManagment.h
 * Author: CAI
 *
 * Created on 2016年5月25日, 下午3:54
 */

#ifndef ICCARD_DBMANAGMENT_H
#define ICCARD_DBMANAGMENT_H
#include "IcCard_DbFundation.h"
#include "IcCard_DbExtension.h"

class IcCard_DbManagment : public IcCard_DbExtension
{
public:


    virtual ~IcCard_DbManagment();

    /**
     * @函数功能：数据库记录插入函数；
     * @参数 CardInfo：卡数据集结构体数组， size:数组大小；
     * @返回值 成功返回0，返回大于0的数表示错误代码；
     */
    int Add(CARD_S CardInfo[], unsigned int size);

    /**
     * @函数功能：数据库记录插入函数；
     * @参数 CardInfo：卡数据集结构体；
     * @返回值 成功返回0，返回大于0的数表示错误代码；
     */
    int Add(CARD_S CardInfo);



    /**
     * 函数功能：数据库记录删除函数；
     * @参数 serialNumber 卡序列号
     * @返回值 成功返回0，返回大于0的数表示错误代码
     */
    int Delete(unsigned int serialNumber);

    /**
     * 函数功能：数据库记录删除函数；
     * @参数 serialNumber 卡序列号
     * @返回值 成功返回0，返回大于0的数表示错误代码
     */
    int Delete(unsigned int serialNumber[], unsigned int size);

    /**
     * 函数功能：数据库记录查询函数；
     * @参数 serialNumber：需要查询记录的序列号索引，CardInfo：卡数据集结构体；
     * @返回值 成功返回0，返回大于0的数表示错误代码
     */
    int FindBySerialNumber(unsigned int serialNumber, CARD_S *CardInfo);


    /**
     * 函数功能：单例模式中的GetInstance方法，提供对外接口；
     * @返回值 IcCard_N4_ID8_Manager类指针；
     */
    static IcCard_DbManagment* GetInstance();


private:
    /**
     * 函数功能：构造函数；
     */
    IcCard_DbManagment();

    /**
     * 单例模式中的类指针，由GetInstance分配内存空间；
     */
    static IcCard_DbManagment* Instance;

    /**
     * 放在flash里给增删改查用的数据库
     */
    IcCard_DbFundation *m_pDb;

    /**
     * 备份的数据库
     */
    IcCard_DbFundation *m_pDbBak;
};

#endif /* ICCARD_DBMANAGMENT_H */

