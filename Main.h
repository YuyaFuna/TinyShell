#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>     /* for atoi() and exit() and malloc() */
#include <ctype.h>      /* for isdigit() */
#include <signal.h>     /* for SIGINT (Ctrl + c) */
#include <stdbool.h>    /* for true or false */
#include <string.h>     /* for memset() */
#include <termios.h>    /* for input password */
#include <unistd.h>     /* for close() and getcwd() */
#include <fcntl.h>      /* for open()/close() and read()/write() */
#include <time.h>       /* for date cmd */
#include <sys/stat.h>   /* for mkdir etc */
#include <dirent.h>     /* for ls etc */
#include <crypt.h>      /* for hashing password */
#include <errno.h>      /* for check some error */

/* for All */
#define PORT 8000
#define BUFSIZE 2048
#define MAXPENDING 5 /* Maximum listen */
#define USERNAME 32
#define PASSWORD 256

/* for Debug */
#define DEBUG

enum State
{
    _Insert,
    _Register,
    _Login
};

struct User
{
    char username[USERNAME];
    char password[PASSWORD];
};

/* for All */
void DieWithError(char *errorMessage);       /* Error handling function */
void SetSignal(int signaltype);              /* set signal */
void InterruptSignalHandler(int signaltype); /* signal handler */

/* for server */
int CreateTCPServerSocket(unsigned short port); /* Create TCP server socket */
void ProcessMain(int servSock);                 /* Create Process */
int AcceptTCPConnection(int servSock);          /* Accept TCP connection request */
void HandleTCPClient(int clntSocket);           /* TCP client handling function */
char *HandleClientLogin(int clntSocket);

/* for client */
int CreateTCPClientSocket(char *ip, unsigned short port); /* Create TCP client socket */
char *Login(int sock);
bool ClientMessanger(int sock, char *clntname);
bool HandleCmd(int clntSocket, char *Buffer, int recvMsgSize, char *clntname);
void Memo(int sock);
char *hashPass(char plane[PASSWORD]);
