#include "Main.h"

int AcceptTCPConnection(int servSock)
{
    int clntSock;                /* Socket descriptor for client */
    struct sockaddr_in ClntAddr; /* Client address */
    unsigned int clntLen;        /* Length of client address data structure */

    /* Set the size of the in-out parameter */
    clntLen = sizeof(ClntAddr);

    /* Wait for a client to connect */
    if ((clntSock = accept(servSock, (struct sockaddr *)&ClntAddr, &clntLen)) < 0)
        DieWithError("accept() failed");

        /* clntSock is connected to a client! */
#ifdef DEBUG
    printf("Handling client %s\n", inet_ntoa(ClntAddr.sin_addr));
#endif

    return clntSock;
}
