#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdbool.h>

struct Node {
    struct Node* next_node;
    struct Node* prev_node;
    void* value; // use void* to store any type of variables
};

typedef struct Node ListNode;

// type of a function that is called to destroy a list node when destroying a list
typedef void destroy_func(void* value);

typedef struct {
    ListNode* head;
    ListNode* tail;
    int size;
    destroy_func* destroy;
} List;

void init_list(List* lst, destroy_func func);
ListNode* list_get(List* lst, int pos); // get a node at pos
void list_append(List* lst, void* val);
void* get_list_val(List* lst, int pos);
void destroy_list(List* lst);
#define get_list_val(lst, pos) list_get(lst, pos)->value
#define list_foreach(lst) for (ListNode* temp_list_iter = lst->head; temp_list_iter!= NULL; temp_list_iter = temp_list_iter->next_node)
#define list_iter(type) ((type*)(temp_list_iter->value))

#endif
