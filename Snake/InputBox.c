#include "InputBox.h"

void destroy_char(void* val) {
    free((char*)val);
}

char* input_box(const char* message, int max_length) {
    char* s = (char*)calloc(max_length + 1, sizeof(char));

    set_color(BLACK, WHITE);
    puts("");
    clear_screen();
    set_color(BLACK, WHITE);

    int top_left_x = SCREEN_WIDTH * 0.25, top_left_y = SCREEN_HEIGHT * 0.25;
    int bottom_left_x = SCREEN_WIDTH * 0.75, bottom_left_y = SCREEN_HEIGHT * 0.75;
    print_to_middle(message, top_left_y + 1);

    print_box(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);
    print_box(top_left_x, top_left_y, bottom_left_x, bottom_left_y);
    print_box(top_left_x + 4, top_left_y + 6, bottom_left_x - 4, top_left_y + 8);
    show_cursor();
    move_cursor_origin(top_left_x + 6, top_left_y + 7);

    int cnt = 0;
    int ch;
    while (true) {
        ch = getch();
        if ((ch == '\n' || ch == '\r') && cnt != 0) {
            break;
        }
        if (ch == 127) {
            if (cnt >= 1) {
                move_cursor_origin(top_left_x + 6 + cnt - 1, top_left_y + 7);
                putchar(' ');
                move_cursor_origin(top_left_x + 6 + cnt - 1, top_left_y + 7);
                cnt --;
            }
        } else if (cnt >= max_length) {
            continue;
        } else if (ch >= 0 && ch < 128 && ch != '\n' && ch != '\r') {
            s[cnt] = ch;
            putchar(ch);
            cnt ++;
        }
    }
    s[cnt] = '\0';

    char* ans = (char*)calloc(cnt + 1, sizeof(char));
    strcpy(ans, s);
    free(s);
    return ans;
}
