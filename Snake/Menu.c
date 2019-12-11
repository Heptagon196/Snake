#include "Menu.h"

typedef struct {
    char* content;
} Option;

Option* new_option(char* content) {
    Option* opt = (Option*)malloc(sizeof(Option));
    opt->content = content;
    return opt;
}

void destroy_option(void* val) {
    free((Option*)val);
}

void print_outline() {
    set_color(WHITE, GREEN);
    move_cursor(1, 1);
    printf("╔");
    for (int i = 2; i <= SCREEN_WIDTH - 1; i ++) {
        printf("═");
    }
    printf("╗");
    move_cursor(1, SCREEN_HEIGHT);
    printf("╚");
    for (int i = 2; i <= SCREEN_WIDTH - 1; i ++) {
        printf("═");
    }
    printf("╝");
    for (int i = 2; i <= SCREEN_HEIGHT - 1; i ++) {
        move_cursor(1, i);
        printf("║ ");
        move_cursor(SCREEN_WIDTH / 2, i);
        printf(" ║");
    }
}

void print_to_middle(const char* content, int line) {
    int left_spaces_count = (SCREEN_WIDTH - strlen(content)) / 2;
    int right_spaces_count = SCREEN_WIDTH - left_spaces_count - strlen(content);
    left_spaces_count -= 2;
    right_spaces_count -= 2;
    move_cursor(2, line + 2);
    for (int i = 0; i < left_spaces_count; i ++) {
        putchar(' ');
    }
    printf("%s", content);
    for (int i = 0; i < right_spaces_count; i ++) {
        putchar(' ');
    }
    putchar('\n');
}

void print_chosen(Option* option, int line) {
    set_color(BLACK, WHITE);
    print_to_middle(option->content, line);
}

void print_normal(Option* option, int line) {
    set_color(WHITE, BLACK);
    print_to_middle(option->content, line);
}

int Menu(int count, ...) {
    List* options = (List*)malloc(sizeof(List));
    init_list(options, destroy_option);
    va_list params;
    va_start(params, count);
    char* title = va_arg(params, char*);
    count --;
    for (int i = 0; i < count; i ++) {
        list_append(options, new_option(va_arg(params, char*)));
    }
    va_end(params);
    
    int ans = 0;
    clear_screen();
    hide_cursor();
    print_outline();
    set_color(BLUE, BLACK);
    print_to_middle(title, 0);
    int option_cnt = 1;
    list_foreach(options) {
        print_normal(list_iter(Option), option_cnt);
        option_cnt ++;
    }
    print_chosen((Option*)get_list_val(options, 0), 1);
    int ch = 0;
    while (true) {
        ch = getch();
        if (ch == 'q') {
            ans = -1;
            break;
        }
        if (ch == '\n' || ch == '\r' || ch == ' ') {
            break;
        }
        int previous_ans = ans;
        if (ch == 'w') {
            ans --;
        }
        if (ch == 's') {
            ans ++;
        }
        if (ans < 0) {
            ans = count - 1;
        }
        if (ans >= count) {
            ans = 0;
        }
        if (previous_ans != ans) {
            print_normal((Option*)get_list_val(options, previous_ans), previous_ans + 1);
            print_chosen((Option*)get_list_val(options, ans), ans + 1);
        }
    }
    show_cursor();
    destroy_list(options);
    return ans;
}
