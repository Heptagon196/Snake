#include "List.h"
#include <stdio.h>

void init_list(List* lst, destroy_func* func) {
    lst->size = 0;
    lst->head = NULL;
    lst->tail = NULL;
    lst->destroy = func;
}

ListNode* list_get(List* lst, int pos) {
    if (pos >= lst->size) {
        fprintf(stderr, "Error: Unable to get a value at pos %d in a list of size %d.\n", pos, lst->size);
        return NULL;
    }
    ListNode* cur;
    if (pos < lst->size / 2) {
        cur = lst->head;
        for (int i = 0; i < pos; i ++) {
            cur = cur->next_node;
        }
    } else {
        cur = lst->tail;
        for (int i = lst->size - 1; i > pos; i --) {
            cur = cur->prev_node;
        }
    }
    return cur;
}

void list_append(List* lst, void* val) {
    if (lst->size == 0) {
        lst->head = (ListNode*)malloc(sizeof(ListNode));
        lst->tail = lst->head;
        lst->head->next_node = NULL;
        lst->head->prev_node = NULL;
        lst->head->value = val;
        lst->size ++;
        return;
    }
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    new_node->next_node = NULL;
    new_node->prev_node = lst->tail;
    new_node->value = val;
    lst->tail->next_node = new_node;
    lst->tail = new_node;
    lst->size ++;
}

void list_delete(List* lst, int pos) {
    ListNode* cur = list_get(lst, pos);
    if (cur->prev_node != NULL) {
        cur->prev_node->next_node = cur->next_node;
    }
    if (cur->next_node != NULL) {
        cur->next_node->prev_node = cur->prev_node;
    }
    lst->destroy(cur->value);
    lst->size --;
    free(cur);
}

void destroy_list(List* lst) {
    ListNode* cur = lst->head;
    while (cur != NULL) {
        ListNode* to_destroy = cur;
        cur = cur->next_node;
        lst->destroy(to_destroy->value);
        free(to_destroy);
    }
    lst->head = lst->tail = NULL;
    free(lst);
}

