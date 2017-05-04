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

u32 pcimem_read(char *bdf_addr, unsigned long base_addr, int type) {
	int fd;
	void *map_base, *virt_addr;
	uint32_t read_result;
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
    
	if(munmap(map_base, MAP_SIZE) == -1) CLS_ERR("puts error here.");
    close(fd);

    return read_result;
}


u32 pcimem_write(char *bdf_addr, unsigned long base_addr, int type, char *data) {
	int fd;
	void *map_base, *virt_addr;
	uint32_t writeval;
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

	if(munmap(map_base, MAP_SIZE) == -1) CLS_ERR("puts error here.");
    close(fd);

    return read_result;
}

/*
 * int read_bar(char *bdf_addr, unsigned int bar_addr,  u32 offset, unsigned int count)
 * {
 *     int step = 0x4;
 *     u32 data[50];
 * 
 *     data = pcimem_read(bdf_addr, bar_addr, 'w');
 * 
 *     return data;
 * }
 * 
 * int write_bar(char *bdf_addr, unsigned int bar_addr,  u32 offset, char *data)
 * {
 *     int step = 0x4;
 *     u32 data;
 * 
 *     bar_addr = offset + i * step;
 *     data = pcimem_write(bdf_addr, bar_addr, 'w', data);
 * 
 *     return 0;
 * }
 */

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


int setup()
{
    int ntb_offset, ntb_offset_1, ntb_offset_2, ntb_offset_3, ntb_offset_4;
    int h;
    u32 host_bridge, read_result;
    u16 req_id;

#define BAR0 0;

    ntb_offset = (0x14 + 2 * h) << 12;
    ntb_offset_1 = ntb_offset + 0x004;
    ntb_offset_2 = ntb_offset + 0x060;
    ntb_offset_3 = ntb_offset + 0x080;
    ntb_offset_4 = ntb_offset + 0x400;
    ntb_offset_5 = ntb_offset + 0x404;

    write_bar(bdf_addr, BAR0 , ntb_offset_1, 0x3);
    read_result = read_bar(bdf_addr, 0, 0x1000c, 1);
    host_bridge = read_bar & 0xFFFF;
    
    write_bar(bdf_addr, 0, ntb_offset_1, 0x1);
    read_result = read_bar(bdf_addr, 0, ntb_offset_4, 1);
    id_entry = read_result | 0x1;

    write_bar(bdf_addr, 0, ntb_offset_4, id_entry);

    read_result = read_bar(bdf_addr, 0, ntb_offset_5, 1);
    id_entry = read_result | (host_bridge <<= 16) | 0x1;
    write_bar(bdf_addr, 0, ntb_offset_5, id_entry);
    write_bar(bdf_addr, 0, ntb_offset_2, 0x15);
    write_bar(bdf_addr, 0, ntb_offset_2, 0x400001a);
    write_bar(bdf_addr, 0, ntb_offset_2, 0x0000000);
    write_bar(bdf_addr, 0, ntb_offset_3, 0x15);
    write_bar(bdf_addr, 0, ntb_offset_3, 0x200015);
    write_bar(bdf_addr, 0, ntb_offset_3, 0x4000000);
    write_bar(bdf_addr, 0, ntb_offset_1, 0x2);
    write_bar(bdf_addr, 4, ntb_offset_1, 0x1);
    write_bar(bdf_addr, 4, ntb_offset_1, 0x1);

    read_result = read_bar(bdf_addr, 0, ntb_offset_4, 1);
    req_id = (read_result & 0x1E) >> 1;

    read_result = read_bar(bdf_addr, 4, ntb_offset_4, 1);
    id_entry = read_result | 0x0(h + 1)    req_id << 16;   // there is an error in the script file.
    write_bar(bdf_addr, 4, ntb_offset_4, id_entry);

    read_result = read_bar(bdf_addr, 0, ntb_offset_5, 1);
    req_id = (read_result & 0x1E) >> 1;

    read_result = read_bar(bdf_addr, 4, ntb_offset_5, 1);
    id_entry = read_result | 0x0(h + 1)    req_id << 16;   // there is an error in the script file.
    write_bar(bdf_addr, 4, ntb_offset_5, id_entry);
    write_bar(bdf_addr, 4, ntb_offset_1, 0x2);

    for (int i = 1; i <= 0x10; i++) {
        offset = offset + i * step;
        read_result = read_bar(bdf_addr, 4, offset, 0x10);
        printf("%08x", read_result);
        if (i % 4 == 0)
            printf("\n");
    }

