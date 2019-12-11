#include "ConsoleIO.h"
#include "Menu.h"
#include "Snake.h"

int main() {
    srand(time(NULL));
    int option =  Menu(4, "SNAKE", "Start Game", "Edit Map", "Exit");
    if (option == -1 || option == 2) {
        clear_screen();
        return 0;
    }
    SnakeGameData* data = (SnakeGameData*)malloc(sizeof(SnakeGameData));
    init_snake_game_data(data, "snake.map");
    init_snake_game_map(data);
    if (option == 0) {
        start_snake_game(data);
        destroy_snake_game_data(data);
        set_color(BLACK, WHITE);
        hide_cursor();
        puts("Game Over! Press any key to exit.");
        getch();
        show_cursor();
    }
    if (option == 1) {
        edit_snake_map(data);
        save_snake_map(data);
        destroy_snake_game_data(data);
    }
    clear_screen();
    return 0;
}
