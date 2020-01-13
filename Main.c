#include "Main.h"

int main(int argc, char *argv[])
{
    system("clear");

    /* シグナルハンドラの設定 */
    SetSignal(SIGINT);
    /* サーバの生成 */
    int servSock;            /* サーバのファイルディスクリプタ */
    unsigned short ServPort; /* ポート */
    pid_t processID;             /* Process ID */
    unsigned int processLimit;   /* Number of child processes to create */
    unsigned int processCt;      /* Process counter */
    /* クライアントの生成 */
    int sock;     /* Socket descriptor */
    char *servIP; /* Server IP address (dotted quad) */
    char clntname[USERNAME];

    if (argc == 1)
    {
        fprintf(stderr, "使用例(Server):  %s server <FORK LIMIT> <SERVER PORT>(指定しない場合8000) \n", argv[0]);
        fprintf(stderr, "使用例(Client):  %s client <SERVER IP> <SERVER PORT>(指定しない場合8000) \n", argv[0]);
        exit(1);
    }
    if (strcmp(argv[1], "server") == 0)
    {

        if (argc < 3 || argc > 4) /* コマンドライン引数のチェック */
        {
            fprintf(stderr, "使用例:  %s server <FORK LIMIT> <SERVER PORT>(指定しない場合8000) \n", argv[0]);
            exit(1);
        }
        processLimit = atoi(argv[2]);
        if (argc == 4) /* ポートの指定 */
            ServPort = atoi(argv[3]);
        else
            ServPort = PORT;

        servSock = CreateTCPServerSocket(ServPort);

        for (processCt = 0; processCt < processLimit; processCt++)
            /* Fork child process and report any errors */
            if ((processID = fork()) < 0)
                DieWithError("fork() failed");
            else if (processID == 0) /* If this is the child process */
                ProcessMain(servSock);

        exit(0); /* The children will carry on */
    }
    else if (strcmp(argv[1], "client") == 0)
    {
        if (argc < 3 || argc > 4) /* コマンドライン引数のチェック */
        {
            fprintf(stderr, "使用例:  %s client <SERVER IP> <SERVER PORT>(指定しない場合8000) \n", argv[0]);
            exit(1);
        }
        servIP = argv[2]; /* server IP address (dotted quad) */

        if (argc == 4)
            ServPort = atoi(argv[3]);
        else
            ServPort = PORT;

        sock = CreateTCPClientSocket(servIP, ServPort);

        strcpy(clntname, Login(sock));

        bool sock_open = true;
        while (sock_open)
        {
            sock_open = ClientMessanger(sock, clntname);
        }
        close(sock);
        system("clear");
        exit(0);
    }
    else
    {
        fprintf(stderr, "1つ目の引数に\"server\"か\"client\"を指定してください。\n");
        exit(1);
    }
}
