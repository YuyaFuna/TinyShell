#include "Main.h"

bool HandleCmd(int clntSocket, char *Buffer, int recvMsgSize, char *clntname)
{
    //空白区切りで文字列を分割
    int length = strlen(Buffer);
    char *ptr;
    ptr = strtok(Buffer, " ");
    //送信用の文字列
    char Str[BUFSIZE];
    memset(Str, 0, sizeof(Str));

    /* echo コマンド */
    if (strcmp(ptr, "echo") == 0)
    {
        while (ptr != NULL)
        {
            ptr = strtok(NULL, " ");
            // ptrがNULLの場合エラーが発生するので対処
            if (ptr != NULL)
                sprintf(Str + strlen(Str), "%s ", ptr);
        }
        if (send(clntSocket, Str, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");
    }
    /* date コマンド */
    else if (strcmp(ptr, "date") == 0)
    {
        time_t t = time(NULL);
        struct tm *localTime = localtime(&t);
        strftime(Str, sizeof(Str), "%Y/%m/%d %H:%M:%S %A", localTime);
        if (send(clntSocket, Str, sizeof(Str), 0) != sizeof(Str))
            DieWithError("send() failed");
    }
    /* pwd コマンド */
    else if (strcmp(ptr, "pwd") == 0)
    {
        if (getcwd(Str, sizeof(Str) - 1) == NULL)
            perror(Str);
        if (send(clntSocket, Str, sizeof(Str), 0) != sizeof(Str))
            DieWithError("send() failed");
    }
    /* mkdir コマンド */
    else if (strcmp(ptr, "mkdir") == 0)
    {
        while (ptr != NULL)
        {
            ptr = strtok(NULL, " ");
            // ptrがNULLの場合エラーが発生するので対処
            if (ptr != NULL)
            {
                if (mkdir(ptr, 0755) != 0)
                {
                    if (send(clntSocket, "ディレクトリを作成できませんでした。", BUFSIZE, 0) != BUFSIZE)
                        DieWithError("send() failed");
                    return true;
                }
            }
        }
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
    }
    /* rmdir コマンド */
    else if (strcmp(ptr, "rmdir") == 0)
    {
        while (ptr != NULL)
        {
            ptr = strtok(NULL, " ");
            // ptrがNULLの場合エラーが発生するので対処
            if (ptr != NULL)
            {
                if (rmdir(ptr) != 0)
                {
                    if (send(clntSocket, "ディレクトリを削除できませんでした。", BUFSIZE, 0) != BUFSIZE)
                        DieWithError("send() failed");
                    return true;
                }
            }
        }
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
    }
    /* touch コマンド */
    else if (strcmp(ptr, "touch") == 0)
    {
        while (ptr != NULL)
        {
            ptr = strtok(NULL, " ");
            // ptrがNULLの場合エラーが発生するので対処
            if (ptr != NULL)
            {
                FILE *f;
                f = fopen(ptr, "w");
                fclose(f);
            }
        }
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
    }
    /* rm コマンド */
    else if (strcmp(ptr, "rm") == 0)
    {
        while (ptr != NULL)
        {
            ptr = strtok(NULL, " ");
            // ptrがNULLの場合エラーが発生するので対処
            if (ptr != NULL)
            {
                if (remove(ptr) != 0)
                {
                    if (send(clntSocket, "ファイルを削除できませんでした。", BUFSIZE, 0) != BUFSIZE)
                        DieWithError("send() failed");
                    return true;
                }
            }
        }
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
    }
    /* cd コマンド */
    else if (strcmp(ptr, "cd") == 0)
    {
        // ptrがNULLの場合エラーが発生するので対処
        if (strcmp(Buffer, "cd") == 0 && length == 2)
            sprintf(ptr, "/home/yuya/myhome/%s", clntname);
        else
            ptr = strtok(NULL, " ");

        if (ptr != NULL)
        {
            if (chdir(ptr) != 0)
            {
                if (send(clntSocket, "作業ディレクトリを移動することができませんでした。", BUFSIZE, 0) != BUFSIZE)
                    DieWithError("send() failed");
                return true;
            }
        }
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
    }
    /* ls コマンド */
    else if (strcmp(ptr, "ls") == 0)
    {
        if (strcmp(Buffer, "ls") == 0 && length == 2)
        {
            char cwd[BUFSIZE];
            getcwd(cwd, sizeof(cwd) - 1);
            strcpy(ptr, cwd);
        }
        else
            ptr = strtok(NULL, " ");

        if (ptr != NULL)
        {
            DIR *dir;
            struct dirent *dent;
            dir = opendir(ptr);
            if (dir == NULL)
            {
                if (send(clntSocket, "指定したディレクトリが存在しません。", BUFSIZE, 0) != BUFSIZE)
                    DieWithError("send() failed");
                return true;
            }
            while ((dent = readdir(dir)) != NULL)
            {
                sprintf(Str + strlen(Str), "%s\n", dent->d_name);
            }
            Str[strlen(Str) - 1] = '\0';
            closedir(dir);
        }
        if (send(clntSocket, Str, strlen(Str), 0) != strlen(Str))
            DieWithError("send() failed");
    }
    /* mv コマンド */
    else if (strcmp(ptr, "mv") == 0)
    {
        int i = 0;
        char *temp[2] = {};
        while (i < 2)
        {
            ptr = strtok(NULL, " ");
            // ptrがNULLの場合エラーが発生するので対処
            if (ptr != NULL)
            {
                temp[i++] = ptr;
            }
        }

        if (rename(temp[0], temp[1]) < 0)
        {
            if (send(clntSocket, "ファイルを移動できませんでした。", BUFSIZE, 0) != BUFSIZE)
                DieWithError("send() failed");
            return true;
        }
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
    }
    /* cp コマンド */
    else if (strcmp(ptr, "cp") == 0)
    {
        int i = 0;
        char *temp[2] = {};
        char c;
        while (i < 2)
        {
            ptr = strtok(NULL, " ");
            // ptrがNULLの場合エラーが発生するので対処
            if (ptr != NULL)
            {
                temp[i++] = ptr;
            }
        }

        int src, dst;
        src = open(temp[0], O_RDONLY);
        dst = open(temp[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

        while (read(src, &c, 1) > 0)
            write(dst, &c, 1);

        close(src);
        close(dst);
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
    }
    /* cat コマンド */
    else if (strcmp(ptr, "cat") == 0)
    {
        FILE *f;
        char c;
        ptr = strtok(NULL, " ");
        if (ptr != NULL)
        {
            f = fopen(ptr, "r");
            if (!f)
            {
                if (send(clntSocket, "ファイルがありません。", sizeof(Str), 0) != sizeof(Str))
                    DieWithError("send() failed");
                return true;
            }

            while ((c = fgetc(f)) != EOF)
                sprintf(Str + strlen(Str), "%c", c);
            Str[strlen(Str)] = '\0';

            fclose(f);
        }
        if (send(clntSocket, Str, sizeof(Str), 0) != sizeof(Str))
            DieWithError("send() failed");
    }
    /* memo コマンド */
    else if (strcmp(ptr, "memo") == 0)
    {
        /* get filename and make the file */
        if ((recv(clntSocket, Str, BUFSIZE, 0)) <= 0)
            return true;
#ifdef DEBUG
        printf("\tfilename:%s\n", Str);
#endif
        FILE *f;
        f = fopen(Str, "w");

        /* get the context of file */
        memset(Str, 0, sizeof(Str));
        if ((recv(clntSocket, Str, BUFSIZE, 0)) <= 0)
            return true;
#ifdef DEBUG
        printf("\tfile context\n\t%s\n\t[EOF]\n", Str);
#endif
        fprintf(f, "%s", Str);
        fclose(f);
        if (send(clntSocket, "jj", 2, 0) != 2)
            DieWithError("send() failed");
        return true;
    }
    /* help コマンド */
    /* exitとclearとmemoコマンドはクライアント側で処理 */
    else if (strcmp(ptr, "help") == 0)
    {
        sprintf(Str + strlen(Str), "TinyShell - (x86-64-linux)\n");
        sprintf(Str + strlen(Str), "These shell commands are defined internally. Type 'help' to see this list.\n\n");
        sprintf(Str + strlen(Str), "List of Commands\n");
        sprintf(Str + strlen(Str), "help : show this help\n");
        sprintf(Str + strlen(Str), "exit : exit TinyShell\n");
        sprintf(Str + strlen(Str), "echo : echo following input string\n");
        sprintf(Str + strlen(Str), "date : show date and time now\n");
        sprintf(Str + strlen(Str), "pwd  : show current working directory\n");
        sprintf(Str + strlen(Str), "mkdir: make directory in cwd\n");
        sprintf(Str + strlen(Str), "rmdir: remove directory in cwd\n");
        sprintf(Str + strlen(Str), "cd   : change directory\n");
        sprintf(Str + strlen(Str), "ls   : list of all entry in directory\n");
        sprintf(Str + strlen(Str), "touch: make file\n");
        sprintf(Str + strlen(Str), "rm   : remove file\n");
        sprintf(Str + strlen(Str), "mv   : move file\n");
        sprintf(Str + strlen(Str), "cp   : copy file\n");
        sprintf(Str + strlen(Str), "cat  : show the contents of file\n");
        sprintf(Str + strlen(Str), "clear: clear shell window\n");
        sprintf(Str + strlen(Str), "memo : simple memo app");
        Str[strlen(Str)] = '\0';
        if (send(clntSocket, Str, strlen(Str), 0) != strlen(Str))
            DieWithError("send() failed");
    }
    /* それ以外 */
    else
    {
        sprintf(Str, "%s:コマンドが見つかりません。", Buffer);
        if (send(clntSocket, Str, strlen(Str), 0) != strlen(Str))
            DieWithError("send() failed");
    }
    return true;
}
