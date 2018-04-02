/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

const char *g_getTableStructTemplate = "PRAGMA table_info([%s]);";

const char *g_replaceRecordTemplate = "REPLACE INTO %s (%s) VALUES (%s);";

const char *g_insertRecordTemplate  = "INSERT INTO %s VALUES(%s)";

const char *g_updateRecordTemplate  = "UPDATE %s SET %s WHERE %s;";

const char *g_deleteRecordTemplate  = "DELETE FROM %s WHERE %s;";