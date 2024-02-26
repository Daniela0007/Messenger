#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <QString>

extern char MessageToServer  [500];
extern char MessageFromServer[500];
extern char MessageForVerify [500];
extern char ThisUser          [20];
extern char ToUser            [20];
extern char AncestorMessage   [50];
extern bool ReplyStatus;
extern char DateLastMessage   [30];

#endif // TOOLBOX_H


/*#ifndef UTILS_H
#define UTILS_H
#include "qtreewidget.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
extern char* sendMessage;
extern char* receiveMessage;
extern char* externUsername;
extern char * to_user;
extern char*  lastDate;
extern QTreeWidgetItem *the_item;
extern int is_replied;
extern QString STRING;
#endif // UTILS_H
*/
