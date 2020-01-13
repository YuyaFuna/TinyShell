#include "Main.h"

int CreateTCPClientSocket(char *servIp, unsigned short servPort)
{
    int sock;                    /* socket to create */
    struct sockaddr_in ServAddr; /* Local address */

    /* Create socket for incoming connections */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

#ifdef DEBUG
    printf("create client socket\n");
#endif

    /* Construct local address structure */
    memset(&ServAddr, 0, sizeof(ServAddr));       /* Zero out structure */
    ServAddr.sin_family = AF_INET;                /* Internet address family */
    ServAddr.sin_addr.s_addr = inet_addr(servIp); /* Any incoming interface */
    ServAddr.sin_port = htons(servPort);          /* Local port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *)&ServAddr, sizeof(ServAddr)) < 0)
        DieWithError("connect() failed");

#ifdef DEBUG
    printf("connect to the server\n");
    printf("server IP : %s\n", inet_ntoa(ServAddr.sin_addr));
    printf("my sock id : %d\n", sock);
#endif

    return sock;
}
