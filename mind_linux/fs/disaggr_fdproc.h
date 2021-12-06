#include <linux/slab.h>
#ifndef __DISAGGR_FDPROC_H__
#define __DISAGGR_FDPROCC_H__
#define NUM_FDPROC_BUCKETS 8
typedef struct fdproc_node fdproc_node;
struct fdproc_node{
    int val;
    fdproc_node* next;
};

typedef struct disaggr_fdproc disaggr_fdproc;
struct disaggr_fdproc_set{
    fdproc_node* buckets[NUM_FDPROC_BUCKETS];
    int num_buckets;
};

//current->tgid; to allow for multiple processes.
fdproc_node* fdnode_create(int val);

disaggr_fdproc_set* fdproc_create(void);

void fdproc_add(int val);

int fdproc_contains(int val);

void fdproc_remove(int val);
#endif
