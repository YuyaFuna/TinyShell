#include "Main.h"

void SetSignal(int signaltype)
{
    if (signal(signaltype, InterruptSignalHandler) == SIG_ERR)
    {
        /* シグナル設定エラー  */
        DieWithError("シグナルの設定が出来ませんでした。終了します\n");
    }

    return;
}

void InterruptSignalHandler(int signaltype)
{
    char c[1];
    printf("\nプロセスを終了しますか?：[y]：はい, [others]：いいえ");
    scanf("%s", c);
    if (strcmp(c, "y") == 0)
        exit(1);

    SetSignal(signaltype);

    return;
}