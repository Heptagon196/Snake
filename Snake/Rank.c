#include "Rank.h"

void destroy_rank_data(void* val) {
    free(((RankData*)val)->name);
    free((RankData*)val);
}

void* new_rank_data() {
    return malloc(sizeof(RankData));
}

void rank_init(Rank* rk) {
    rk->scores = (List*)malloc(sizeof(List));
    list_init(rk->scores, new_rank_data, destroy_rank_data);
}

void rank_destroy(Rank* rk) {
    list_destroy(rk->scores);
    free(rk);
}

void rank_add_data(Rank* rk, int score, char* name) {
    list_append(rk->scores);
    list_tail(rk->scores, RankData)->score = score;
    list_tail(rk->scores, RankData)->name = name;
}

#define score(a) ((RankData*)a->value)->score
#define name(a) ((RankData*)a->value)->name
// 冒泡排序
void rank_sort(Rank* rk) {
    for (int i = 0; i < rk->scores->size - 1; i ++) {
        ListIterator* a = rk->scores->head;
        for (int j = 0; j < rk->scores->size - 1 - i; j ++) {
            ListIterator* b = a->next_node;
            if (score(a) < score(b)) {
                // 交换
                int tmp_score = score(b);
                char* tmp_name = name(b);
                score(b) = score(a);
                name(b) = name(a);
                score(a) = tmp_score;
                name(a) = tmp_name;
            }
            a = a->next_node;
        }
    }
}

void rank_save(Rank* rk, const char* filename) {
    FILE* fp = fopen(filename, "w");
    for (ListIterator* i = rk->scores->head; i != NULL; i = i->next_node) {
        fprintf(fp, "%s\n%d\n", name(i), score(i));
    }
    fclose(fp);
}

void rank_load(Rank* rk, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return ;
    }
    while (!feof(fp)) {
        char s[256];
        if (fgets(s, 255, fp) == NULL) {
            break;
        }
        s[strlen(s) - 1] = '\0'; // 舍去末尾换行
        char* new_string = (char*)calloc(strlen(s) + 1, sizeof(char));
        strcpy(new_string, s);
        int score;
        if (fgets(s, 255, fp) == NULL) {
            free(new_string);
            break;
        }
        sscanf(s, "%d", &score);
        rank_add_data(rk, score, new_string);
    }
    fclose(fp);
    rank_sort(rk);
}
