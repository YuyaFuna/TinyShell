#include "Main.h"

void HandleTCPClient(int clntSocket)
{
    char Buffer[BUFSIZE]; /* Buffer for echo string */
    int recvMsgSize;      /* Size of received message */
    char clntname[USERNAME];

#ifdef DEBUG
    printf("Handle TCP Client clntSocket : %d\n", clntSocket);
#endif

    strcpy(clntname, HandleClientLogin(clntSocket));
    if (strcmp(clntname, "") == 0)
    {
        printf("ログインに失敗しました。\n");
        close(clntSocket); /* Close client socket */
        return;
    }

    /* main */
    while (1)
    {
        memset(Buffer, 0, sizeof(char) * BUFSIZE);
        /* Receive message from client */
        if ((recvMsgSize = recv(clntSocket, Buffer, BUFSIZE, 0)) <= 0)
            break;

        char datetime[32];
        time_t t = time(NULL);
        struct tm *localTime = localtime(&t);
        strftime(datetime, sizeof(datetime), "%Y/%m/%d %H:%M:%S %A", localTime);
        printf("[%s @ %s]: >> %s\n", clntname, datetime, Buffer);
        /* read cmd */
        if (!HandleCmd(clntSocket, Buffer, recvMsgSize, clntname))
            break;
    }

    printf("%sさんがログアウトしました\n", clntname);
    close(clntSocket); /* Close client socket */
}
