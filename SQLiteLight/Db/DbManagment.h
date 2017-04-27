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
     * @parameter: CardInfo Card info array;
     * @parameter: insertDataSetSize array size;
     * @return: success return 0, others signify function error code;
     */
    int Add(CARD_S CardInfo[], unsigned int size);

    /**
     * @function: insert record into table;
     * @parameter: CardInfo card info;
     * @return: success return 0, others signify function error code;
     */
    int Add(CARD_S CardInfo);

    /**
     * @function: delete record from table;
     * @parameter: serialNumber card serial number;
     * @return: success return 0, others signify function error code;
     */
    int Delete(unsigned int serialNumber);

    /**
     * @function: delete batch record from table;
     * @parameter: serialNumber serial number array;
     * @parameter: size array size;
     * @return: success return 0, others signify function error code;
     */
    int Delete(unsigned int serialNumber[], unsigned int size);

    /**
     * @function: find record from table;
     * @parameter: serialNumber card serial number;
     * @parameter: CardInfo card info;
     * @return: success return 0, others signify function error code;
     */
    int FindBySerialNumber(unsigned int serialNumber, CARD_S *CardInfo);

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

