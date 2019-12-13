#include "Snake.h"

#define min(a, b) ((a) > (b) ? (b) : (a))

const BlockType empty_block = {WHITE, WHITE, "  "};
const BlockType wall_block = {WHITE, YELLOW, "  "};

// random_portal_block 随机传送
// portal_block 定向传送
#if defined(linux) || defined(__APPLE__)
const BlockType random_portal_block = {MAGENTA, WHITE, "◆ "};
const BlockType portal_block = {CYAN, WHITE, "◆ "};
const BlockType food_block = {RED, WHITE, "● "};
const BlockType eraser_block = {LIGHT_BLUE, WHITE, "▲ "};
const BlockType additional_food_block[3] = {{LIGHT_RED, WHITE, "★ "}, {LIGHT_BLUE, WHITE, "★ "}, {LIGHT_GREEN, WHITE, "★ "}};
#else
const BlockType random_portal_block = {MAGENTA, WHITE, "◆"};
const BlockType portal_block = {CYAN, WHITE, "◆"};
const BlockType food_block = {RED, WHITE, "●"};
const BlockType eraser_block = {LIGHT_BLUE, WHITE, "▲"};
const BlockType additional_food_block[3] = {{LIGHT_RED, WHITE, "★"}, {LIGHT_BLUE, WHITE, "★"}, {LIGHT_GREEN, WHITE, "★"}};
#endif

const BlockType snake_head_block = {WHITE, BLUE, "  "};
const BlockType snake_body_block = {WHITE, GREEN, "  "};

int randint(int m) {
    return rand() % m;
}

