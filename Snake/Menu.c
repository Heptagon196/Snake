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
    set_color(BLACK, WHITE);
    print_box(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void print_chosen(Option* option, int line) {
    set_color(WHITE, YELLOW);
    print_to_middle(option->content, line + 2, 2);
}

void print_normal(Option* option, int line) {
    set_color(BLACK, WHITE);
    print_to_middle(option->content, line + 2, 2);
}

int Menu(int count, ...) {
    // 可变参数列表
    // 将所有参数依次放入链表中
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
    set_color(BLACK, WHITE);
    puts("");
    clear_screen();
    hide_cursor();
    print_outline();
    set_color(BLUE, WHITE);
    print_to_middle(title, 2, 2);
    int option_cnt = 1;
    for (ListNode* i = options->head; i != NULL; i = i->next_node) {
        print_normal((Option*)i->value, option_cnt);
        option_cnt ++;
    }
    print_chosen((Option*)get_list_val(options, 0), 1);
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
            print_normal((Option*)get_list_val(options, previous_ans), previous_ans + 1);
            print_chosen((Option*)get_list_val(options, ans), ans + 1);
        }
    }
    show_cursor();
    destroy_list(options);
    return ans;
}
