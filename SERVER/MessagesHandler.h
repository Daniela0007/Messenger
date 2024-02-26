#include <iostream>
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

class MessagesHandler
{
    MYSQL *connection; 
public:
    MessagesHandler()
    {
        connection = mysql_init(NULL);
        if (!mysql_real_connect(connection, "localhost", "daniela", "123450", "offline_messenger", 0, NULL, 0))
        {
            fprintf(stderr, "ERROR: %s\n", mysql_error(connection));
            exit(1);
        }
    }
    char *addMessage(char *Username, char *Receiver, char *Message, char *ReplyStatus, char *AncestorMessage)
    {
        if (connection)
        {
            char command[200];
            sprintf(command, "INSERT INTO Messages (message, id_writer, id_receiver, timestamp, reply_status, ancestor_message) VALUES('%s', '%s', '%s', NOW(), '%s', '%s')", Message, Username, Receiver, ReplyStatus, AncestorMessage);
            mysql_query(connection, command);
            return (char *)"Message added successfully to database";
        }
        return NULL;
    }
    char *getNewMessages(char *User1, char *User2, char *Date)
    {
        if (connection)
        {
            char command[300], date[50];
            sprintf(command, "SELECT * FROM Messages where timestamp>'%s' AND ((id_writer='%s' AND id_receiver='%s') OR (id_writer='%s' AND id_receiver='%s')) order by timestamp", Date, User1, User2, User2, User1);
            mysql_query(connection, command);
            printf("EXECUTED COMMAND SQL: \n%s\n", command);
            MYSQL_RES *res = mysql_store_result(connection);
            char ChatMessages[1000]{};
            MYSQL_ROW row;
            if (!res)
            {
                printf("Nu AVem niciun mesaj in inbox..... \n");
                fflush(stdout);
                strcat(ChatMessages, "/-/Inbox currently empty\n");
                char *ans = ChatMessages;
                return ans;
            }
            while (row = mysql_fetch_row(res))
            {
                if ((char *)row[6] == NULL)
                {
                    strcat(ChatMessages, "/-/");
                }
                else
                {
                    strcat(ChatMessages, "/");
                    strcat(ChatMessages, row[6]);
                    strcat(ChatMessages, "/ ");
                }
                strcat(ChatMessages, row[2]);
                strcat(ChatMessages, ": ");

                strcat(ChatMessages, row[1]);
                strcat(ChatMessages, "\n");
                strcpy(date, row[4]);
            }
            if (ChatMessages[0])
            { 
                strcat(ChatMessages, date);
            }
            else
            {
                strcat(ChatMessages, "/-/Inbox currently empty\n");
            }
            char *ans = ChatMessages;
            return ans;
        }
        return NULL;
    }
    char *getChatMessages(char *User1, char *User2)
    {
        if (connection)
        {
            char command[200];
            char date[50];
            sprintf(command, "SELECT * FROM Messages WHERE (id_writer='%s' and id_receiver='%s') or (id_writer='%s' and id_receiver='%s') order by timestamp", User1, User2, User2, User1);
            mysql_query(connection, command);
            MYSQL_RES *res = mysql_store_result(connection);
            char ChatMessages[1000]{};
            MYSQL_ROW row;
            while (row = mysql_fetch_row(res))
            {
                if ((char *)row[6] == NULL)
                {
                    strcat(ChatMessages, "/-/");
                }
                else
                {
                    strcat(ChatMessages, "/");
                    strcat(ChatMessages, row[6]);
                    strcat(ChatMessages, "/ ");
                }

                    strcat(ChatMessages, row[2]);
                    strcat(ChatMessages, ": ");
                strcat(ChatMessages, row[1]);
                strcat(ChatMessages, "\n");
                strcpy(date, row[4]);
            }

            if (!ChatMessages[0])
            {
                strcpy(ChatMessages, "No messages in the conversation");
            }
            else
            {
                strcat(ChatMessages, date);
            }
            char *ans = ChatMessages;
            return ans;
        } 
        return (char *)"Unexecuted query\n";
    }
};