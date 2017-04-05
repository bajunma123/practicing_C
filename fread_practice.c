#include <stdio.h>

struct student {
    unsigned int ID;
    char student_name[32];
    struct student *next;
};

int main(void)
{
    FILE *fp;
    struct student stu, *stu1, *stu2;
    stu1 = &stu;

    fp = fopen("stu_info.bin", "rb");
    for(; fread(stu1, sizeof(struct student), 1, fp) > 0; stu1 += 3)
        printf("%d\t%s\n", stu1->ID, stu1->student_name);
    fclose(fp);

    return 0;
}
