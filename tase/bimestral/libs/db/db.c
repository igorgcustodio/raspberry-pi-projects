#include "db.h"

sqlite3 *db;
char *errorMessage = 0;

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

/**
 * Open a database connection
 * 
 * @param <char*>       the db file
 * @param <sqlite3*>    the connection
 * 
 * @return <int> 0 - connection successfullly, 1 - connection failed
*/
int open()
{
    int rc;

    rc = sqlite3_open(DATABASE, &db);

    if (rc)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        printf("Opened database successfully\n");
    }

    return rc;
}

/**
 * Close the connection
 * 
 * @param <sqlite3*>    the connection
 * 
*/
void close()
{
    sqlite3_close(db);
}

int exec(char *sql)
{
    open();

    int rc = sqlite3_exec(db, sql, callback, 0, &errorMessage);

    if (rc != SQLITE_OK)
    {
        printf("SQL error: %s\n", errorMessage);
        sqlite3_free(errorMessage);
    }
    else
    {
        printf("\nOperation done successfully!\n");
    }

    close();
    return rc;
}