

#ifndef __DISAGGR_FS_H__
#define __DISAGGR_FS_H__
// #include <disagg/network_disagg.h>
#define MT_OPEN 1
#define MT_CLOSE 2
#define MT_READ 3
#define MT_WRITE 4


int disaggr_open_file(const char __user *filename, int flags, umode_t mode );

int disaggr_read_file(unsigned int fd, char* usr_buf, size_t num_bytes);

int disaggr_write_file(unsigned int fd, char* usr_buf, size_t num_bytes);

int disaggr_close_file(unsigned int fd);

#endif