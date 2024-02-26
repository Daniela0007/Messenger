#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <mysql/mysql.h>
#include "UserHandler.h"
#include "MessagesHandler.h"

#define PORT 2024
#define NR_CLIENTI_MAX 20

UserHandler user_handler;
MessagesHandler messages_handler;
int nr_connected_clients = 0;

//extern int errno;

void error(const char *msg, int err)
{
    perror(msg);
    exit(err);
}

int Message_Work(int);
void login_or_signup_command(char[100], int, const char *);
void list_users(char[100], int, const char *);
void send_message_to_user(char[100], int);
void see_history(char[100], int);
void getNewMessages(char[100], int);

int main()
{
    int sd, client, optval = 1, nfds, fd, Nr_Clients = 0;
    struct sockaddr_in server, from;
    fd_set readfds, actfds;
    struct timeval timev;
    socklen_t len;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        error("[SERVER] Error socket\n", errno);

    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    bzero(&server, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
        error("[SERVER] Error bind\n", errno);

    if (listen(sd, 5) == -1)
        error("[SERVER] Error listen\n", errno);

    FD_ZERO(&actfds);
    FD_SET(sd, &actfds);

    timev.tv_sec = 1;
    timev.tv_usec = 0;
    nfds = sd;

    printf("[server] Asteptam la portul %d...\n", PORT);
    fflush(stdout);

    while (1)
    {
        bcopy((char *)&actfds, (char *)&readfds, sizeof(readfds));

        if (select(nfds + 1, &readfds, NULL, NULL, &timev) < 0)
            error("[SERVER] Error select\n", errno);
        if (FD_ISSET(sd, &readfds))
        {
            len = sizeof(from);
            bzero(&from, sizeof(from));
            client = accept(sd, (struct sockaddr *)&from, &len);

            if (client < 0)
                error("[SERVER] Error client\n", errno);

            if (nfds < client)
                nfds = client;
            FD_SET(client, &actfds);

            printf("[server] S-a conectat clientul cu descriptorul %d.\n", client);
            fflush(stdout);
        }

        for (fd = 0; fd <= nfds; fd++)
        {
            if (fd != sd && FD_ISSET(fd, &readfds))
            {
                if (Message_Work(fd) == 0)
                {
                    printf("[server] S-a deconectat clientul cu descriptorul %d.\n", fd);
                    fflush(stdout);
                    close(fd);          
                    FD_CLR(fd, &actfds); 
                }
            }
        }
    }
    return 0;
}

void login_or_signup_command(char command[100], int fd, const char *type)
{
    int nr_args = -1;
    char *Username, *Password;
    char *token = strtok(command, ":");
    char Response[100];
    while (token != NULL)
    {
        switch (nr_args)
        {
        case 0:
            Username = token;
            Username[strlen(Username)] = '\0';
            nr_args++;
            break;
        case 1:
            Password = token;
            Password[strlen(Password)] = '\0';
            nr_args++;
            break;
        case 2:
            nr_args++;
            break;
        case -1:
            nr_args++;
            break;
        }
        token = strtok(NULL, ":");
    }

    printf("Username : %s\n", Username);
    fflush(stdout);
    printf("Password : %s\n", Password);
    fflush(stdout);

    // addClient(clients_fds, &nr_connected_clients, fd, Username);
    // printf("Am adaugat clientul in array de descriptori %s. Total numar clienti: %d\n", Username, nr_connected_clients); fflush(stdout);

    char *result = NULL;
    if (strcmp(type, "signup") == 0)
        result = user_handler.signup(Username, Password);
    else if (strcmp(type, "login") == 0)
        result = user_handler.login(Username, Password);
    bzero(Response, sizeof(Response));
    strcpy(Response, result);
    if (-1 == write(fd, Response, strlen(Response)))
        error("[SERVER] Error writing to client\n", errno);
}

void list_users(char command[100], int fd, const char *type)
{
    char Response[200];
    char *Result = NULL;

    Result = user_handler.getAllUsers();
    bzero(Response, sizeof(Response));
    strcpy(Response, Result);
    printf("Lista este %s\n", Response);
    if (-1 == write(fd, Response, strlen(Response)))
        error("[SERVER] Error writing to client\n", errno);
}

void send_message_to_user(char command[100], int fd)
{
    char *token = strtok(command, "/");
    char *UserCurr, *ToUser, *Message, *ReplyStatus, *AncestorMessage;
    int nr_args = -1;
    while (token != NULL)
    {
        switch (nr_args)
        {
        case 1:
            UserCurr = token;
            UserCurr[strlen(UserCurr)] = '\0';
            nr_args++;
            break;
        case 2:
            ToUser = token;
            ToUser[strlen(ToUser)] = '\0';
            nr_args++;
            break;
        case 3:
            ReplyStatus = token;
            ReplyStatus[strlen(ReplyStatus)] = '\0';
            nr_args++;
            break;
        case 4:
            AncestorMessage = token;
            AncestorMessage[strlen(AncestorMessage)] = '\0';
            nr_args++;
            break;
        case 0:
            Message = token;
            Message[strlen(Message)] = '\0';
            nr_args++;
            break;
        case -1:
            nr_args++;
            break;
        }
        token = strtok(NULL, "/");
    }

   // printf("Username : %s\n", UserCurr);
   // fflush(stdout);
   // printf("ToUser : %s\n", ToUser);
   // fflush(stdout);
   // printf("Message : %s\n", Message);
   // fflush(stdout);
    char *response = messages_handler.addMessage(UserCurr, ToUser, Message, ReplyStatus, AncestorMessage);
    if (-1 == write(fd, response, strlen(response))) {
        error("Error at writing in socket\n", errno);
    }
}

int Message_Work(int fd)
{
    char Command_From_Client[100];
    int bytes;
    char msg[100];
    char Response[100] = " ";

    bzero(Command_From_Client, 100);
    bytes = read(fd, Command_From_Client, sizeof(Command_From_Client));
    Command_From_Client[bytes] = '\0';
    if (bytes < 0)
        error("[SERVER] Error reading from client\n", errno);
    printf("Am citit comanda %s ----------\n", Command_From_Client);
    fflush(stdout);

    if (strstr(Command_From_Client, "LOGIN"))
    {
        login_or_signup_command(Command_From_Client, fd, "login");
    }
    else if (strstr(Command_From_Client, "REGISTER"))
    {
        login_or_signup_command(Command_From_Client, fd, "signup");
    }
    else if (strcmp(Command_From_Client, "LIST_ALL_USERS") == 0)
    {
        list_users(Command_From_Client, fd, "all");
    }
    else if (strstr(Command_From_Client, "GET"))
    {
        getNewMessages(Command_From_Client, fd);
    }
    else if (strstr(Command_From_Client, "SEND"))
    {
        send_message_to_user(Command_From_Client, fd);
    }
    else if (strstr(Command_From_Client, "HISTORY"))
    {
        see_history(Command_From_Client, fd);
    }
    else
    {
        if (-1 == write(fd, "Unknown command format\n", 24))
            error("[SERVER] Error at writing in socket", errno);
    }
    strcpy(Command_From_Client, "");
    return bytes;
}

void see_history(char command[100], int fd)
{
    int nr_args = -1;
    char *UserCurr, *ToUser;
    char *token = strtok(command, ":");
    char Response[1000];
    while (token != NULL)
    {
        switch (nr_args)
        {
        case 0:
            UserCurr = token;
            UserCurr[strlen(UserCurr)] = '\0';
            nr_args++;
            break;
        case 1:
            ToUser = token;
            ToUser[strlen(ToUser)] = '\0';
            nr_args++;
            break;
        case 2:
            nr_args++;
            break;
        case -1:
            nr_args++;
            break;
        }
        token = strtok(NULL, ":");
    }
    char *Result = messages_handler.getChatMessages(UserCurr, ToUser);
    bzero(Response, sizeof(Response));
    strcpy(Response, Result);
    if (-1 == write(fd, Response, strlen(Response)))
        error("SERVER: Error at writing to client\n", errno);
    printf("HISTORY MESSAGES:\n%s\n", Response);
    fflush(stdout);
}

void getNewMessages(char command[100], int fd)
{
    int nr_args = -1;
    char *UserCurr, *ToUser, *Date;
    char *token = strtok(command, "/");
    char Response[300];
    while (token != NULL)
    {
        switch (nr_args)
        {
        case 0:
            Date = token;
            Date[strlen(Date)] = '\0';
            nr_args++;
            break;
        case 1:
            UserCurr = token;
            UserCurr[strlen(UserCurr)] = '\0';
            nr_args++;
            break;
        case 2:
            ToUser = token;
            ToUser[strlen(ToUser)] = '\0';
            nr_args++;
            break;
        case 3:
            nr_args++;
            break;
        case -1:
            nr_args++;
            break;
        }
        token = strtok(NULL, "/");
    }
    char *Result = messages_handler.getNewMessages(UserCurr, ToUser, Date);
    bzero(Response, sizeof(Response));
    strcpy(Response, Result);

    if (-1 == write(fd, Response, strlen(Response)))
    {
        error("SERVER: Error at writing in client", errno);
    }
    printf("MESAJ CATRE CLIENT .....\n%s\n", Response);
    fflush(stdout);
}
