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
 * Created on 2016/7/29, 9:58pm
 */

#include "CardDB/IcCard_DbManagment.h"

int main(int argc, char** argv)
{
	IcCard_DbManagment::GetInstance()->Init("CardDateBase.db","CardDateBase_bak.db", 60);

	return 0;
}
