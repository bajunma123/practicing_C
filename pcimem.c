#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)
#define BUFFER_SIZE 300

int pcimem(char *bdf_addr, unsigned long base_addr, int type, char *data) {
	int fd;
	void *map_base, *virt_addr;
	uint32_t read_result, writeval;
	off_t target = base_addr;
	int access_type = type;

    sprintf(dev_loc, "/sys/bus/pci/devices/0000:%s", bdf_addr);
    if ((bar_status = access(dev_loc, F_OK)) < 0) {
        printf("BDF %s does not exist", bdf_addr);
    } else {
        sprintf(dev_loc, "/sys/bus/pci/devices/0000:%s/resource%d", bdf_addr, bar_addr)
        if ((bar_status = access(dev_loc, F_OK)) < 0) {
            printf("BAR %d does not exist for BDF %s", bar_addr, bdf_addr);
        }
    }

    if((fd = open(dev_loc, O_RDWR | O_SYNC)) == -1) CLS_ERR("put error here.");

    /* Map one page */
    map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
    if(map_base == (void *) -1) CLS_ERROR("put error here");

    virt_addr = map_base + (target & MAP_MASK);
    switch(access_type) {
		case 'b':
			read_result = *((uint8_t *) virt_addr);
			break;
		case 'h':
			read_result = *((uint16_t *) virt_addr);
			break;
		case 'w':
			read_result = *((uint32_t *) virt_addr);
			break;
		default:
			fprintf(stderr, "Illegal data type '%c'.\n", access_type);
			exit(2);
	}
    printf("%8X\n", read_result);
    fflush(stdout);

	if(data != NULL) {
		writeval = strtoul(data, 0, 0);
		switch(access_type) {
			case 'b':
				*((uint8_t *) virt_addr) = writeval;
				read_result = *((uint8_t *) virt_addr);
				break;
			case 'h':
				*((uint16_t *) virt_addr) = writeval;
				read_result = *((uint16_t *) virt_addr);
				break;
			case 'w':
				*((uint32_t *) virt_addr) = writeval;
				read_result = *((uint32_t *) virt_addr);
				break;
		}
		printf("%8X\n", read_result);
        fflush(stdout);
	}

	if(munmap(map_base, MAP_SIZE) == -1) CLS_ERR("puts error here.");
    fclose(pci_fd);
    close(fd);
    return 0;
}

int read_bar(char *bdf_addr, unsigned int bar_addr,  u32 offset, unsigned int count)
{
    int step = 0x4;

    for (i = 1; i <= count; i++) {
        bar_addr = offset + i * step;
        pcimem(bdf_addr, bar_addr, 'w', NULL);
        if (i % 4 == 0)
            printf("\n");
    }

    return 0;
}

int write_bar(char *bdf_addr, unsigned int bar_addr,  u32 offset, char *data)
{
    int step = 0x4;

    bar_addr = offset + i * step;
    pcimem(bdf_addr, bar_addr, 'w', data);

    return 0;
}

int cmiconf_setup(int numArg, char **pArg, struct command *cmd, struct plugin *plugin)
{
    const char *desc = "setup csr";

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
