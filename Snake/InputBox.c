#include "InputBox.h"

void destroy_char(void* val) {
    free((char*)val);
}

char* input_box(int max_length, int message_count, ...) {
    char* s = (char*)calloc(max_length + 1, sizeof(char));

    set_color(BLACK, WHITE);

    int top_left_x = SCREEN_WIDTH * 0.3, top_left_y = SCREEN_HEIGHT * 0.3;
    int bottom_right_x = SCREEN_WIDTH * 0.7, bottom_right_y = SCREEN_HEIGHT * 0.7;

    for (int i = top_left_y; i <= bottom_right_y; i ++) {
        move_cursor_origin(top_left_x - 1, i);
        for (int j = top_left_x - 1; j <= bottom_right_x; j ++) {
            putchar(' ');
        }
    }

    va_list messages;
    va_start(messages, message_count);
    for (int i = 0; i < message_count; i ++) {
        const char* msg = va_arg(messages, const char*);
        print_to_middle(msg, top_left_y + 2 + i, (SCREEN_WIDTH - strlen(msg)) / 2 - 1);
    }
    va_end(messages);

    //print_box(1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);
    print_box(top_left_x, top_left_y, bottom_right_x, bottom_right_y);
    print_box(top_left_x + 4, top_left_y + 5, bottom_right_x - 4, top_left_y + 7);
    show_cursor();
    move_cursor_origin(top_left_x + 6, top_left_y + 6);

    int cnt = 0;
    int ch;
    while (true) {
        ch = getch();
        if (ch == '\n' || ch == '\r') {
            break;
        }
        // escape
        if (ch == 27) {
            free(s);
            return NULL;
        }
        if (ch == 127 || ch == 8) {
            if (cnt >= 1) {
                move_cursor_origin(top_left_x + 6 + cnt - 1, top_left_y + 6);
                putchar(' ');
                move_cursor_origin(top_left_x + 6 + cnt - 1, top_left_y + 6);
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
