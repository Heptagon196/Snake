#include "Snake.h"

// random integer from 1 to m
int randint(int m) {
    return ((int)((double) rand() / RAND_MAX * (m - 1) + 0.5) + 1);
}

SnakeBody* new_snake_body(int x, int y) {
    SnakeBody* new_body = (SnakeBody*)malloc(sizeof(SnakeBody));
    new_body->x = x;
    new_body->y = y;
    return new_body;
}

void destroy_snake_body(void* val) {
    free((SnakeBody*)val);
}

void init_snake_game_data(SnakeGameData* data) {
    data->snake = (List*)malloc(sizeof(List));
    data->speed = 0.15;
    init_list(data->snake, destroy_snake_body);
    memset(data->game_map, 0, sizeof(data->game_map));
}

void destroy_snake_game_data(SnakeGameData* data) {
    destroy_list(data->snake);
    free(data);
}

void init_snake_game_map(SnakeGameData* data) {
    for (int i = 1; i <= SCREEN_WIDTH / 2; i ++) {
        data->game_map[i][1] = data->game_map[i][SCREEN_HEIGHT] = 1;
    }
    for (int i = 1; i <= SCREEN_HEIGHT; i ++) {
        data->game_map[1][i] = data->game_map[SCREEN_WIDTH / 2][i] = 1;
    }
}

int food_pos_x, food_pos_y;

void get_random_pos(SnakeGameData* data, int* x, int* y) {
    do {
        *x = randint(SCREEN_WIDTH / 2);
        *y = randint(SCREEN_HEIGHT);
    } while (data->game_map[*x][*y]);
}

void generate_food(SnakeGameData* data) {
    get_random_pos(data, &food_pos_x, &food_pos_y);
}

void add_snake_body(SnakeGameData* data, int x, int y) {
    SnakeBody* new_body = (SnakeBody*)malloc(sizeof(SnakeBody));
    new_body->x = x;
    new_body->y = y;
    data->game_map[x][y] = 1;
    list_append(data->snake, new_body);
}

void display_food() {
    move_cursor(food_pos_x, food_pos_y);
    set_color(WHITE, RED);
    puts("  ");
}

void display_body(SnakeBody* body) {
    move_cursor(body->x, body->y);
    set_color(WHITE, GREEN);
    puts("  ");
}

void display_head(SnakeBody* body) {
    move_cursor(body->x, body->y);
    set_color(WHITE, BLUE);
    puts("  ");
}

void display_wall(int x, int y) {
    move_cursor(x, y);
    set_color(WHITE, YELLOW);
    puts("  ");
}

void display_empty(int x, int y) {
    move_cursor(x, y);
    set_color(WHITE, WHITE);
    puts("  ");
}

#define SNAKEBODY_TAIL ((SnakeBody*)data->snake->tail->value)
#define SNAKEBODY_HEAD ((SnakeBody*)data->snake->head->value)

void move_head_to(SnakeGameData* data, int x, int y) {
    SnakeBody* tail = SNAKEBODY_TAIL;
    int hide_x = tail->x;
    int hide_y = tail->y;
    data->game_map[tail->x][tail->y] = 0;
    tail->x = x;
    tail->y = y;
    display_head(tail);
    data->game_map[tail->x][tail->y] = 1;
    if (data->snake->size > 1) {
        set_color(BLACK, WHITE);
        display_body(SNAKEBODY_HEAD);
        ListNode* next_tail_node = data->snake->tail->prev_node;
        ListNode* last_tail_node = data->snake->tail;
        data->snake->head->prev_node = data->snake->tail;
        data->snake->tail->prev_node->next_node = NULL;
        data->snake->tail->prev_node = NULL;
        data->snake->tail->next_node = data->snake->head;
        data->snake->tail = next_tail_node;
        data->snake->head = last_tail_node;
    }
    display_empty(hide_x, hide_y);
}

void start_snake_game(SnakeGameData* data) {
    clear_screen();
    hide_cursor();

    generate_food(data);

    int head_pos_x, head_pos_y;
    get_random_pos(data, &head_pos_x, &head_pos_y);
    add_snake_body(data, head_pos_x, head_pos_y);

    for (int i = 1; i <= SCREEN_WIDTH / 2; i ++) {
        for (int j = 1; j <= SCREEN_HEIGHT; j ++) {
            if (data->game_map[i][j]) {
                display_wall(i, j);
            }
        }
    }
    display_food();
    display_head(SNAKEBODY_HEAD);

    int ch = "wasd"[randint(4) - 1];
    int last_ch = ch;
    while (true) {
        move_cursor(1, 21);
        set_color(WHITE, WHITE);
        ch = read_in_seconds(data->speed);
#define OPPOSITE(a, b) (((a) == 'w' && (b) == 's') || ((a) == 'a' && (b) == 'd'))
        if (OPPOSITE(ch, last_ch) || OPPOSITE(last_ch, ch)) {
            ch = last_ch;
        }
#undef OPPOSITE
        if (ch == 0) {
            ch = last_ch;
        }
        last_ch = ch;
        if (ch == 'q') {
            break;
        }
        int backup_x = head_pos_x;
        int backup_y = head_pos_y;
        if (ch == 'a') {
            head_pos_x --;
        }
        if (ch == 'd') {
            head_pos_x ++;
        }
        if (ch == 's') {
            head_pos_y ++;
        }
        if (ch == 'w') {
            head_pos_y --;
        }
        if (data->game_map[head_pos_x][head_pos_y] == 1) {
            break;
        }
        if (head_pos_x != backup_x || head_pos_y != backup_y) {
            int last_tail_x = SNAKEBODY_TAIL->x;
            int last_tail_y = SNAKEBODY_TAIL->y;
            move_head_to(data, head_pos_x, head_pos_y);
            if (head_pos_x == food_pos_x && head_pos_y == food_pos_y) {
                add_snake_body(data, last_tail_x, last_tail_y);
                display_body((SnakeBody*)data->snake->tail->value);
                generate_food(data);
                display_food();
            }
        }
    }
    show_cursor();
}