// 返回给定时间内按下的键，如果没有按键，返回缓冲区最后一个按键（缓冲区无按键返回 0)
int read_in_seconds(double lasting_time) {
    double last_time_point = get_time();
    char last_ch = 0;
    while (kbhit()) {
        last_ch = getch();
    }
    while (!kbhit() && (get_time() - last_time_point < lasting_time)) {
        Sleep(20);
    }
    if (!kbhit()) {
        return last_ch;
    }
    while (get_time() - last_time_point < lasting_time) {
        Sleep(20);
    }
    return getch();
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
    FILE* fp = fopen(data->map_filename, "r");
    if (fp == NULL) {
        return ;
    }
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
    FILE* fp = fopen(data->map_filename, "w");
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

void init_snake_game_data(SnakeGameData* data, const char* map_filename, const char* rank_filename) {
    data->speed = 0.15;
    data->map_filename = map_filename;
    data->rank_filename = rank_filename;
    data->additional_food_state = 0;
    data->additional_food_pos.x = -1;
    data->additional_food_lasting_time = 7;
    data->additional_food_generate_time = 20;
    data->score = 0;

    data->snake = (List*)malloc(sizeof(List));
    init_list(data->snake, destroy_pos);

    data->random_portals = (List*)malloc(sizeof(List));
    init_list(data->random_portals, destroy_pos);

    data->score_record = (Rank*)malloc(sizeof(Rank));
    init_rank(data->score_record);

    for (int i = 0; i < SNAKE_MAP_WIDTH + 2; i ++) {
        for (int j = 0; j < SNAKE_MAP_HEIGHT + 2; j ++) {
            data->game_map[i][j] = &empty_block;
        }
    }
    memset(data->transport_to, 0, sizeof(data->transport_to));

    // 读取地图
    for (int i = 1; i <= SNAKE_MAP_WIDTH; i ++) {
        data->game_map[i][1] = data->game_map[i][SNAKE_MAP_HEIGHT] = &wall_block;
    }
    for (int i = 1; i <= SNAKE_MAP_HEIGHT; i ++) {
        data->game_map[1][i] = data->game_map[SNAKE_MAP_WIDTH][i] = &wall_block;
    }
    load_snake_map(data);

    // 读取 rank
    load_rank(data->score_record, data->rank_filename);
}

void destroy_snake_game_data(SnakeGameData* data) {
    destroy_list(data->snake);
    destroy_list(data->random_portals);
    destroy_rank(data->score_record);
    free(data);
}

Pos get_random_pos(SnakeGameData* data) {
    Pos ans;
    do {
        ans.x = randint(SNAKE_MAP_WIDTH) + 1;
        ans.y = randint(SNAKE_MAP_HEIGHT) + 1;
    } while (data->game_map[ans.x][ans.y] != &empty_block);
    return ans;
}

void generate_additional_food(SnakeGameData* data) {
    data->additional_food_pos = get_random_pos(data);
    data->game_map[data->additional_food_pos.x][data->additional_food_pos.y] = &additional_food_block[0];
    show_block(&additional_food_block[0], data->additional_food_pos.x, data->additional_food_pos.y);
}

void generate_eraser(SnakeGameData* data) {
    Pos p = get_random_pos(data);
    data->game_map[p.x][p.y] = &eraser_block;
    show_block(&eraser_block, p.x, p.y);
}

void generate_food(SnakeGameData* data) {
    Pos p = get_random_pos(data);
    data->game_map[p.x][p.y] = &food_block;
    show_block(&food_block, p.x, p.y);
    if (!randint(5)) {
        generate_eraser(data);
    }
}

#define SNAKEBODY_TAIL ((Pos*)data->snake->tail->value)
#define SNAKEBODY_HEAD ((Pos*)data->snake->head->value)

void add_snake_body(SnakeGameData* data, int x, int y) {
    Pos* new_body = (Pos*)malloc(sizeof(Pos));
    new_body->x = x;
    new_body->y = y;
    data->game_map[x][y] = &wall_block;
    list_append(data->snake, new_body);
    show_block(&snake_body_block, x, y);
}

void pop_snake_body(SnakeGameData* data) {
    data->game_map[SNAKEBODY_TAIL->x][SNAKEBODY_TAIL->y] = &empty_block;
    show_block(&empty_block, SNAKEBODY_TAIL->x, SNAKEBODY_TAIL->y);
    set_color(BLACK, WHITE);
    list_delete(data->snake, data->snake->size - 1);
}

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

#define PROGRESSBAR_X (SNAKE_MAP_WIDTH + 1)
#define PROGRESSBAR_Y (SNAKE_MAP_HEIGHT - 1)
#define PROGRESSBAR_LEN 20

void draw_sidebar_outline() {
    set_color(WHITE, YELLOW);
    move_cursor(SNAKE_MAP_WIDTH + 1, 1);
    for (int i = 0; i < 20; i ++) {
        putchar(' ');
    }
    move_cursor(SNAKE_MAP_WIDTH + 1, SNAKE_MAP_HEIGHT);
    for (int i = 0; i < 20; i ++) {
        putchar(' ');
    }
    for (int i = 1; i <= SNAKE_MAP_HEIGHT; i ++) {
        move_cursor(SNAKE_MAP_WIDTH + 11, i);
        printf("  ");
    }
}

void update_score(SnakeGameData* data) {
    move_cursor(SNAKE_MAP_WIDTH + 1, 3);
    set_color(BLACK, WHITE);
    printf("     Score: %3d     \n", data->score);
}

void start_snake_game(SnakeGameData* data) {
    set_color(BLACK, WHITE);
    puts("");
    clear_screen();
    hide_cursor();
    draw_sidebar_outline();

    move_cursor(PROGRESSBAR_X, PROGRESSBAR_Y - 3);
    for (int i = 0; i < PROGRESSBAR_LEN; i ++) {
        putchar(' ');
    }

    move_cursor(SNAKE_MAP_WIDTH + 1, 5);
    for (int i = 0; i < PROGRESSBAR_LEN; i ++) {
        putchar(' ');
    }

    move_cursor(SNAKE_MAP_WIDTH + 1, 7);
    set_color(BLACK, WHITE);
    printf("        Rank        \n");
    ListNode* cur = data->score_record->scores->head;
    for (int l = 9; l <= PROGRESSBAR_Y - 5 && cur != NULL; l ++, cur = cur->next_node) {
        move_cursor(SNAKE_MAP_WIDTH + 1, l);
        printf("  %s", ((RankData*)cur->value)->name);
        move_cursor(SNAKE_MAP_WIDTH + 8, l);
        printf(" %3d  ", ((RankData*)cur->value)->score);
    }

    update_score(data);

    Pos head_pos = get_random_pos(data);
    add_snake_body(data, head_pos.x, head_pos.y);

    for (int i = 1; i <= SNAKE_MAP_WIDTH; i ++) {
        for (int j = 1; j <= SNAKE_MAP_HEIGHT; j ++) {
            show_block(data->game_map[i][j], i, j);
        }
    }

    show_block(&snake_head_block, SNAKEBODY_HEAD->x, SNAKEBODY_HEAD->y);
    generate_food(data);

    int ch = "wasd"[randint(4)];
    int last_ch = ch;
    double additional_food_last_shown = get_time() - data->additional_food_generate_time * 0.75;
    double additional_food_last_sparkle = get_time();
    while (true) {
        move_cursor(1, 22);
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
        // -1 表示没有奖励食物
        if (data->additional_food_pos.x != -1) {
            // 更新进度条
            move_cursor(PROGRESSBAR_X, PROGRESSBAR_Y);
            set_color(WHITE, RED);
            for (int i = 0; i < min(PROGRESSBAR_LEN * (get_time() - additional_food_last_shown) / data->additional_food_lasting_time, PROGRESSBAR_LEN); i ++) {
                putchar(' ');
            }
            move_cursor(PROGRESSBAR_X, PROGRESSBAR_Y - 1);
            set_color(BLACK, WHITE);
            printf("   Time Left: %2ds   \n", (int)(data->additional_food_lasting_time - get_time() + additional_food_last_shown));
            // 闪烁
            if (get_time() - additional_food_last_sparkle > 0.3) {
                data->additional_food_state ++;
                data->additional_food_state %= sizeof(additional_food_block) / sizeof(BlockType);
                show_block(&additional_food_block[data->additional_food_state], data->additional_food_pos.x, data->additional_food_pos.y);
                additional_food_last_sparkle = get_time();
            }
            // 持续一段时间后消失
            if (get_time() - additional_food_last_shown > data->additional_food_lasting_time) {
                data->game_map[data->additional_food_pos.x][data->additional_food_pos.y] = &empty_block;
                show_block(&empty_block, data->additional_food_pos.x, data->additional_food_pos.y);
                data->additional_food_pos.x = -1;
                // 清空进度条
                set_color(WHITE, WHITE);
                for (int l = PROGRESSBAR_Y - 2; l <= PROGRESSBAR_Y; l ++) {
                    move_cursor(PROGRESSBAR_X, l);
                    for (int i = 0; i < PROGRESSBAR_LEN; i ++) {
                        putchar(' ');
                    }
                }
                putchar('\n');
            }
        } else if (get_time() - additional_food_last_shown > data->additional_food_generate_time) { // 每过一段时间生成
            generate_additional_food(data);
            move_cursor(PROGRESSBAR_X, PROGRESSBAR_Y - 2);
            set_color(BLACK, WHITE);
            printf("     Bonus Food     \n");
            additional_food_last_shown = get_time();
        }
        putchar('\n');
        START_MOVE:;
        int backup_x = head_pos.x;
        int backup_y = head_pos.y;
        if (ch == 'a') {
            head_pos.x --;
        }
        if (ch == 'd') {
            head_pos.x ++;
        }
        if (ch == 's') {
            head_pos.y ++;
        }
        if (ch == 'w') {
            head_pos.y --;
        }
        if (data->game_map[head_pos.x][head_pos.y] == &wall_block) {
            break;
        }
        if (data->game_map[head_pos.x][head_pos.y] == &random_portal_block) {
            Pos* transport_to = (Pos*)get_list_val(data->random_portals, randint(data->random_portals->size));
            head_pos.x = transport_to->x;
            head_pos.y = transport_to->y;
            goto START_MOVE;
        }
        if (data->game_map[head_pos.x][head_pos.y] == &portal_block) {
            Pos transport_to = data->transport_to[head_pos.x][head_pos.y];
            head_pos.x = transport_to.x;
            head_pos.y = transport_to.y;
            goto START_MOVE;
        }
        if (head_pos.x != backup_x || head_pos.y != backup_y) {
            int last_tail_x = SNAKEBODY_TAIL->x;
            int last_tail_y = SNAKEBODY_TAIL->y;
            if (data->game_map[head_pos.x][head_pos.y] == &food_block) {
                move_head_to(data, head_pos.x, head_pos.y);
                add_snake_body(data, last_tail_x, last_tail_y);
                generate_food(data);
                data->score += 1;
                update_score(data);
            } else if (data->game_map[head_pos.x][head_pos.y] == &eraser_block) {
                if (data->snake->size <= 1) {
                    break;
                }
                move_head_to(data, head_pos.x, head_pos.y);
                pop_snake_body(data);
            } else if (data->game_map[head_pos.x][head_pos.y] == &additional_food_block[0]) {
                data->additional_food_pos.x = -1;
                move_head_to(data, head_pos.x, head_pos.y);
                add_snake_body(data, last_tail_x, last_tail_y);
                // 清空进度条
                set_color(WHITE, WHITE);
                for (int l = PROGRESSBAR_Y - 2; l <= PROGRESSBAR_Y; l ++) {
                    move_cursor(PROGRESSBAR_X, l);
                    for (int i = 0; i < PROGRESSBAR_LEN; i ++) {
                        putchar(' ');
                    }
                }
                putchar('\n');
                data->score += 5;
                update_score(data);
            } else {
                move_head_to(data, head_pos.x, head_pos.y);
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
    set_color(BLACK, WHITE);
    puts("");
    clear_screen();
    hide_cursor();
    draw_sidebar_outline();

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
