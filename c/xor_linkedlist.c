#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


typedef struct xorNode
{
    int info;
    struct xorNode *xor_ptr;
}xor_node, *xor_node_ptr;

typedef struct xorLinkedList
{
    xor_node_ptr head, tail;
}xor_linkedlist, *xor_linkedlist_ptr;

//compute two pointer xor result
xor_node_ptr compute_xor_ptr(xor_node_ptr ptr1, xor_node_ptr ptr2)
{
    return (xor_node_ptr)((uintptr_t)ptr1 ^ (uintptr_t)ptr2);
}
//initial xorlinkedlist head, tail node
xor_linkedlist_ptr init_xorlinkedlist(xor_linkedlist_ptr list)
{
    xor_node_ptr temp = (xor_node_ptr)malloc(sizeof(xor_node));
    temp->xor_ptr = NULL;
    list->tail = list->head = temp;
    return list;
}

//create a xor_linkedlist sample
xor_linkedlist_ptr create_sample_xor_linkedlist(xor_linkedlist_ptr list)
{
    xor_node_ptr new_node = (xor_node_ptr)malloc(sizeof(xor_node));
    assert(new_node != NULL);
    
    scanf("%d", &(new_node->info));
    while (new_node->info)
    {
        new_node->xor_ptr = compute_xor_ptr(list->head, NULL);
        list->head->xor_ptr = compute_xor_ptr(new_node, compute_xor_ptr(NULL, list->head->xor_ptr));
        list->head = new_node;
        new_node = (xor_node_ptr)malloc(sizeof(xor_node));
        scanf("%d", &(new_node->info));
    }
    return list;
}

void out_xor_linkedlist(xor_linkedlist_ptr list)
{
    xor_node_ptr pre = NULL;
    xor_node_ptr cur = list->tail;
    xor_node_ptr next = NULL;

    while (cur)
    {
        printf("--%d--\n", cur->info);
        next = compute_xor_ptr(pre, cur->xor_ptr);
        pre = cur;
        cur = next;
    }
}


int main(int argc, const char *argv[])
{
    xor_linkedlist_ptr list = (xor_linkedlist_ptr)malloc(sizeof(xor_linkedlist));
    
    list = init_xorlinkedlist(list);
    list = create_sample_xor_linkedlist(list);
    out_xor_linkedlist(list);
    
    return 0;
}
