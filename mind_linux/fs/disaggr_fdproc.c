#include <linux/slab.h>
#include "disaggr_fdproc.h"


disaggr_fdproc_set D_FDPROC;

fdproc_node* fdnode_create(int val){
    fdproc_node *node = kmalloc(sizeof(fdproc_node), GFP_KERNEL);
    node->val = val;
    node->next = NULL;
    return node;
}

void fdproc_add(int val){
    printk("AYOADD\n");
    disaggr_fdproc_set* set = &D_FDPROC;
    int hash_idx = val % NUM_FDPROC_BUCKETS;
    fdproc_node* curr = set->buckets[hash_idx];
    fdproc_node* node = fdnode_create(val);
    if(!curr){
        printk(KERN_INFO "BASECASE bucket=%d; pointer=%p;\n", hash_idx, node);
        printk_safe_flush();
        set->buckets[hash_idx] = node;
        return;
    }
    while(curr->next){
        printk(KERN_INFO "IT ALREADY EXISTS?\n");
        printk_safe_flush();
        curr = curr->next;
    }
    curr->next = node;
}

int fdproc_contains(int val){
    disaggr_fdproc_set* set = &D_FDPROC;
    int hash_idx = val % NUM_FDPROC_BUCKETS;
    fdproc_node* curr = set->buckets[hash_idx];
    while(curr){
        if(curr->val == val){
            printk(KERN_INFO "AYOCONTAINS\n");
            printk_safe_flush();
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void fdproc_remove(int val){
    disaggr_fdproc_set* set = &D_FDPROC;
    int hash_idx = val % NUM_FDPROC_BUCKETS;
    fdproc_node* curr = set->buckets[hash_idx];
    fdproc_node* last = curr;
    while(curr){
        if(curr->val == val){
            //It is the first element in the chaing
            if(last == curr){
                set->buckets[hash_idx] = NULL;
            } else {
                last->next = curr->next;
            }
            kfree(curr);
        }
        last = curr;
        curr = curr->next;
    }  
}