#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX 32

void login(void);

struct student {
    unsigned int ID;
    char student_name[32];
    struct student *next;
};


struct student *create_student_info(struct student *);
void diplay_student_info(struct student *);
void search_student_info(struct student *);
void delete_student_info(struct student *);

int main(void) 
{
    struct student *first = NULL;
    char *student_name;
    unsigned int ID, option;
    
    login();

    printf("1. insert students information.\n"
           "2. search and display the students' information.\n"
           "3. revise the students' information.\n"
           "4. delete the students' information.\n"
           "5. quit and exit the program.\n");

    //printf("%d", option);
    for (;;) {
        puts("\nplease select the number your want to operate: ");
        scanf("%d", &option);
        switch(option) {
            case 1:
                first = create_student_info(first);
                break;
            case 2:
                search_student_info(first);
                break;
            //case 'u':
            //    update_student_info();
            //    break;
            //case 'd':
            //    delete_student_info();
            //    break;
            default:
                if (first != NULL) {
                    puts("all the students' information:\n");
                    diplay_student_info(first);
                    return 0;
                }
                else {
                    puts("\n    PLEASE SELECT THE RIGHT OPERATION NUMBER!");
                    return 0;
                }

        }
    }
    
    return 0;

}

struct student *create_student_info(struct student *first)
{
    FILE *fp;
    struct student *new_student;

    new_student = malloc(sizeof(struct student));

    puts("please input the studetn's ID");
    scanf("%d", &new_student->ID);
    puts("please input the studetn's name");
    scanf("%s", new_student->student_name);

    new_student->next = first;
    first = new_student;

    if ((fp = fopen("stu_info.bin", "ab")) != NULL) {
        fwrite(new_student, sizeof(struct student), 1, fp);
        puts("write into file here");
    }

    return first;
}

void search_student_info(struct student *search_info)
{
    int item_num, ID;
    char *student_name;
    struct student *p;

    puts("which information do you want to search, just select the number below:\n"
         "      1. ID\n"
         "      2. student_name \n"
        );

    scanf("%d", &item_num);
    switch(item_num) {
        case 1:
            puts("please input the ID number");
            scanf("%d", &ID);
            for (p = search_info; p != NULL && p->ID != search_info->ID; p = p->next)
                ;
            printf("%d\t%s\n", p->ID, p->student_name);
            break;
        case 2:
            puts("please input the student's name");
            scanf("%s", student_name);
            for (p = search_info; p != NULL && 
                 p->student_name != search_info->student_name;
                 p = p->next)
                ;
            printf("%u\t\t%s\n", p->ID, p->student_name);
            break;
    }

}

void diplay_student_info(struct student *first) 
{
    struct student *student_list;

    for (student_list = first; student_list != NULL; 
         student_list = student_list->next) 
        printf("%d\t\t%s\n", student_list->ID, student_list->student_name);

}

void login(void)
{
    char *username, *ori_useranme = "aa";
    char *password, *ori_password = "11";

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
}
