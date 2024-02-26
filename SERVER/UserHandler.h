#include <iostream>
#include <stdio.h>
#include <string>
#include <mysql/mysql.h>

class UserHandler {
    MYSQL *connection; 
    bool verifyExistentUser(char* Username) {
        if (connection) {
            std :: string command = "SELECT username FROM User WHERE username = \'";
            command.append(Username);
            command.append("\'");
            mysql_query(connection, command.c_str());
            MYSQL_RES *result = mysql_store_result(connection);
            MYSQL_ROW row;

            while (row = mysql_fetch_row(result)) {
                if (strcmp(row[0], Username) == 0) {
                    return true;
                }
            }
        }
        return false; 
    }
    void insertUser(char* Username, char* Password) {
        if(connection) {
            std::string command = "INSERT INTO User (username, password, status) VALUES(\'";
            command.append(Username);
            command.append("\',\'");
            command.append(Password);
            command.append("\',\'0\')");
            mysql_query(connection, command.c_str());
        }
    }
    bool setUserStatus (char* Username) {
        if (connection) {
            std::string command = "UPDATE User SET status=1 WHERE username=\'";
            command.append(Username);
            command.append("\'");
            mysql_query(connection, command.c_str());
            return true;
        }
        return false;
    }

    public:
    
    UserHandler() {
        connection = mysql_init(NULL);
        if (!mysql_real_connect(connection, "localhost", "daniela", "123450", "offline_messenger", 0, NULL, 0)) {
            fprintf(stderr, " ERROR: %s\n", mysql_error(connection));
            exit(1);
        }
    }
    char* signup(char* Username, char* Password) {
        if (verifyExistentUser(Username) == true) {
            return (char*)"User already exists\n";
        }
        insertUser(Username, Password);
        return (char*)"Successfully registered\n";
    }
    char* login(char* Username, char* Password) {
        if (connection) {
            std :: string command = "SELECT COUNT(*) FROM User WHERE username = \'";
            command.append(Username);
            command.append("\' AND password= \'");
            command.append(Password);
            command.append("\'");
            mysql_query(connection, command.c_str());
            MYSQL_RES *result = mysql_store_result(connection);
            MYSQL_ROW row;

            while (row = mysql_fetch_row(result)) {
                if (strcmp(row[0], "1") == 0) {
                    if (setUserStatus(Username) == true)
                        return (char*)"Successfully logged in\n";
                    else
                        return (char*)"Unsuccessfull login\n";
                }
            }
        }
        return (char*)"Unsuccessfull login\n";
    }
    char* getAllUsers() {
        std::string command;
        command = "SELECT username FROM User";
        mysql_query(connection, command.c_str());
        MYSQL_RES *result = mysql_store_result(connection);
        MYSQL_ROW row;

        char Users[200]{};
        while (row = mysql_fetch_row(result)) {
            strcat(Users, row[0]);
            strcat(Users, "\n");
        }
        char* r = Users;
        return r;
    }
};