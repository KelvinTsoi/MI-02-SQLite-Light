/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IcCard_DbFundation.h
 * Author: CAI
 *
 * Created on 2016年5月23日, 下午7:32
 */

#ifndef ICCARD_DBFUNDATION_H
#define ICCARD_DBFUNDATION_H
#include "DbOperator.h"

class IcCard_DbFundation:public DbOperator
{
public:
    
    /**
     * 构造函数
     */
    IcCard_DbFundation();
    
    /**
     * 析构函数
     */
    virtual ~IcCard_DbFundation();
    
    /**
     * @函数功能：打开数据库
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    int CreateTable(); 
    
    /**
     * @函数功能：数据库记录插入函数；
     * @参数： CardInfo：卡数据集结构体；
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */ 
    int Add(CARD_S CardInfo);

    /**
     * 函数功能：ic_card_table数据表批量记录插入函数；
     * @参数 CardInfo：卡数据集结构体数组，size：卡数据集结构体数组大小；
     * @返回值 成功返回0，返回大于0的数表示错误代码
     */
    int Add(CARD_S CardInfo[], const int insertDataSetSize);


    /**
     * @函数功能：数据库记录删除函数；
     * @参数： serialNumber 卡序列号
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    int Delete(unsigned int serialNumber);

    /**
     * @函数功能：数据库记录更新函数；
     * @参数： CardInfo：卡数据集结构体；
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    int Update(CARD_S CardInfo);

    /**
     * @函数功能：数据库记录查询函数；
     * @参数： serialNumber：需要查询记录的序列号索引，CardInfo：卡数据集结构体；
     * @返回值： 成功返回0，返回大于0的数表示错误代码
     */
    int FindBySerialNumber(unsigned int serialNumber, CARD_S *CardInfo);
};

#endif /* ICCARD_DBFUNDATION_H */

