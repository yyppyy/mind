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
    open_file_req_t request;
    open_file_res_t response;
    strcpy(request.path, filename);
    request.flags = flags;
    request.mode = mode;
    send_msg_to_memory(MT_OPEN, &request, sizeof(request),&response, sizeof(&response));
    return response.file_descriptor;
}

int disaggr_read_file(unsigned int fd, char* usr_buf, size_t num_bytes){
    read_file_req_t request;
    read_file_res_t response;
    request.fd = fd;
    request.num_bytes = num_bytes;
    send_msg_to_memory(MT_READ, &request, sizeof(request),&response, sizeof(&response));
    strncpy(usr_buf,response.read_buf, response.bytes_read);
    return response.bytes_read;
}

int disaggr_write_file(unsigned int fd, char* usr_buf, size_t num_bytes){
    write_file_req_t request;
    write_file_res_t response;
    request.num_chars = num_bytes;
    send_msg_to_memory(MT_WRITE, &request, sizeof(request),&response, sizeof(&response));
    return response.bytes
}

int disaggr_close_file(unsigned int fd){
    close_file_req_t request;
    close_file_res_t response;
    request.fd = fd;
    send_msg_to_memory(MT_CLOSE, &request, sizeof(request),&response, sizeof(&response));
    return response.closed;
}

#endif