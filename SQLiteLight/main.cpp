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
 * File:   Main.cpp
 * Author: CAI
 * Created on 2017/4/27, 10:00pm
 */

#include "Db/DbManagment.h"

int main(int argc, char** argv)
{
    char tmpPath[256] = {0x00};
    char bakPath[256] = {0x00};

    sprintf(tmpPath, "DataBase.db");
    sprintf(bakPath, "DateBase_bak.db");

	DbManagment::GetInstance()->Init(tmpPath, bakPath, 60);

	while(1);

	return 0;
}
