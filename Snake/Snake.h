#ifndef SNAKE_H
#define SNAKE_H

#include <string.h>
#include "ConsoleIO.h"
#include "List.h"

typedef struct {
    int x, y;
} SnakeBody;

typedef struct {
    List* snake;
    int game_map[SCREEN_WIDTH / 2 + 2][SCREEN_HEIGHT + 2];
    double speed;
} SnakeGameData;

void init_snake_game_data(SnakeGameData* data);
void destroy_snake_game_data(SnakeGameData* data);
void init_snake_game_map(SnakeGameData* data);
void start_snake_game(SnakeGameData* data);

#endif
