
#include <linux/slab.h>
#ifndef __DISAGGR_H__
#define __DISAGGR_H__
// #include <disagg/network_disagg.h>
#include "disaggr_types.h"
#define MT_OPEN 1
#define MT_CLOSE 2
#define MT_READ 3
#define MT_WRITE 4

extern int send_msg_to_memory(u32 msg_type, void *payload, u32 len_payload,
                        void *retbuf, u32 max_len_retbuf);

int disaggr_open_file(const char __user *filename, int flags, umode_t mode ){
    open_file_req_t* request = kmalloc(sizeof(open_file_req_t), GFP_USER);
    open_file_res_t* response = kmalloc(sizeof(open_file_res_t), GFP_USER);
    strcpy(request->path, filename);
    request->flags = flags;
    request->mode = mode;
    send_msg_to_memory(MT_OPEN, request, sizeof(*request), response, sizeof(*response));
    return response->file_descriptor;
}

int disaggr_read_file(unsigned int fd, char* usr_buf, size_t num_bytes){
    read_file_req_t* request = kmalloc(sizeof(read_file_req_t), GFP_USER);
    read_file_res_t* response = kmalloc(sizeof(read_file_res_t), GFP_USER);
    request->fd = fd;
    request->num_bytes = num_bytes;
    send_msg_to_memory(MT_READ, request, sizeof(*request),response, sizeof(*response));
    strncpy(usr_buf, response->read_buf, response->bytes_read);
    return response->bytes_read;
}

int disaggr_write_file(unsigned int fd, char* usr_buf, size_t num_bytes){
    write_file_req_t* request = kmalloc(sizeof(write_file_req_t), GFP_USER);
    write_file_res_t* response = kmalloc(sizeof(write_file_res_t), GFP_USER);
    request->num_chars = num_bytes;
    send_msg_to_memory(MT_WRITE, request, sizeof(*request),response, sizeof(*response));
    return response->bytes_written;
}

int disaggr_close_file(unsigned int fd){
    close_file_req_t* request = kmalloc(sizeof(close_file_req_t), GFP_USER);
    close_file_res_t* response = kmalloc(sizeof(close_file_res_t), GFP_USER);
    request->fd = fd;
    send_msg_to_memory(MT_CLOSE, request, sizeof(*request), response, sizeof(*response));
    return response->closed;
}

#endif