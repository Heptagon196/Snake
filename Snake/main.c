#include "ConsoleIO.h"
#include "Menu.h"
#include "Snake.h"

char map_filename[256], rank_filename[256];
double snake_speed = 0.15;
int additional_food_lasting_time = 7;
int additional_food_generate_time = 20;
double eraser_possibility = 0.25;

// 读取配置文件
void load_config() {
    FILE* fp = fopen("data/config.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: config file not found.\n");
        exit(EXIT_FAILURE);
    }
    while (!feof(fp)) {
        char op[256], content[256];
        // 读入两个以 = 分隔的字符串并舍去末尾换行符
        fscanf(fp, "%[^=]=%[^\n]%*c", op, content);
        if (!strcmp(op, "map_file")) {
            strcpy(map_filename, content);
        } else if (!strcmp(op, "rank_file")) {
            strcpy(rank_filename, content);
        } else if (!strcmp(op, "snake_speed")) {
            double d;
            sscanf(content, "%lf", &d);
            snake_speed = d;
        } else if (!strcmp(op, "additional_food_lasting_time")) {
            int t;
            sscanf(content, "%d", &t);
            additional_food_lasting_time = t;
        } else if (!strcmp(op, "additional_food_generate_time")) {
            int t;
            sscanf(content, "%d", &t);
            additional_food_generate_time = t;
        } else if (!strcmp(op, "eraser_possibility")) {
            double p;
            sscanf(content, "%lf", &p);
            eraser_possibility = p;
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
    // 更改终端窗口及缓冲区大小
    int width = 81;
    int height = 21;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rect = {1, 1, width, height};
	SetConsoleWindowInfo(handle, true, &rect);
	COORD size = {width, height};
	SetConsoleScreenBufferSize(handle, size);
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
        init_snake_game_data(data, map_filename, rank_filename, snake_speed, additional_food_lasting_time, additional_food_generate_time, eraser_possibility);
        if (option == 0) {
            start_snake_game(data);
            set_color(BLACK, WHITE);
            hide_cursor();
            // 弹出输入框，未使用 esc 退出且输入非空时保存成绩
            char* s = input_box(12, 2, "Game Over!", "Input your name:");
            if (s != NULL) {
                if (strlen(s) != 0) {
                    rank_add_data(data->score_record, data->score, s);
                    save_rank(data->score_record, data->rank_filename);
                } else {
                    free(s);
                }
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
