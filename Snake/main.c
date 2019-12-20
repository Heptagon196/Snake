#include "ConsoleIO.h"
#include "Menu.h"
#include "Snake.h"
#include "Config.h"

#define CONFIG_FILENAME "data/config.txt"

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
    int option;
    while (true) {
        option = Menu(5, 0, "SNAKE", (const char*[]){"Start Game", "Edit Map", "Settings", "About", "Exit"});
        if (option == -1 || option == 4) {
            clear_screen();
            return 0;
        }
        SnakeGameData* data = (SnakeGameData*)malloc(sizeof(SnakeGameData));
        init_snake_game_data(data, CONFIG_FILENAME);
        if (option == 0) {
            start_snake_game(data);
            set_color(BLACK, WHITE);
            hide_cursor();
            // 弹出输入框，未使用 esc 退出且输入非空时保存成绩
            char* s = input_box(12, 2, "", (const char*[]){"Game Over!", "Input your name:"});
            if (s != NULL) {
                if (strlen(s) != 0) {
                    rank_add_data(data->score_record, data->score, s);
                    save_rank(data->score_record, data->rank_filename);
                } else {
                    free(s);
                }
            }
            show_cursor();
        }
        if (option == 1) {
            edit_snake_map(data);
            save_snake_map(data);
        }
        if (option == 2) {
            edit_config(data);
            save_config(data, CONFIG_FILENAME);
        }
        if (option == 3) {
            set_color(BLACK, WHITE);
            puts("");
            clear_screen();
            hide_cursor();
            print_box(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);
            print_to_middle("Snake", SCREEN_HEIGHT / 2 - 1, 1);
            print_to_middle("by heptagon196", SCREEN_HEIGHT / 2 + 1, 1);
            getch();
        }
        destroy_snake_game_data(data);
        clear_screen();
    }
    return 0;
}
