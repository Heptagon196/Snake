#ifndef SNAKE_H
#define SNAKE_H

#include <string.h>
#include "ConsoleIO.h"
#include "List.h"
#include "Block.h"
#include "Rank.h"

typedef struct {
    int x, y;
} Pos;

#define SNAKE_MAP_WIDTH (SCREEN_WIDTH / 2 - 11)
#define SNAKE_MAP_HEIGHT (SCREEN_HEIGHT)

typedef struct {
    List* snake; // 贪吃蛇各节身体位置
    List* random_portals; // 所有随机传送门位置
    const BlockType* game_map[SNAKE_MAP_WIDTH + 2][SNAKE_MAP_HEIGHT + 2];
    Pos transport_to[SNAKE_MAP_WIDTH + 2][SNAKE_MAP_HEIGHT + 2]; // 定向传送门
    double speed; // 控制游戏速度，蛇每秒移动格数
    int additional_food_state; // 用于处理额外食物的闪烁
    Pos additional_food_pos; // 额外食物位置
    int additional_food_lasting_time; // 额外食物持续时间
    int additional_food_generate_time; // 额外食物生成间隔时间
    double eraser_possibility; // eraser 伴随食物生成的概率
    int score; // 得分
    Rank* score_record; // 排行榜
    char map_filename[256]; // 地图文件文件名
    char rank_filename[256]; // 排行榜文件名
} SnakeGameData;

void load_snake_map(SnakeGameData* data);
void save_snake_map(SnakeGameData* data);

void init_snake_game_data(SnakeGameData* data, const char* config_filename);
void destroy_snake_game_data(SnakeGameData* data);

void start_snake_game(SnakeGameData* data);

void edit_snake_map(SnakeGameData* data);

#endif
