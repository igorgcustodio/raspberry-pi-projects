#ifndef _H_DB
#define _H_DB

#define DATABASE "/home/pi/tase/bimestral/data.db"

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Select query
 * 
 * @param <char *> sql      the query
 * 
 * @return <int> 0 - success, 1 - failed
 * 
*/
int exec(char *sql);

#endif