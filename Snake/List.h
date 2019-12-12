#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    struct Node* next_node;
    struct Node* prev_node;
    void* value; // 使用 void* 存下任何类型的指针
} ListNode;

typedef void destroy_func(void* value);

typedef struct {
    ListNode* head;
    ListNode* tail;
    int size;
    destroy_func* destroy; // 在删除列表时用于删除单个元素的函数，由用户在 init_list 函数中传入
} List;

void init_list(List* lst, destroy_func func);
ListNode* list_get(List* lst, int pos);
#define get_list_val(lst, pos) list_get(lst, pos)->value
void list_insert(List* lst, void* val, int pos); // 插入后元素处于 pos 位置
#define list_append(lst, val) list_insert((lst), (val), (lst)->size) // 插入末尾
void list_delete(List* lst, int pos);
void destroy_list(List* lst);
// 用于循环访问链表的宏
#define list_foreach(lst) for (ListNode* temp_list_iter = lst->head; temp_list_iter!= NULL; temp_list_iter = temp_list_iter->next_node)
#define list_iter(type) ((type*)(temp_list_iter->value))

#endif
