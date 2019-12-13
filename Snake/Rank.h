#ifndef RANK_H
#define RANK_H

#include "List.h"
#include "InputBox.h"

typedef struct {
    int score;
    char* name;
} RankData;

typedef struct {
    List* scores;
} Rank;

void init_rank(Rank* rk);
void destroy_rank(Rank* rk);
void rank_add_data(Rank* rk, int score, char* name);
void rank_sort(Rank* rk);
void save_rank(Rank* rk, const char* filename);
void load_rank(Rank* rk, const char* filename);

#endif
