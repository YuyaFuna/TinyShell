#include "Main.h"

bool Check(char username[USERNAME], char password[PASSWORD])
{
    FILE *fin;
    const char *home = getenv("HOME");
    char *path = strdup(home);
    strcat(path, "/.account.txt");
    if ((fin = fopen(path, "r")) == NULL)
    {
        printf("check:ファイルが開けません\n");
        fclose(fin);
    }

    char user[USERNAME];
    char pass[PASSWORD];
    while (fscanf(fin, "%s\t%s\n", user, pass) != EOF)
    {

        if ((strcmp(username, user) == 0) && (strcmp(password, pass) == 0))
        {
            fclose(fin);
            return true;
        }
    }

    fclose(fin);
    return false;
}

bool Register(struct User user)
{
    FILE *fout;
    const char *home = getenv("HOME");
    char *path = strdup(home);
    strcat(path, "/.account.txt");
    chmod(path, 0666);

#ifdef DEBUG
    printf("%s\n", path);
#endif

    if ((fout = fopen(path, "a")) == NULL)
    {
        printf("register:ファイルが開けません\n");
        fclose(fout);
        return false;
    }
    fprintf(fout, "%s\t%s\n", user.username, user.password);
    chmod(path, 0644);

    char *myhome = strdup(home);
    strcat(myhome, "/myhome/");
    strcat(myhome, user.username);

    if (mkdir(myhome, 0775) != 0)
    {
        printf("errno: %d\n", errno);
        fclose(fout);
        return false;
    }

    fclose(fout);
    return true;
}

char *HandleClientLogin(int clntSocket)
{
    //login
    bool logined = false;
    enum State state = _Insert;
    /* _Insert */
    int num;
    /* _Register */
    struct User user;
    /* _Login */
    char *username = (char *)malloc(sizeof(char) * USERNAME);
    char *password = (char *)malloc(sizeof(char) * PASSWORD);
    const char *home = getenv("HOME");
    char *myhome = strdup(home);
    while (!logined)
    {
        switch (state)
        {
        case _Insert:
            if ((recv(clntSocket, &num, sizeof(int), 0)) <= 0)
                return "";
            if (num == 0)
                state = _Register;
            else if (num == 1)
                state = _Login;

            if (send(clntSocket, &state, sizeof(state), 0) != sizeof(state))
                return "";
            break;
        case _Register:
            if ((recv(clntSocket, user.username, USERNAME, 0)) <= 0)
                return "";
            if ((recv(clntSocket, user.password, PASSWORD, 0)) <= 0)
                return "";

            if (Register(user)) //登録
                state = _Login;
            if (send(clntSocket, &state, sizeof(state), 0) != sizeof(state))
                return "";
            break;
        case _Login:
            if ((recv(clntSocket, username, USERNAME, 0)) <= 0)
                return "";
            if ((recv(clntSocket, password, PASSWORD, 0)) <= 0)
                return "";

            if (Check(username, password))
                logined = true;

            if (send(clntSocket, &logined, sizeof(bool), 0) != sizeof(bool))
                return "";
            break;
        default:
            break;
        }
    }
    printf("%sさんがログインしました\n", username);
    strcat(myhome, "/myhome/");
    strcat(myhome, username);
    chdir(myhome);
    return username;
}