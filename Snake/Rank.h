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

void rank_init(Rank* rk);
void rank_destroy(Rank* rk);
void rank_add_data(Rank* rk, int score, char* name);
void rank_sort(Rank* rk);
void rank_save(Rank* rk, const char* filename);
void rank_load(Rank* rk, const char* filename);

#endif
