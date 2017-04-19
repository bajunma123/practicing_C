#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int 
read_bar(char *bdf_addr, u8 bar_addr,  u32 offset, u8 count)
{
    char dev_loc[50]; 
    char *pcimem_arg[4];
    char *pcimem_env[] = {NULL};
    char check_device[30] = "lspci -s ";
    int bar_status; 
    int step = 0x4;
    int fd;

    strcat(check_device, bdf_addr);
    dev_status = system(check_device);
    if (dev_status) {
        printf("BDF %s does not exitst.\n", argv[1]);
        exit(1);
    }

    sprintf(dev_loc, "/sys/bus/pci/devices/0000:%s/resource%d", bdf_addr, bar_addr)
    if ((bar_status = access(dev_loc, F_OK)) < 0) {
        printf("BAR %d does not exist for BDF %s", bar_addr, bdf_addr);
    }

    pcimem_arg[0] = dev_loc;
    pcimem_arg[2] = "w";
    pcimem_arg[3] = NULL;

    fd = open("file.txt", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IROTH);
    //dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    for (i = 0; i < count; i++) {
        pcimem_arg[1] = offset + i * step;
        execve("./pcimem", pcimem_arg, pcimem_env);
    }

}

