#include "List.h"
#include <stdio.h>

void init_list(List* lst, destroy_func* func) {
    lst->size = 0;
    lst->head = NULL;
    lst->tail = NULL;
    lst->destroy = func;
}

ListNode* list_get(List* lst, int pos) {
    if (pos >= lst->size || pos < 0) {
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

void list_insert(List* lst, void* val, int pos) {
    if (lst->size == 0) {
        if (pos != 0) {
            fprintf(stderr, "Unable to insert a value to pos %d in an empty list.\n", pos);
            return;
        }
        lst->head = (ListNode*)malloc(sizeof(ListNode));
        lst->tail = lst->head;
        lst->head->next_node = NULL;
        lst->head->prev_node = NULL;
        lst->head->value = val;
        lst->size ++;
        return ;
    }
    ListNode* new_node = (ListNode*)malloc(sizeof(ListNode));
    new_node->value = val;
    if (pos == 0) {
        new_node->next_node = lst->head;
        new_node->prev_node = NULL;
        lst->head->prev_node = new_node;
        lst->head = new_node;
        lst->size ++;
        return ;
    }
    ListNode* prev = list_get(lst, pos - 1);
    new_node->next_node = prev->next_node;
    new_node->prev_node = prev;
    if (prev->next_node != NULL) {
        prev->next_node->prev_node = new_node;
    } else {
        lst->tail = new_node;
    }
    prev->next_node = new_node;
    lst->size ++;
}

void list_delete(List* lst, int pos) {
    ListNode* cur = list_get(lst, pos);
    if (cur->prev_node != NULL) {
        cur->prev_node->next_node = cur->next_node;
    } else {
        lst->head = cur->next_node;
    }
    if (cur->next_node != NULL) {
        cur->next_node->prev_node = cur->prev_node;
    } else {
        lst->tail = cur->prev_node;
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

