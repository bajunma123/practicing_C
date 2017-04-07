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
void save_info_file(struct student *);
void display_student_info(struct student *);
void search_student_info(struct student *);
void update_student_info(struct student *);
struct student *delete_student_info(struct student *);
struct student *sort_student_info(struct student *);


int main(void) 
{
    char *student_name;
    unsigned int ID, option;
    
    login();

    for (;;) {
        printf("\nOptions:\n"
               "    1. insert students information.\n"
               "    2. search and display the students' information.\n"
               "    3. update the students' information.\n"
               "    4. delete the students' information.\n"
               "    5. sort the students' information.\n"
               "    6. display all the students' information.\n"
               "    7. save to file.\n"
               "    8. quit and exit the program.\n");

        //printf("%d", option);
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
                    first = delete_student_info(first);
                    break;
                case 5:
                    sort_student_info(first);
                    break;
                case 6:
                    display_student_info(first);
                    break;
                case 7:
                    save_info_file(first);
                    break;
                default:
                    puts("Exit this program!");
                    exit(0);

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


    return first;
}

void search_student_info(struct student *first)
{
    int item_num, ID, age;
    char name[32];
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
            printf("ID: %d\t name: %s\t age: %d\t math: %d\t yuwen: %d\t english: %d\t\n", 
                    p->ID, p->name, p->age, p->math, p->yuwen, p->english);
            break;
        case 2:
            puts("Please input the student's name: ");
            scanf("%s", name);
            puts("Search Results: ");
            p = first;
            while (p != NULL && strcmp(p->name, name) == 0) {
                printf("ID: %d\t name: %s\t age: %d\t math: %d\t yuwen: %d\t english: %d\t\n", 
                        p->ID, p->name, p->age, p->math, p->yuwen, p->english);
                p = p->next;
            }
            break;
        case 3:
            puts("Please input the student's age");
            scanf("%d", &age);
            puts("Search Results: ");
            for (p = first; p != NULL && p->age == age; p = p->next) {
                printf("ID: %d\t name: %s\t age: %d\t math: %d\t yuwen: %d\t english: %d\t\n", 
                        p->ID, p->name, p->age, p->math, p->yuwen, p->english);
            }
            break;
    }

}

void save_info_file(struct student *first)
{
    struct student *p;
    FILE *fp;

    if ((fp = fopen("stu_info.bin", "wb")) == NULL) {
        puts("Cannot open this file.");
    }
    p = first;
    printf("\nWrite the students' info into file.\nDone.\n");
    while(p != NULL) {
        fwrite(p, sizeof(struct student), 1, fp);
        p = p->next;
    }

    fclose(fp);
}

void display_student_info(struct student *first) 
{
    struct student *p;

    if (first == NULL)
        printf("\nThe students' information is empty.\n");
    else {
        printf("\nAll the students' information: \n");
        for (p = first; p != NULL; p = p->next) 
            printf("ID: %d\t name: %s\t age: %d\t math: %d\t yuwen: %d\t english: %d\t\n", 
                    p->ID, p->name, p->age, p->math, p->yuwen, p->english);
    }

}

void update_student_info(struct student *first)
{
    int item_num, ID, age, new_ID;
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
            puts("Please input the student's new ID number");
            scanf("%d", &new_ID);
            p->ID = new_ID;
            puts("The related information is changed to: ");
            printf("ID: %d\t name: %s\t age: %d\t math: %d\t yuwen: %d\t english: %d\t\n", 
                    p->ID, p->name, p->age, p->math, p->yuwen, p->english);
            break;
        case 2:
            puts("Please input the student's ID number you want to update: ");
            scanf("%d", &ID);
            for (p = first; p != NULL && p->ID != ID; p = p->next)
                ;
            puts("Please input the student's new name");
            scanf("%s", p->name);
            puts("The related information is changed to: ");
            printf("ID: %d\t name: %s\t age: %d\t math: %d\t yuwen: %d\t english: %d\t\n", 
                    p->ID, p->name, p->age, p->math, p->yuwen, p->english);
            break;
        case 3:
            puts("Please input the student's ID number you want to update: ");
            scanf("%d", &ID);
            for (p = first; p != NULL && p->ID != ID; p = p->next)
                ;
            puts("Please input the student's new age");
            scanf("%d", &age);
            p->age= age;
            puts("The related information is changed to: ");
            printf("ID: %d\t name: %s\t age: %d\t math: %d\t yuwen: %d\t english: %d\t\n", 
                    p->ID, p->name, p->age, p->math, p->yuwen, p->english);
            break;
    }

}

struct student *delete_student_info(struct student *first)
{
    struct student *p, *prep;
    int ID;

    puts("Please input the student's ID number you want to delete: ");
    scanf("%d", &ID);
    p = first;
    while (p->ID == ID) {
        first = p->next;
        free(p);
        return first;
    }

    while(p->ID != ID) {
        prep = p;
        p = p->next;
    }
    prep->next = p->next;
    free(p);
    return first;
}

struct student *sort_student_info(struct student *first)
{
    struct student *ptr1, *ptr2, *temp;
    ptr1 = first;
    while(ptr1->next != NULL)
    {
        ptr2 = ptr1->next;
        while(ptr2 != NULL) {
            if(ptr1->ID > ptr2->ID) {
                temp->ID = ptr1->ID;
                ptr1->ID = ptr2->ID;
                ptr2->ID = temp->ID;

                strcpy(temp->name, ptr1->name);
                strcpy(ptr1->name, ptr2->name);
                strcpy(ptr2->name, temp->name);

                temp->age = ptr1->age;
                ptr1->age = ptr2->age;
                ptr2->age = temp->age;

                temp->math = ptr1->math;
                ptr1->math = ptr2->math;
                ptr2->math = temp->math;

                temp->yuwen = ptr1->yuwen;
                ptr1->yuwen = ptr2->yuwen;
                ptr2->yuwen = temp->yuwen;

                temp->english = ptr1->english;
                ptr1->english = ptr2->english;
                ptr2->english = temp->english;
            }
            ptr2 = ptr2->next;
        }
        ptr1 = ptr1->next;
    }
    printf("\nSort the student information as per their ID number.\nDone.\n");
    return first;
}

void login(void)
{
    char *username, *ori_useranme = "admin";
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
