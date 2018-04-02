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
 * File:   IcCard_DbManagment.cpp
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#include "DbManagment.h"

extern "C"
{
    #include "../md5/md5.h"
}

DbManagment* DbManagment::Instance = NULL;

DbManagment::DbManagment()
{
    m_pDbBase = m_pDb = new DbFundation();
    m_pDbBakBase = m_pDbBak = new DbFundation();
}

DbManagment::~DbManagment()
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

DbManagment* DbManagment::GetInstance()
{
    if (NULL == Instance)
    {
        Instance = new DbManagment;
    }
    return Instance;
}

int DbManagment::Add(DATA_S DataInfo)
{
    AUTO_LOCK(mtx);
    return (m_pDb->Add(DataInfo));
}

int DbManagment::Add(DATA_S DataInfo[], unsigned int size)
{
    AUTO_LOCK(mtx);
    for (unsigned int i = 0; i < size; i++)
    {
        m_pDb->Add(DataInfo[i]);
    }
    return DATABASE_OK;
}


int DbManagment::Delete(unsigned int SeqenceNumber)
{
    AUTO_LOCK(mtx);
    return (m_pDb->Delete(SeqenceNumber));
}

int DbManagment::Delete(unsigned int SeqenceNumber[], unsigned int size)
{
    AUTO_LOCK(mtx);
    for (unsigned int i = 0; i < size; i++)
    {
        m_pDb->Delete(SeqenceNumber[i]);
    }
    return DATABASE_OK;
}

int DbManagment::FindBySeqenceNumber(unsigned int SeqenceNumber, DATA_S *DataInfo)
{
    AUTO_LOCK(mtx);
    return (m_pDb->FindBySeqenceNumber(SeqenceNumber, DataInfo));
}

int DbManagment::CountBySeqenceNumber(unsigned int &amount)
{
    AUTO_LOCK(mtx);
    return (m_pDb->CountBySeqenceNumber(amount));
}
