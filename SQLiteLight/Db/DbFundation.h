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
 * File:   DbFundation.h
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#ifndef _DBFUNDATION_H
#define _DBFUNDATION_H
#include "DbOperator.h"

class DbFundation:public DbOperator
{
public:

    /**
     * Constructor;
     */
    DbFundation();

    /**
     * Destructor;
     */
    virtual ~DbFundation();

    /**
     * @function: create date table;
     * @return: success return 0, others signify function error code;
     */
    int CreateTable();

    /**
     * @function: insert record into table;
     * @parameter: CardInfo card info;
     * @return: success return 0, others signify function error code;
     */
    int Add(DATA_S DataInfo);

    /**
     * @function: insert batch records into table;
     * @parameter: CardInfo Card info array;
     * @parameter: insertDataSetSize array size;
     * @return: success return 0, others signify function error code;
     */
    int Add(DATA_S DataInfo[], const int insertDataSetSize);

    /**
     * @function: delete record from table;
     * @parameter: serialNumber card serial number;
     * @return: success return 0, others signify function error code;
     */
    int Delete(unsigned int SeqenceNumber);

    /**
     * @function: update record from table;
     * @parameter: CardInfo card info;
     * @return: success return 0, others signify function error code;
     */
    int Update(DATA_S DataInfo);

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
};

#endif /* _DBFUNDATION_H */

