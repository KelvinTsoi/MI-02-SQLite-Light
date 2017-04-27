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
    int Add(CARD_S CardInfo);

    /**
     * @function: insert batch records into table;
     * @parameter: CardInfo Card info array;
     * @parameter: insertDataSetSize array size;
     * @return: success return 0, others signify function error code;
     */
    int Add(CARD_S CardInfo[], const int insertDataSetSize);

    /**
     * @function: delete record from table;
     * @parameter: serialNumber card serial number;
     * @return: success return 0, others signify function error code;
     */
    int Delete(unsigned int serialNumber);

    /**
     * @function: update record from table;
     * @parameter: CardInfo card info;
     * @return: success return 0, others signify function error code;
     */
    int Update(CARD_S CardInfo);

    /**
     * @function: find record from table;
     * @parameter: serialNumber card serial number;
	 * @parameter: CardInfo card info;
     * @return: success return 0, others signify function error code;
     */
    int FindBySerialNumber(unsigned int serialNumber, CARD_S *CardInfo);

    /**
     * @function: counting records of table;
     * @parameter: amount sum of record in database;
     * @return: success return 0, others signify function error code;
     */
    int CountBySerialNumber(unsigned int &amount);
};

#endif /* _DBFUNDATION_H */

