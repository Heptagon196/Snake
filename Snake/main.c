#include "ConsoleIO.h"
#include "Menu.h"
#include "Snake.h"

int main() {
    srand(time(NULL));
    int option =  Menu(3, "SNAKE", "Start", "Exit");
    if (option == -1 || option == 2) {
        clear_screen();
        return 0;
    }
    if (option == 0) {
        SnakeGameData* data = (SnakeGameData*)malloc(sizeof(SnakeGameData));
        init_snake_game_data(data);
        init_snake_game_map(data);
        start_snake_game(data);
        destroy_snake_game_data(data);
        set_color(BLACK, WHITE);
        hide_cursor();
        puts("Game Over! Press any key to exit.");
        getch();
        show_cursor();
    }
    clear_screen();
    return 0;
}
