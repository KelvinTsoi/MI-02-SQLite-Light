/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IcCard_DbManger.cpp
 * Author: CAI
 * 
 * Created on 2016年5月25日, 下午3:54
 */

#include "IcCard_DbManagment.h"

extern "C"
{
#include "md5.h"
}

IcCard_DbManagment* IcCard_DbManagment::Instance = NULL;

IcCard_DbManagment::IcCard_DbManagment()
{
    m_pDbBase = m_pDb = new IcCard_DbFundation();
    m_pDbBakBase = m_pDbBak = new IcCard_DbFundation();
}

IcCard_DbManagment::~IcCard_DbManagment()
{
    if (m_pDb != NULL)
    {
        delete m_pDb;
        m_pDb = NULL;
        m_pDbBase = NULL;
    }
    if (m_pDbBak != NULL)
    {
        delete m_pDbBak;
        m_pDbBak = NULL;
        m_pDbBakBase = NULL;
    }

}

IcCard_DbManagment* IcCard_DbManagment::GetInstance()
{
    if (NULL == Instance)
        Instance = new IcCard_DbManagment;
    return Instance;
}

int IcCard_DbManagment::Add(CARD_S CardInfo)
{
    AUTO_LOCK(mtx);
    return (m_pDb->Add(CardInfo));
}

int IcCard_DbManagment::Add(CARD_S CardInfo[], unsigned int size)
{
    AUTO_LOCK(mtx);
    for (int i = 0; i < size; i++)
        m_pDb->Add(CardInfo[i]);
    return DATABASE_OK;
}


int IcCard_DbManagment::Delete(unsigned int serialNumber)
{
    AUTO_LOCK(mtx);
    return (m_pDb->Delete(serialNumber));
}

int IcCard_DbManagment::Delete(unsigned int serialNumber[], unsigned int size)
{
    AUTO_LOCK(mtx);
    for (int i = 0; i < size; i++)
        m_pDb->Delete(serialNumber[i]);
    return DATABASE_OK;
}

int IcCard_DbManagment::FindBySerialNumber(unsigned int serialNumber, CARD_S *CardInfo)
{
    AUTO_LOCK(mtx);
    return (m_pDb->FindBySerialNumber(serialNumber, CardInfo));
}
