#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int dev_status, bar_status;
    int count;
    int step = 0x4;
    char check_device[30];
    char dev_loc[100]; 

    sprintf(check_device, "lspci -s %s", argv[1]);
    sprintf(dev_loc, "/sys/bus/pci/devices/0000:%s/resource%s", argv[1], argv[2]);
    //puts(check_device);
    //puts(dev_loc);
    dev_status = system(check_device);
    if (dev_status) {
        printf("BDF %s does not exitst.\n", argv[1]);
        exit(1);
    }

    if ((bar_status = access(dev_loc, F_OK)) < 0) {
        printf("BAR %s does not exist for BDF %s", argv[1], argv[2]);
    }

    if (argc < 3)
        count = 1;
    else
        count = atoi(argv[4]);


    char *newargv[10];
    char *newenv = {NULL};

        strcat(cmd_line, dev_loc, atoi(argv[3])+i*step);
        argv[0] = cmd_line;
        newargv[] = {cmd_line, NULL} 
        execve("./pcimem", newargv, newenv);



    return 0;
}

