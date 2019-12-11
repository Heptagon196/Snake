#include "Snake.h"

const BlockType empty_block = {WHITE, WHITE, "  "};
const BlockType wall_block = {WHITE, YELLOW, "  "};

// random_portal_block 随机传送
// portal_block 定向传送
#if defined(linux) || defined(__APPLE__)
const BlockType random_portal_block = {PURPLE, WHITE, "◆ "};
const BlockType portal_block = {CYAN, WHITE, "◆ "};
const BlockType food_block = {RED, WHITE, "● "};
#else
const BlockType random_portal_block = {PURPLE, WHITE, "◆"};
const BlockType portal_block = {CYAN, WHITE, "◆"};
const BlockType food_block = {RED, WHITE, "●"};
#endif

const BlockType snake_head_block = {WHITE, BLUE, "  "};
const BlockType snake_body_block = {WHITE, GREEN, "  "};

// random integer from 1 to m
int randint(int m) {
    return ((int)((double) rand() / RAND_MAX * (m - 1) + 0.5) + 1);
}

Pos* new_snake_body(int x, int y) {
    Pos* new_body = (Pos*)malloc(sizeof(Pos));
    new_body->x = x;
    new_body->y = y;
    return new_body;
}

void destroy_pos(void* val) {
    free((Pos*)val);
}

void load_snake_map(SnakeGameData* data) {
    FILE* fp = fopen(data->filename, "r");
    while (!feof(fp)) {
        int operation;
        fscanf(fp, "%d", &operation);
        if (operation == 1) {
            int x, y;
            fscanf(fp, "%d %d", &x, &y);
            data->game_map[x][y] = &wall_block;
        }
        if (operation == 2) {
            int x, y;
            fscanf(fp, "%d %d", &x, &y);
            data->game_map[x][y] = &random_portal_block;
            Pos* new_portal = (Pos*)malloc(sizeof(Pos));
            new_portal->x = x;
            new_portal->y = y;
            list_append(data->random_portals, new_portal);
        }
        if (operation == 3) {
            int x1, y1, x2, y2;
            fscanf(fp, "%d %d %d %d", &x1, &y1, &x2, &y2);
            data->game_map[x1][y1] = data->game_map[x2][y2] = &portal_block;
            data->transport_to[x1][y1] = (Pos){x2, y2};
            data->transport_to[x2][y2] = (Pos){x1, y1};
        }
    }
    fclose(fp);
}

void save_snake_map(SnakeGameData* data) {
    FILE* fp = fopen(data->filename, "w");
    for (int i = 1; i <= SNAKE_MAP_WIDTH; i ++) {
        for (int j = 1; j <= SNAKE_MAP_HEIGHT; j ++) {
            if (data->game_map[i][j] == &wall_block) {
                fprintf(fp, "1 %d %d\n", i, j);
            }
            if (data->game_map[i][j] == &random_portal_block) {
                fprintf(fp, "2 %d %d\n", i, j);
            }
            if (data->game_map[i][j] == &portal_block) {
                fprintf(fp, "3 %d %d %d %d\n", i, j, data->transport_to[i][j].x, data->transport_to[i][j].y);
            }
        }
    }
    fclose(fp);
}

void init_snake_game_data(SnakeGameData* data, const char* filename) {
    data->snake = (List*)malloc(sizeof(List));
    data->random_portals = (List*)malloc(sizeof(List));
    data->speed = 0.15;
    data->filename = filename;
    init_list(data->snake, destroy_pos);
    init_list(data->random_portals, destroy_pos);
    for (int i = 0; i < SNAKE_MAP_WIDTH + 2; i ++) {
        for (int j = 0; j < SNAKE_MAP_HEIGHT + 2; j ++) {
            data->game_map[i][j] = &empty_block;
        }
    }
    memset(data->transport_to, 0, sizeof(data->transport_to));
}

void destroy_snake_game_data(SnakeGameData* data) {
    destroy_list(data->snake);
    destroy_list(data->random_portals);
    free(data);
}

