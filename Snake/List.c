#include "List.h"

void list_init(List* lst, init_func* init, destroy_func* func) {
    lst->size = 0;
    lst->head = NULL;
    lst->tail = NULL;
    lst->destroy = func;
    lst->init = init;
}

ListIterator* list_node_get(List* lst, int pos) {
    if (pos >= lst->size || pos < 0) {
        fprintf(stderr, "Error: Unable to get a value at pos %d in a list of size %d.\n", pos, lst->size);
        return NULL;
    }
    ListIterator* cur;
    // 判断 pos 更靠近开头还是结尾，并选择从头部还是尾部考试遍历，提高效率
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

void list_insert(List* lst, int pos) {
    ListIterator* new_node = (ListIterator*)malloc(sizeof(ListIterator));
    new_node->value = lst->init();
    // pos 为 0 时找不到 pos - 1 位置，单独处理
    if (pos == 0) {
        new_node->next_node = lst->head;
        new_node->prev_node = NULL;
        if (lst->size == 0) {
            lst->tail = new_node;
        }
        if (lst->head != NULL) {
            lst->head->prev_node = new_node;
        }
        lst->head = new_node;
        lst->size ++;
        return ;
    }
    ListIterator* prev = list_node_get(lst, pos - 1);
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
    ListIterator* cur = list_node_get(lst, pos);
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

void list_destroy(List* lst) {
    ListIterator* cur = lst->head;
    while (cur != NULL) {
        ListIterator* to_destroy = cur;
        cur = cur->next_node;
        lst->destroy(to_destroy->value);
        free(to_destroy);
    }
    lst->head = lst->tail = NULL;
    free(lst);
}

