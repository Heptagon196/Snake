#include "ConsoleIO.h"
#include "Menu.h"
#include "Snake.h"

int main() {
    srand(time(NULL));
#if defined(linux) || defined(__APPLE__)
#else
    // Windows 下更改编码为 UTF-8 防止乱码
    SetConsoleOutputCP(65001);
#endif
    int option;
    while (true) {
        option = Menu(4, "SNAKE", "Start Game", "Edit Map", "Exit");
        if (option == -1 || option == 2) {
            clear_screen();
            return 0;
        }
        SnakeGameData* data = (SnakeGameData*)malloc(sizeof(SnakeGameData));
        init_snake_game_data(data, "data/snake_map.txt", "data/rank.txt");
        if (option == 0) {
            start_snake_game(data);
            set_color(BLACK, WHITE);
            hide_cursor();
            move_cursor(1, 21);
            puts("Game Over! Press 'S' to save your score, or press another key to exit.");
            int ch = getch();
            if (ch == 's' || ch == 'S') {
                rank_add_data(data->score_record, data->score, input_box("Input your name:", 12));
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
