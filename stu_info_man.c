#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int login(void);

int main(void)
{
    login();

    return 0;
}


int login(void)
{
    char *username, *ori_useranme = "abcdef";
    char *password, *ori_password = "111111";

    username = (char *) malloc(sizeof(ori_useranme));
    password = (char *) malloc(sizeof(ori_password));

    if ((username != NULL) && (password != NULL)) {

        puts("please enter your username: ");
        scanf("%s", username);

        puts("please enter your password: ");
        scanf("%s", password);

        if ((strcmp(ori_useranme, username) != 0) |
            (strcmp(ori_password, password) != 0))
            printf("your username or password is wrong!\n");
        else
            puts("login successfully, welcome!");
    }

    free(username);
    free(password);

    return 0;
}
