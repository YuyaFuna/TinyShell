#include "Main.h"

char *Login(int sock)
{
    /* login */
    bool logined = false;
    enum State state = _Insert;
    /* _Insert */
    char c[1];
    int num;
    /* _Register */
    struct User user;
    char checkpass[PASSWORD];
    char encryptedpass[PASSWORD];
    bool reregister = false;
    char cache[USERNAME];
    /* _Login */
    char *username = (char *)malloc(USERNAME);
    char *password = (char *)malloc(PASSWORD);
    char hash[PASSWORD];
    /* password */
    struct termios term;
    struct termios save;

    while (!logined)
    {
        switch (state)
        {
        case _Insert:
            printf("数字を入力してください。[0]：新規登録, [1]：ログイン\n");
            scanf("%s", c);
            if (strcmp(c, "0") == 0 || strcmp(c, "1") == 0)
            {
                num = atoi(c);
                if (send(sock, &num, sizeof(int), 0) != sizeof(int))
                    DieWithError("send() failed");
                if (recv(sock, &state, sizeof(state), 0) <= 0)
                    DieWithError("recv() failed");
            }
            else
            {
                printf("0か1の数字を入力してください。\n");
            }
            break;
        case _Register:
            printf("新規登録：ユーザー名とパスワードを入力してください。(それぞれ32文字以内)\n");
            //ユーザー名
            printf("ユーザー名：");
            if (reregister)
            {
                strcpy(user.username, cache);
                printf("%s\n", user.username);
            }
            else
                scanf("%s", user.username);
            //パスワード
            tcgetattr(0, &term);
            save = term;
            term.c_lflag &= ~ECHO;
            tcsetattr(0, TCSANOW, &term);
            printf("パスワード：");
            scanf("%s", user.password);
            printf("\n");
            printf("パスワード(確認用)：");
            scanf("%s", checkpass);
            printf("\n");
            tcsetattr(0, TCSANOW, &save);
            term = save;
            //パスワードの正しさチェック
            if (strcmp(user.password, checkpass) == 0)
            {
                printf("このアカウントを作成してもよろしいですか?[y]：はい, [others]：いいえ\n");
                char answer[1];
                scanf("%s", answer);
                if (strcmp(answer, "y") == 0)
                {
                    //パスワードの暗号化
                    strcpy(encryptedpass, hashPass(user.password));
                    if (send(sock, user.username, USERNAME, 0) != USERNAME)
                        DieWithError("send() failed");
                    if (send(sock, encryptedpass, PASSWORD, 0) != PASSWORD)
                        DieWithError("send() failed");

                    if (recv(sock, &state, sizeof(state), 0) <= 0)
                        DieWithError("recv() failed");
                    if (state == _Register)
                        printf("登録に失敗しました。アカウントを作り直してください。\n");
                }
                else
                {
                    printf("アカウントを再度作り直します。\n");
                    strcpy(cache, "");
                    reregister = false;
                }
            }
            else
            {
                printf("確認のパスワードが違います。再度入力してください。\n");
                if (!reregister)
                {
                    strcpy(cache, user.username);
                    reregister = true;
                }
            }
            break;
        case _Login:
            printf("ログイン：ユーザー名とパスワードを入力してください。(それぞれ32文字以内)\n");
            printf("ユーザー名：");
            scanf("%s", username);
            //パスワード
            tcgetattr(0, &term);
            save = term;
            term.c_lflag &= ~ECHO;
            tcsetattr(0, TCSANOW, &term);
            printf("パスワード：");
            scanf("%s", password);
            printf("\n");
            tcsetattr(0, TCSANOW, &save);
            term = save;
            strcpy(hash, hashPass(password));
            if (send(sock, username, USERNAME, 0) != USERNAME)
                DieWithError("send() failed");
            if (send(sock, hash, PASSWORD, 0) != PASSWORD)
                DieWithError("send() failed");

            if (recv(sock, &logined, sizeof(bool), 0) <= 0)
                DieWithError("recv() failed");
            if (!logined)
                printf("ユーザー名もしくはパスワードが違います。\n");
            break;
        default:
            break;
        }
    }
    printf("ログインしました\n");
    getchar();
    return username;
}