void init_snake_game_map(SnakeGameData* data) {
    for (int i = 1; i <= SNAKE_MAP_WIDTH; i ++) {
        data->game_map[i][1] = data->game_map[i][SNAKE_MAP_HEIGHT] = &wall_block;
    }
    for (int i = 1; i <= SNAKE_MAP_HEIGHT; i ++) {
        data->game_map[1][i] = data->game_map[SNAKE_MAP_WIDTH][i] = &wall_block;
    }
    load_snake_map(data);
}

int food_pos_x, food_pos_y;

void get_random_pos(SnakeGameData* data, int* x, int* y) {
    do {
        *x = randint(SNAKE_MAP_WIDTH);
        *y = randint(SNAKE_MAP_HEIGHT);
    } while (data->game_map[*x][*y] != &empty_block);
}

void generate_food(SnakeGameData* data) {
    get_random_pos(data, &food_pos_x, &food_pos_y);
}

void add_snake_body(SnakeGameData* data, int x, int y) {
    Pos* new_body = (Pos*)malloc(sizeof(Pos));
    new_body->x = x;
    new_body->y = y;
    data->game_map[x][y] = &wall_block;
    list_append(data->snake, new_body);
}

#define SNAKEBODY_TAIL ((Pos*)data->snake->tail->value)
#define SNAKEBODY_HEAD ((Pos*)data->snake->head->value)

void move_head_to(SnakeGameData* data, int x, int y) {
    Pos* tail = SNAKEBODY_TAIL;
    int hide_x = tail->x;
    int hide_y = tail->y;
    data->game_map[tail->x][tail->y] = &empty_block;
    tail->x = x;
    tail->y = y;
    show_block(&snake_head_block, tail->x, tail->y);
    data->game_map[tail->x][tail->y] = &wall_block;
    if (data->snake->size > 1) {
        set_color(BLACK, WHITE);
        show_block(&snake_body_block, SNAKEBODY_HEAD->x, SNAKEBODY_HEAD->y);
        ListNode* next_tail_node = data->snake->tail->prev_node;
        ListNode* last_tail_node = data->snake->tail;
        data->snake->head->prev_node = data->snake->tail;
        data->snake->tail->prev_node->next_node = NULL;
        data->snake->tail->prev_node = NULL;
        data->snake->tail->next_node = data->snake->head;
        data->snake->tail = next_tail_node;
        data->snake->head = last_tail_node;
    }
    show_block(data->game_map[hide_x][hide_y], hide_x, hide_y);
}

void start_snake_game(SnakeGameData* data) {
    clear_screen();
    hide_cursor();

    generate_food(data);

    int head_pos_x, head_pos_y;
    get_random_pos(data, &head_pos_x, &head_pos_y);
    add_snake_body(data, head_pos_x, head_pos_y);

    for (int i = 1; i <= SNAKE_MAP_WIDTH; i ++) {
        for (int j = 1; j <= SNAKE_MAP_HEIGHT; j ++) {
            show_block(data->game_map[i][j], i, j);
        }
    }
    show_block(&food_block, food_pos_x, food_pos_y);
    show_block(&snake_head_block, SNAKEBODY_HEAD->x, SNAKEBODY_HEAD->y);

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
        START_MOVE:;
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
        if (data->game_map[head_pos_x][head_pos_y] == &wall_block) {
            break;
        }
        if (data->game_map[head_pos_x][head_pos_y] == &random_portal_block) {
            Pos* transport_to = (Pos*)get_list_val(data->random_portals, randint(data->random_portals->size) - 1);
            head_pos_x = transport_to->x;
            head_pos_y = transport_to->y;
            goto START_MOVE;
        }
        if (data->game_map[head_pos_x][head_pos_y] == &portal_block) {
            Pos transport_to = data->transport_to[head_pos_x][head_pos_y];
            head_pos_x = transport_to.x;
            head_pos_y = transport_to.y;
            goto START_MOVE;
        }
        if (head_pos_x != backup_x || head_pos_y != backup_y) {
            int last_tail_x = SNAKEBODY_TAIL->x;
            int last_tail_y = SNAKEBODY_TAIL->y;
            move_head_to(data, head_pos_x, head_pos_y);
            if (head_pos_x == food_pos_x && head_pos_y == food_pos_y) {
                add_snake_body(data, last_tail_x, last_tail_y);
                show_block(&snake_body_block, SNAKEBODY_TAIL->x, SNAKEBODY_TAIL->y);
                generate_food(data);
                show_block(&food_block, food_pos_x, food_pos_y);
            }
        }
    }
    show_cursor();
}

