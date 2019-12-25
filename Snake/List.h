#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    struct Node* next_node;
    struct Node* prev_node;
    void* value; // 使用 void* 存下任何类型的指针
} ListIterator;

typedef void destroy_func(void* value);
typedef void* init_func();

typedef struct {
    ListIterator* head;
    ListIterator* tail;
    int size;
    destroy_func* destroy; // 在删除列表时用于删除单个元素的函数，由用户在 init_list 函数中传入
    init_func* init; // 初始化单个元素的函数
} List;

void list_init(List* lst, init_func* init, destroy_func* func);
ListIterator* list_node_get(List* lst, int pos); // 获取第 pos 个位置的 ListIterator
void list_insert(List* lst, int pos); // 插入一个空元素，插入后元素处于 pos 位置
void list_delete(List* lst, int pos); // 删除第 pos 个元素
void list_destroy(List* lst); // 删除链表

#define list_at(lst, pos, type) ((type*)list_node_get(lst, pos)->value)
#define list_head(lst, type) ((type*)(lst)->head->value)
#define list_tail(lst, type) ((type*)(lst)->tail->value)
#define list_append(lst) list_insert((lst), (lst)->size) // 插入末尾
#define list_pop(lst) list_delete((lst), (lst)->size - 1) // 删除末尾元素

#endif
