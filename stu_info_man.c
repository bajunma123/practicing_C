#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_MAX 32

void login(void);

struct student {
    unsigned int ID;
    char name[32];
    unsigned int age;
    unsigned int math;
    unsigned int yuwen;
    unsigned int english;
    struct student *next;
};

struct student *first = NULL;

struct student *create_student_info(struct student *);
//struct student *diplay_student_info(struct student *);
void search_student_info(struct student *);
void update_student_info(struct student *);
struct student *delete_student_info(struct student *);

int main(void) 
{
    char *student_name;
    unsigned int ID, option;
    
    login();

    printf("Options:\n"
           "    1. insert students information.\n"
           "    2. search and display the students' information.\n"
           "    3. revise the students' information.\n"
           "    4. delete the students' information.\n"
           "    5. save to file.\n"
           "    6. quit and exit the program.\n");

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
            case 3:
                update_student_info(first);
                break;
            case 4:
                delete_student_info(first);
                break;
            default:
                if (first != NULL) {
                    puts("all the students' information:\n");
                    //diplay_student_info(first);
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

    if (new_student == NULL) {
        printf("No enough memory space");
        exit(0);
    }

    puts("please input the student's ID: ");
    scanf("%d", &new_student->ID);
    puts("please input the student's name: ");
    scanf("%s", new_student->name);
    puts("please input the student's age (0~100): ");
    scanf("%u", &new_student->age);
    puts("please input the student's math score (0~150): ");
    scanf("%u", &new_student->math);
    puts("please input the student's yuwen score (0-150): ");
    scanf("%u", &new_student->yuwen);
    puts("please input the student's english score (0~150): ");
    scanf("%u", &new_student->english);
    if (new_student->age > 100 || new_student->math > 150 ||
        new_student->yuwen > 150 || new_student->english > 150) {
        puts("Please check your input value in the right scope");
        exit(0);
    }

    new_student->next = first;
    first = new_student;

    if ((fp = fopen("stu_info.bin", "ab")) != NULL) {
        fwrite(new_student, sizeof(struct student), 1, fp);
        puts("write your data into file.");
    }

    return first;
}

void search_student_info(struct student *first)
{
    int item_num, ID, age;
    char *name;
    struct student *p;

    puts("which information do you want to search, just select the number below:\n"
         "      1. ID\n"
         "      2. name \n"
         "      3. age\n"
        );

    scanf("%d", &item_num);
    switch(item_num) {
        case 1:
            puts("Please input the student's ID number");
            scanf("%d", &ID);
            for (p = first; p != NULL && p->ID != ID; p = p->next)
                ;
            // if cannot find the result, what will happen?
            // deal with this issue later
            puts("Search Results: ");
            printf("%d\t %s\t %d\t %d\t %d\t %d\t\n", 
                    p->ID, 
                    p->name, 
                    p->age,
                    p->math, 
                    p->yuwen, 
                    p->english);
            break;
        case 2:
            puts("Please input the student's name");
            scanf("%s", name);
            for (p = first; p != NULL && p->name == name; p = p->next) {
                puts("Search Results: ");
                printf("%d\t %s\t %d\t %d\t %d\t %d\t\n", 
                        p->ID, 
                        p->name, 
                        p->age,
                        p->math, 
                        p->yuwen, 
                        p->english);
            }
            break;
        case 3:
            puts("Please input the student's age");
            scanf("%d", &age);
            for (p = first; p != NULL && p->age== age; p = p->next) {
                puts("Search Results: ");
                printf("%d\t %s\t %d\t %d\t %d\t %d\t\n", 
                        p->ID, 
                        p->name, 
                        p->age,
                        p->math, 
                        p->yuwen, 
                        p->english);
            }
            break;
    }

}

//void display_student_info(struct student *first) 
//{
//    struct student *student_list;
//
//    for (student_list = first; student_list != NULL; 
//         student_list = student_list->next) 
//        printf("%d\t\t%s\n", student_list->ID, student_list->student_name);
//
//}

void update_student_info(struct student *first)
{
    int item_num, ID, age;
    char *name;
    struct student *p;

    // just show 3 options to update, the logic is same.
    puts("Which information do you want to update, just select the number below:\n"
         "      1. ID\n"
         "      2. name \n"
         "      3. age\n"
        );

    scanf("%d", &item_num);
    switch(item_num) {
        case 1:
            puts("Please input the student's ID number you want to update: ");
            scanf("%d", &ID);
            for (p = first; p != NULL && p->ID != ID; p = p->next)
                ;
            p->ID = ID;
            puts("The related information is changed to: ");
            printf("%d\t %s\t %d\t %d\t %d\t %d\t\n", 
                    p->ID, 
                    p->name, 
                    p->age,
                    p->math, 
                    p->yuwen, 
                    p->english);
            break;
        case 2:
            puts("Please input the student's ID number and name" 
                 "you want to update: ");
            scanf("%d%s", &ID, name);
            for (p = first; p != NULL && p->ID != ID; p = p->next)
                ;
            p->name[32] = *name;
            puts("The related information is changed to: ");
            printf("%u\t %s\t %u\t %u\t %u\t %u\t\n", 
                    p->ID, 
                    p->name, 
                    p->age,
                    p->math, 
                    p->yuwen, 
                    p->english);
            break;
        case 3:
            puts("Please input the student's ID number and age" 
                 "you want to update: ");
            scanf("%d%d", &ID, &age);
            for (p = first; p != NULL && p->ID != ID; p = p->next)
                ;
            p->age= age;
            puts("The related information is changed to: ");
            printf("%u\t %s\t %u\t %u\t %u\t %u\t\n", 
                    p->ID, 
                    p->name, 
                    p->age,
                    p->math, 
                    p->yuwen, 
                    p->english);
            break;
    }

}

struct student *delete_student_info(struct student *first)
{
    int ID;
    struct student *ptrStu, *preptrStu;

    if (first != NULL) {
        if (first->next == NULL || first->ID == ID)
            free(first);
        else {
            puts("Please input the student's ID number you want to delete: ");
            scanf("%d", &ID);
            ptrStu = first;
            while (ptrStu->ID != ID) {
                preptrStu = ptrStu;
                ptrStu = ptrStu->next;
            }
            preptrStu->next = ptrStu->next;
            free(ptrStu);
        }
    }
    if (first == NULL)
        puts("The students' information is empty");

    return first;
}



void login(void)
{
    char *username, *ori_useranme = "aa";
    char *password, *ori_password = "11";
    int i = 0;

    username = (char *) malloc(sizeof(ori_useranme));
    password = (char *) malloc(sizeof(ori_password));

    if ((username != NULL) && (password != NULL)) {

        puts("please enter your username: ");
        scanf("%s", username);

        puts("please enter your password: ");
        scanf("%s", password);

        //if ((strcmp(ori_useranme, username) != 0) |
        //    (strcmp(ori_password, password) != 0)) {
        //    puts("your username or password is wrong!");
        //    exit(0);
        //}
        //else
        //    puts("\nlogin successfully, welcome!\n");

        // input three times, the program will exit, there is 
        // a issue for the last time. latter to solve it.
        while ((strcmp(ori_useranme, username) != 0) |
            (strcmp(ori_password, password) != 0)) {
            printf("Sorry, your username or password is wrong!"
                   " you have %d times left to try.\n", 2-i);
            puts("Please enter your username: ");
            scanf("%s", username);
            puts("Please enter your password: ");
            scanf("%s", password);
            i++;
            if (i >= 2) {
                puts("Your username or password is wrong! EXIT!");
                exit(0);
            }
        }

        if ((strcmp(ori_useranme, username) == 0) |
            (strcmp(ori_password, password) == 0)) {
            puts("\nLogin successfully, Welcome!\n\n");
        }
    }
}
