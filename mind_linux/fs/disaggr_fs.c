#include <linux/slab.h>
#include <linux/uaccess.h>
#include "disaggr_fs.h"
#include "disaggr_types.h"

extern int send_msg_to_memory(u32 msg_type, void *payload, u32 len_payload,
                        void *retbuf, u32 max_len_retbuf);

int disaggr_open_file(const char __user *filename, int flags, umode_t mode ){
    printk("hopen\n");
    
    open_file_req_t* request = kmalloc(sizeof(open_file_req_t), GFP_KERNEL);
    
    if(!request){
        printk("FAILED REQ ALLOC\n");
        return -1;
    }
    
    open_file_res_t* response = kmalloc(sizeof(open_file_res_t), GFP_KERNEL);
    if(!response){
        printk("FAILED RES ALLOC\n");
        return -1;
    }
    memset(request,0,sizeof(*request));
    response->file_descriptor = -1;
    strncpy_from_user(request->path, filename, 256);
    request->flags = flags;
    request->mode = mode;
    int open_res = send_msg_to_memory(MT_OPEN, request, sizeof(*request), response, sizeof(*response));
    printk("OPEN RES: %d\n", open_res);
    int fd = response->file_descriptor;
    printk("Returned fd: %d\n", fd);
    kfree(request);
    kfree(response);
    return fd;
}

int disaggr_read_file(unsigned int fd, char __user* usr_buf, size_t num_bytes){
    printk("hread\n");
    printk("hello I'm disaggr_read, called by %s\n", current->comm);
    if(!strcmp(current->comm, "in:imklog")){
        int end = 0;
        copy_to_user(usr_buf, &end, sizeof(end));
        return 0;
    }
    
    read_file_req_t* request = kmalloc(sizeof(read_file_req_t), GFP_KERNEL);
    if(!request){
        printk("FAILED REQ ALLOC\n");
        return -1;
    }
    read_file_res_t* response = kmalloc(sizeof(read_file_res_t), GFP_KERNEL);
    if(!response){
        printk("FAILED RES ALLOC\n");
        return -1;
    }
    memset(request, 0, sizeof(*request));
    request->fd = fd;
    request->num_bytes = num_bytes;
    send_msg_to_memory(MT_READ, request, sizeof(*request),response, sizeof(*response));
    printk("READ: %s", response->read_buf);
    printk("BYTES READ: %d", response->bytes_read);
    copy_to_user(usr_buf, response->read_buf, response->bytes_read);
    int bytes_read = response->bytes_read;
    kfree(request);
    kfree(response);
    return bytes_read;
}

int disaggr_write_file(unsigned int fd, const char __user * buf, size_t num_bytes){
    printk("hwrite\n");
    
    write_file_req_t* request = kmalloc(sizeof(write_file_req_t), GFP_KERNEL);
    write_file_res_t* response = kmalloc(sizeof(write_file_res_t), GFP_KERNEL);
    if(!response){
        printk("FAILED RES ALLOC\n");
        return -1;
    }
    memset(request, 0, sizeof(*request));
    request->num_chars = num_bytes;
    request->fd = fd;
    strncpy_from_user(request->write_buf, buf, 4096);
    send_msg_to_memory(MT_WRITE, request, sizeof(*request), response, sizeof(*response));
    int bytes_written = response->bytes_written;
    kfree(request);
    kfree(response);
    return bytes_written;
}

int disaggr_close_file(unsigned int fd){
    printk("hclose\n");
    
    close_file_req_t* request = kmalloc(sizeof(close_file_req_t), GFP_KERNEL);
    close_file_res_t* response = kmalloc(sizeof(close_file_res_t), GFP_KERNEL);
    if(!response){
        printk("FAILED RES ALLOC\n");
        return -1;
    }
    request->fd = fd;
    send_msg_to_memory(MT_CLOSE, request, sizeof(*request), response, sizeof(*response));
    int closed = response->closed;
    kfree(request);
    kfree(response);
    return closed;
}
