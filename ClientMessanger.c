#include "Main.h"

bool ClientMessanger(int sock, char *clntname)
{
    char *String = (char *)malloc(BUFSIZE); /* String to send to echo server */
    char receiveBuffer[BUFSIZE];            /* Buffer for server string */
    int bytesRcvd, totalBytesRcvd;          /* Bytes read in single recv() and total bytes read */
    int length;

    memset(String, 0, sizeof(char) * BUFSIZE);
    memset(receiveBuffer, 0, sizeof(receiveBuffer));

    printf("%s@> ", clntname);
    /* send shell commands*/
    if (fgets(String, BUFSIZE, stdin) == NULL)
    {
        system("clear");
        free(String);
        return true;
    }
    length = strlen(String);

    /* 改行をスルー */
    if (String[0] == '\n')
    {
        free(String);
        return true;
    }
    else
        String[length - 1] = '\0';

    if (strcmp(String, "exit") == 0)
    {
        free(String);
        return false;
    }
    if (strcmp(String, "clear") == 0)
    {
        free(String);
        system("clear");
        return true;
    }

    /* Send the commands to the server */
    if (send(sock, String, length, 0) != length)
        DieWithError("send() sent a different number of bytes than expected");

    /* Receive the same string back from the server */
    totalBytesRcvd = 0;
    /* Setup to print the received string */
    while (totalBytesRcvd < 1)
    {
        if (strcmp(String, "memo") == 0)
            Memo(sock);

        memset(receiveBuffer, 0, sizeof(receiveBuffer));
        /* Receive up to the buffer size */
        bytesRcvd = recv(sock, receiveBuffer, BUFSIZE, 0);
        if (bytesRcvd < 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;                                              /* Keep tally of total bytes */
        receiveBuffer[bytesRcvd] = '\0';                                          /* Terminate the string! */
        if (strcmp(receiveBuffer, "jj") == 0 || strcmp(receiveBuffer, "\0") == 0) /* for some commands ex) cat,mkdir,rmdir etc...*/
        {
            free(String);
            return true;
        }
        printf("%s", receiveBuffer); /* Print the buffer */
    }
    printf("\n");
    free(String);
    return true;
}