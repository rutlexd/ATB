#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

int main() {
    sqlite3 *db;
    char *errMsg = nullptr;
    string userEnter;

    // Open or create a database
    int rc = sqlite3_open("Database.db", &db);


   
    sqlite3_close(db);

    return 0;
}
