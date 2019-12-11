#ifndef SNAKE_H
#define SNAKE_H

#include <string.h>
#include "ConsoleIO.h"
#include "List.h"
#include "Block.h"

typedef struct {
    int x, y;
} SnakeBody;

#define SNAKE_MAP_WIDTH (SCREEN_WIDTH / 2)
#define SNAKE_MAP_HEIGHT (SCREEN_HEIGHT)

typedef struct {
    List* snake;
    List* block_types;
    const BlockType* game_map[SNAKE_MAP_WIDTH + 2][SNAKE_MAP_HEIGHT + 2];
    double speed;
    const char* filename;
} SnakeGameData;

void load_snake_map(SnakeGameData* data);
void save_snake_map(SnakeGameData* data);

void init_snake_game_data(SnakeGameData* data, const char* filename);
void destroy_snake_game_data(SnakeGameData* data);
void init_snake_game_map(SnakeGameData* data);

void start_snake_game(SnakeGameData* data);

void edit_snake_map(SnakeGameData* data);

#endif
