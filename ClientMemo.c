#include "Main.h"

void Memo(int sock)
{
    int c;
    /* get filename and make the file */
    char filename[BUFSIZE];
    char context[BUFSIZE];
    memset(context, 0, sizeof(context));
    printf("ファイル名を入力してください。：");
    fgets(filename, BUFSIZE, stdin);
    filename[strlen(filename) - 1] = '\0';
    if (send(sock, filename, strlen(filename), 0) != strlen(filename))
        DieWithError("send() sent a different number of bytes than expected");

    /* input the context of the file */
    system("clear");
    printf("編集を終えるには[Esc]キーを押してください...\n%s\n", filename);
    while ((c = fgetc(stdin)) != 27)
        sprintf(context + strlen(context), "%c", c);
    context[strlen(context)] = '\0';
    getchar();

    if (send(sock, context, strlen(context), 0) != strlen(context))
        DieWithError("send() failed");
    system("clear");
}
