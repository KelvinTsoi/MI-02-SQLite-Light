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
 * File:   DbManagment.h
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#ifndef _DBMANAGMENT_H
#define _DBMANAGMENT_H
#include "DbFundation.h"
#include "DbExtension.h"

class DbManagment : public DbExtension
{
public:

	/**
	 * Destructor;
	 */
    virtual ~DbManagment();

    /**
     * @function: insert batch records into table;
     * @parameter: Data info array;
     * @parameter: insertDataSetSize array size;
     * @return: success return 0, others signify function error code;
     */
    int Add(DATA_S DataInfo[], unsigned int size);

    /**
     * @function: insert record into table;
     * @parameter: Data info;
     * @return: success return 0, others signify function error code;
     */
    int Add(DATA_S DataInfo);

    /**
     * @function: delete record from table;
     * @parameter: Seqence Number;
     * @return: success return 0, others signify function error code;
     */
    int Delete(unsigned int SeqenceNumber);

    /**
     * @function: delete batch record from table;
     * @parameter: Seqence Number array;
     * @parameter: size array size;
     * @return: success return 0, others signify function error code;
     */
    int Delete(unsigned int SeqenceNumber[], unsigned int size);

    /**
     * @function: find record from table;
     * @parameter: Seqence Number;
     * @parameter: Data info;
     * @return: success return 0, others signify function error code;
     */
    int FindBySeqenceNumber(unsigned int SeqenceNumber, DATA_S *DataInfo);

    /**
     * @function: counting records of table;
     * @parameter: amount sum of record in database;
     * @return: success return 0, others signify function error code;
     */
    int CountBySeqenceNumber(unsigned int &amount);

    /**
     * @function: GetInstance method in singleton pattern;
     * @return: pointer of class;
     */
    static DbManagment* GetInstance();

private:

    /**
     * Constructor;
     */
    DbManagment();

    /**
     * pointer of class[singleton pattern], allocate memory space from GetInstance method;
     */
    static DbManagment* Instance;

    /**
     * memory data base;
     */
    DbFundation *m_pDb;

    /**
     * back data base;
     */
    DbFundation *m_pDbBak;
};

#endif /* _DBMANAGMENT_H */

