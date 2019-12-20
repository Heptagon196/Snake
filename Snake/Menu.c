#include "Menu.h"

void print_chosen(const char* option, int line) {
    set_color(WHITE, YELLOW);
    print_to_middle(option, line + 2, 1);
}

void print_normal(const char* option, int line) {
    set_color(BLACK, WHITE);
    print_to_middle(option, line + 2, 1);
}

int Menu(int count, int init_option, const char* title, const char* option[]) {
    set_color(BLACK, WHITE);
    puts("");
    clear_screen();

    hide_cursor();
    set_color(BLACK, WHITE);
    print_box(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);

    set_color(BLUE, WHITE);
    print_to_middle(title, 2, 2);
    int option_cnt = 1;
    for (int i = 0; i < count; i ++) {
        print_normal(option[i], option_cnt);
        option_cnt ++;
    }
    int ans = init_option;
    print_chosen(option[ans], ans + 1);
    int ch = 0;
    while (true) {
        ch = getch();
        if (ch == 'q' || ch == 27) {
            ans = -1;
            break;
        }
        if (ch == '\n' || ch == '\r') {
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
            print_normal(option[previous_ans], previous_ans + 1);
            print_chosen(option[ans], ans + 1);
        }
    }
    show_cursor();
    return ans;
}
