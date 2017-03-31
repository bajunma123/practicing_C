#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX 32

int login(void);
struct student *create_student_info(struct student *, unsigned int , char *);
void diplay_student_info(struct student *);

struct student {
    unsigned int ID;
    char *student_name;
    struct student *next;
};

int main(void) 
{
    struct student *first = NULL;
    char *student_name;
    unsigned int ID;
    
    login();

    for (;;) {
        puts("please input the studetn's ID");
        scanf("%u", &ID);
        if (ID == 0) goto END;
        puts("please input the studetn's name");
        scanf("%s", student_name);
        first = create_student_info(first, ID, student_name);
    }

    END:
        puts("\n\nshow all the students's information: ");
        diplay_student_info(first);

    return 0;
}

struct student *create_student_info(struct student *first, 
                            unsigned int ID, char *student_name)
{
    struct student *new_student;

    new_student = malloc(sizeof(struct student));
    new_student->ID = ID;
    new_student->student_name = student_name;
    new_student->next = first;
    first = new_student;

    return first;
}

void diplay_student_info(struct student *first) 
{
    struct student *student_list;

    for (student_list = first; student_list != NULL; 
         student_list = student_list->next) 
        printf("%u\t\t%s\n", student_list->ID, student_list->student_name);
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
            (strcmp(ori_password, password) != 0)) {
            puts("your username or password is wrong!");
            exit(0);
        }
        else
            puts("\nlogin successfully, welcome!\n");
    }

    return 0;
}
