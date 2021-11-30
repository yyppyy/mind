#include <linux/slab.h>
#ifndef __DISAGGR_FDSET_H__
#define __DISAGGR_FDSET_H__
#define NUM_BUCKETS 8
typedef struct fdset_node fdset_node;
struct fdset_node{
    int val;
    fdset_node* next;
};

typedef struct disaggr_fdset disaggr_fdset;
struct disaggr_fdset{
    fdset_node* buckets[NUM_BUCKETS];
    int num_buckets;
};



//current->tgid; to allow for multiple processes.
fdset_node* fdnode_create(int val);

disaggr_fdset* fdset_create(void);

void fdset_add(int val);

int fdset_contains(int val);

void fdset_remove(int val);
#endif
