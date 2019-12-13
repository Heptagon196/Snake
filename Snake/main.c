#include "ConsoleIO.h"
#include "Menu.h"
#include "Snake.h"

char map_filename[256], rank_filename[256];

void load_config() {
    FILE* fp = fopen("data/config.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: config file not found.\n");
        exit(EXIT_FAILURE);
    }
    while (!feof(fp)) {
        char op[256], filename[256];
        // 读入可能含空格的字符串并舍去回车
        fscanf(fp, "%[^=]=%[^\n]%*c", op, filename);
        if (!strcmp(op, "map_file")) {
            strcpy(map_filename, filename);
        } else if (!strcmp(op, "rank_file")) {
            strcpy(rank_filename, filename);
        }
    }
    fclose(fp);
}

int main() {
    srand(time(NULL));
#if defined(linux) || defined(__APPLE__)
#else
    // Windows 下更改编码为 UTF-8 防止乱码
    SetConsoleOutputCP(65001);
#endif
    load_config();
    int option;
    while (true) {
        option = Menu(4, "SNAKE", "Start Game", "Edit Map", "Exit");
        if (option == -1 || option == 2) {
            clear_screen();
            return 0;
        }
        SnakeGameData* data = (SnakeGameData*)malloc(sizeof(SnakeGameData));
        init_snake_game_data(data, map_filename, rank_filename);
        if (option == 0) {
            start_snake_game(data);
            set_color(BLACK, WHITE);
            hide_cursor();
            move_cursor(1, 21);
            char* s = input_box(12, 2, "Game Over!", "Input your name:");
            if (s != NULL && strlen(s) != 0) {
                rank_add_data(data->score_record, data->score, s);
                save_rank(data->score_record, data->rank_filename);
            }
            show_cursor();
            destroy_snake_game_data(data);
        }
        if (option == 1) {
            edit_snake_map(data);
            save_snake_map(data);
            destroy_snake_game_data(data);
        }
        clear_screen();
    }
    return 0;
}
