#include <linux/slab.h>
#include "disaggr_fs.h"
#include "disaggr_types.h"

extern int send_msg_to_memory(u32 msg_type, void *payload, u32 len_payload,
                        void *retbuf, u32 max_len_retbuf);

int disaggr_open_file(const char __user *filename, int flags, umode_t mode ){
    printk("hopen\n");
    printk_safe_flush();
    if(!filename){
        return -5;
    }
    open_file_req_t* request = kmalloc(sizeof(open_file_req_t), GFP_KERNEL);
    if(!request){
        return -1;
    }
    
    open_file_res_t* response = kmalloc(sizeof(open_file_res_t), GFP_KERNEL);
    if(!response){
        return -2;
    }
    if(strlen(filename) > 256){
        panic("OHHH");
    }
    strcpy(request->path, filename);
    request->flags = flags;
    request->mode = mode;
    send_msg_to_memory(MT_OPEN, request, sizeof(*request), response, sizeof(*response));
    int fd = response->file_descriptor;
    kfree(request);
    kfree(response);
    return fd;
}

int disaggr_read_file(unsigned int fd, char* usr_buf, size_t num_bytes){
    printk("hread\n");
    printk_safe_flush();
    read_file_req_t* request = kmalloc(sizeof(read_file_req_t), GFP_KERNEL);
    read_file_res_t* response = kmalloc(sizeof(read_file_res_t), GFP_KERNEL);
    request->fd = fd;
    request->num_bytes = num_bytes;
    send_msg_to_memory(MT_READ, request, sizeof(*request),response, sizeof(*response));
    strncpy(usr_buf, response->read_buf, response->bytes_read);
    int bytes_read = response->bytes_read;
    kfree(request);
    kfree(response);
    return bytes_read;
}

int disaggr_write_file(unsigned int fd, char* usr_buf, size_t num_bytes){
    printk("hwrite\n");
    printk_safe_flush();
    write_file_req_t* request = kmalloc(sizeof(write_file_req_t), GFP_KERNEL);
    write_file_res_t* response = kmalloc(sizeof(write_file_res_t), GFP_KERNEL);
    request->num_chars = num_bytes;
    send_msg_to_memory(MT_WRITE, request, sizeof(*request),response, sizeof(*response));
    int bytes_written = response->bytes_written;
    kfree(request);
    kfree(response);
    return bytes_written;
}

int disaggr_close_file(unsigned int fd){
    printk("hclose\n");
    printk_safe_flush();
    close_file_req_t* request = kmalloc(sizeof(close_file_req_t), GFP_KERNEL);
    close_file_res_t* response = kmalloc(sizeof(close_file_res_t), GFP_KERNEL);
    request->fd = fd;
    send_msg_to_memory(MT_CLOSE, request, sizeof(*request), response, sizeof(*response));
    int closed = response->closed;
    kfree(request);
    kfree(response);
    return closed;
}