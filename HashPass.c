#include "Main.h"

char *hashPass(char plane[PASSWORD])
{
    char *res = (char *)malloc(sizeof(char) * PASSWORD);
    const char *encrypted;

    encrypted = crypt(plane, "$5$jfhvbcue");
    strcpy(res,encrypted + 12);
    return res;
}