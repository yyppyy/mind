#include <linux/slab.h>
#include "disaggr_fdset.h"


disaggr_fdset D_FDSET;

fdset_node* fdnode_create(int val){
    fdset_node *node = kmalloc(sizeof(fdset_node), GFP_KERNEL);
    node->val = val;
    node->next = NULL;
    return node;
}

// disaggr_fdset* fdset_create(void){
//     printk("AYO CREATE\n");
//     printk_safe_flush();
//     disaggr_fdset* fdset = kmalloc(sizeof(disaggr_fdset), GFP_KERNEL);
//     if(!fdset){
//         return NULL;
//     }
//     fdset->num_buckets = 8;
//     fdset-> buckets = kmalloc(fdset->num_buckets * sizeof(int*), GFP_KERNEL);
//     int i;
//     for(i = 0; i < fdset->num_buckets; ++i){
//         fdset->buckets[i] = NULL;
//     }
//     return fdset;
// }



void fdset_add(int val){
    printk("AYOADD\n");
    printk_safe_flush();
    disaggr_fdset* set = &D_FDSET;
    int hash_idx = val % NUM_BUCKETS;
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

int fdset_contains(int val){
    disaggr_fdset* set = &D_FDSET;
    int hash_idx = val % NUM_BUCKETS;
    fdset_node* curr = set->buckets[hash_idx];
    while(curr){
        if(curr->val == val){
            printk("AYOCONTAINS\n");
            printk_safe_flush();
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void fdset_remove(int val){
    // printk("AYO REMOVE\n");
    // printk_safe_flush();
    disaggr_fdset* set = &D_FDSET;
    int hash_idx = val % NUM_BUCKETS;
    fdset_node* curr = set->buckets[hash_idx];
    fdset_node* last = curr;
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