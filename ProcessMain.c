#include "Main.h"

void ProcessMain(int servSock)
{
    int clntSock;
    clntSock = AcceptTCPConnection(servSock);

#ifdef DEBUG
    printf("with child process: %d\n", (unsigned int)getpid());
#endif
    HandleTCPClient(clntSock);
}
