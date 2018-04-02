/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DbInfo.h
 * Author: root
 *
 * Created on 2017年8月15日, 上午9:36
 */

#ifndef DBINFO_H
#define DBINFO_H

typedef struct TableMember
{
    int s_cid;
    char s_name[32];
    char s_type[16];
    int s_notNull;
    char s_defaultValue[128];
    int s_pk;
}TableMember;

typedef struct TableInfo
{
    int struct_number;
    TableMember struct_info[32];
}TableInfo;

#endif /* DBINFO_H */