void display_selected(int x, int y) {
    move_cursor(x, y);
    set_color(WHITE, BLUE);
    puts("  ");
}

void edit_snake_map(SnakeGameData* data) {
    clear_screen();
    hide_cursor();

    int pos_x = 1, pos_y = 1;

    for (int i = 1; i <= SNAKE_MAP_WIDTH; i ++) {
        for (int j = 1; j <= SNAKE_MAP_HEIGHT; j ++) {
            show_block(data->game_map[i][j], i, j);
        }
    }
    show_block(&snake_head_block, pos_x, pos_y);

    Pos portal = {-1, -1};

    int ch;
    while (true) {
        move_cursor(1, 21);
        set_color(WHITE, WHITE);
        ch = getch();
        if (ch == 'q') {
            break;
        }
        int backup_x = pos_x;
        int backup_y = pos_y;
        if (ch == 'a') {
            pos_x --;
        }
        if (ch == 'd') {
            pos_x ++;
        }
        if (ch == 's') {
            pos_y ++;
        }
        if (ch == 'w') {
            pos_y --;
        }
        if (pos_x == 0) {
            pos_x = SNAKE_MAP_WIDTH;
        }
        if (pos_x == SNAKE_MAP_WIDTH + 1) {
            pos_x = 1;
        }
        if (pos_y == 0) {
            pos_y = SNAKE_MAP_HEIGHT;
        }
        if (pos_y == SNAKE_MAP_HEIGHT + 1) {
            pos_y = 1;
        }
        if (ch == 'h') {
            for (int i = 2; i <= pos_x - 1; i ++) {
                data->game_map[i][pos_y] = data->game_map[pos_x][pos_y];
                show_block(data->game_map[i][pos_y], i, pos_y);
            }
        }
        if (ch == 'l') {
            for (int i = pos_x + 1; i <= SNAKE_MAP_WIDTH - 1; i ++) {
                data->game_map[i][pos_y] = data->game_map[pos_x][pos_y];
                show_block(data->game_map[i][pos_y], i, pos_y);
            }
        }
        if (ch == 'k') {
            for (int i = 2; i <= pos_y - 1; i ++) {
                data->game_map[pos_x][i] = data->game_map[pos_x][pos_y];
                show_block(data->game_map[pos_x][i], pos_x, i);
            }
        }
        if (ch == 'j') {
            for (int i = pos_y + 1; i <= SNAKE_MAP_HEIGHT - 1; i ++) {
                data->game_map[pos_x][i] = data->game_map[pos_x][pos_y];
                show_block(data->game_map[pos_x][i], pos_x, i);
            }
        }
        show_block(data->game_map[backup_x][backup_y], backup_x, backup_y);
        show_block(&snake_head_block, pos_x, pos_y);
        if (ch == '0') {
            data->game_map[pos_x][pos_y] = &empty_block;
        } else if (ch == '1') {
            data->game_map[pos_x][pos_y] = &wall_block;
        } else if (ch == '2') {
            data->game_map[pos_x][pos_y] = &random_portal_block;
        } else if (ch == '3') {
            data->game_map[pos_x][pos_y] = &portal_block;
            if (portal.x == -1 && portal.y == -1) {
                portal.x = pos_x;
                portal.y = pos_y;
            } else {
                data->transport_to[portal.x][portal.y] = (Pos){pos_x, pos_y};
                data->transport_to[pos_x][pos_y] = (Pos){portal.x, portal.y};
                portal.x = -1;
                portal.y = -1;
            }
        } else {
            continue;
        }
        show_block(data->game_map[pos_x][pos_y], pos_x, pos_y);
    }
    show_cursor();
}
