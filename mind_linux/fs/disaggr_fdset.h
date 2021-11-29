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
fdset_node* fdnode_create(int val){
    fdset_node *node = kmalloc(sizeof(fdset_node), GFP_KERNEL);
    node->val = val;
    node->next = NULL;
    return node;
}

disaggr_fdset* fdset_create(void){
    disaggr_fdset* fdset = kmalloc(sizeof(disaggr_fdset), GFP_KERNEL);
    if(!fdset){
        return NULL;
    }
    fdset->num_buckets = 8;
    fdset-> buckets = kmalloc(fdset->num_buckets * sizeof(int*), GFP_KERNEL);
    int i;
    for(i = 0; i < fdset->num_buckets; ++i){
        fdset->buckets[i] = NULL;
    }
    return fdset;
}



void fdset_add(disaggr_fdset* set, int val){
    int hash_idx = set->num_buckets % val;
    fdset_node* curr = set->buckets[hash_idx];
    fdset_node* node = fdnode_create(val);
    if(!curr){
        curr = node;
        return;
    }
    while(curr->next){
        curr = curr->next;
    }
    curr->next = node;
}

int fdset_contains(disaggr_fdset* set, int val){
    int hash_idx = set->num_buckets % val;
    fdset_node* curr = set->entries[hash_idx];
    while(curr){
        if(curr->val == val){
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void fdset_remove(disaggr_fdset* set, int val){
  int hash_idx = set->num_buckets % val;
    fdset_node* curr = set->entries[hash_idx];
    fdset_node* last = curr;
    while(curr){
        if(curr->val == val){
            //It is the first element in the chaing
            if(last == curr){
                set->entries[hash_idx] = NULL;
            } else {
                last->next = curr->next;
            }
            kfree(curr);
        }
        last = curr;
        curr = curr->next;
    }  
}
#endif
