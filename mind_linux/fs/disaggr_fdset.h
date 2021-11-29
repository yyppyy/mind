#include <linux/slab.h>
#ifndef __DISAGGR_FDSET_H__
#define __DISAGGR_FDSET_H__
typedef struct fdset_node fdset_node;
struct fdset_node{
    int val;
    fdset_node* next;
};

typedef struct disaggr_fdset disaggr_fdset;
struct disaggr_fdset{
    fdset_node** buckets;
    int num_buckets;
};


disaggr_fdset D_FDSET;

//current->tgid; to allow for multiple processes.
fdset_node* fdnode_create(int val);

disaggr_fdset* fdset_create(void);

void fdset_add(disaggr_fdset* set, int val);

int fdset_contains(disaggr_fdset* set, int val);

void fdset_remove(disaggr_fdset* set, int val);
#endif
