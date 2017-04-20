#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "nebulaapp.h"
#include "psxapp.h"
#include "error.h"

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

    fd = open("read_bar.log", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IROTH);
    //dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    for (i = 0; i < count; i++) {
        pcimem_arg[1] = offset + i * step;
        execve("./pcimem", pcimem_arg, pcimem_env);
    }

    return 0;
}

int file_analysis(void)
{
    const char *regex = "grep '0x\\w\\{8\\}$' read_bar.log -o | xxd -r -ps | xxd -g 4 -c 16";
    system(regex);

    return 0;
}


int cmiconf_show(int numArg, char **pArg, struct command *cmd, struct plugin *plugin)
{
    const char *desc = "show csr";

    struct config {
        char *bdf_addr;
        u8 bar_addr;
        u32 offset;
        u8 count;
    };

    struct config cfg = {
        .count = 1,
        .offset = 0x134000,
    };

    const struct argconfig_commandline_options command_line_options[] = {
        {"bdf_addr", 'b', "", CFG_STRING, &cfg.bdf_addr, reqired_argument, bdf_addr},
        {"bar_addr", 'a', "NUM", CFG_POSITIVE, &cfg.bar_addr, required_argument, bar_addr},
        {"offset", 'o', "", CFG_POSITIVE, &cfg.offset, required_argument, offset},
        {"count", 'c', "NUM", CFG_POSITIVE, &cfg.count, required_argument, count},
    };

    parse_and_open(numArg, pArg, desc, command_line_options, &cfg, sizeof(cfg));

    ret = read_bar(&cfg.bdf_addr, cfg.bar_addr, cfg.offset, cfg.count);
    if (ret != SUCCESS) {
        CLS_ERR("show csr informaition failed.\n");
    }
}
