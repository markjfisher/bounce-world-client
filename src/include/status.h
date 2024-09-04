#ifndef BWC_STATUS_H
#define BWC_STATUS_H

#define CLIENT_CHANGE     1
#define OBJECT_CHANGE     2
#define FROZEN_TOGGLE     4
#define CLIENT_CMD        8

#define COLLISION        32

void handle_app_status();


#endif // BWC_STATUS_H