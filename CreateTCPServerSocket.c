#include "Main.h"

int CreateTCPServerSocket(unsigned short port)
{
    int sock;                    /* socket to create */
    struct sockaddr_in ServAddr; /* Local address */

    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

#ifdef DEBUG
    printf("create server socket\n");
#endif

    /* Construct local address structure */
    memset(&ServAddr, 0, sizeof(ServAddr));       /* Zero out structure */
    ServAddr.sin_family = AF_INET;                /* Internet address family */
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    ServAddr.sin_port = htons(port);              /* Local port */

    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
        DieWithError("bind() failed");

#ifdef DEBUG
    printf("bind server socket to addr\n");
    printf("server IP %s\n", inet_ntoa(ServAddr.sin_addr));
#endif

    /* Mark the socket so it will listen for incoming connections */
    if (listen(sock, MAXPENDING) < 0)
        DieWithError("listen() failed");

#ifdef DEBUG
    printf("listen...\n");
#endif

    return sock;
}