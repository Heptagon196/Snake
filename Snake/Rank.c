#include "Rank.h"

void destroy_rank_data(void* val) {
    free(((RankData*)val)->name);
    free((RankData*)val);
}

void init_rank(Rank* rk) {
    rk->scores = (List*)malloc(sizeof(List));
    init_list(rk->scores, destroy_rank_data);
}

void destroy_rank(Rank* rk) {
    destroy_list(rk->scores);
    free(rk);
}

void rank_add_data(Rank* rk, int score, char* name) {
    RankData* data = (RankData*)malloc(sizeof(RankData));
    data->score = score;
    data->name = name;
    list_append(rk->scores, data);
}

#define score(a) ((RankData*)a->value)->score
#define name(a) ((RankData*)a->value)->name
void rank_sort(Rank* rk) {
    for (ListNode* a = rk->scores->head; a != NULL; a = a->next_node) {
        for (ListNode* b = a->next_node; b != NULL; b = b->next_node) {
            if (score(a) < score(b)) {
                int tmp_score = score(b);
                char* tmp_name = name(b);
                score(b) = score(a);
                name(b) = name(a);
                score(a) = tmp_score;
                name(a) = tmp_name;
            }
        }
    }
}

void save_rank(Rank* rk, const char* filename) {
    FILE* fp = fopen(filename, "w");
    for (ListNode* i = rk->scores->head; i != NULL; i = i->next_node) {
        fprintf(fp, "%s\n%d\n", name(i), score(i));
    }
    fclose(fp);
}

void load_rank(Rank* rk, const char* filename) {
    FILE* fp = fopen(filename, "r");
    while (!feof(fp)) {
        char s[256];
        if (fgets(s, 255, fp) == NULL) {
            break;
        }
        s[strlen(s) - 1] = '\0';
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
