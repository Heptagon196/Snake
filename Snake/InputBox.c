#include "InputBox.h"

char* input_box(int max_length, int message_count, const char* init_ans, ...) {
    char* s = (char*)calloc(max_length + 1, sizeof(char));

    set_color(BLACK, WHITE);

    int top_left_x = SCREEN_WIDTH * 0.3, top_left_y = SCREEN_HEIGHT * 0.3;
    int bottom_right_x = SCREEN_WIDTH * 0.7, bottom_right_y = SCREEN_HEIGHT * 0.7;

    // 清空区域
    for (int i = top_left_y; i <= bottom_right_y; i ++) {
        move_cursor_origin(top_left_x, i);
        for (int j = top_left_x - 1; j <= bottom_right_x - 1; j ++) {
            putchar(' ');
        }
    }

    // 可变参数列表。依次获取提示语并居中输出
    va_list messages;
    va_start(messages, init_ans);
    for (int i = 0; i < message_count; i ++) {
        const char* msg = va_arg(messages, const char*);
        print_to_middle(msg, top_left_y + 2 + i, (SCREEN_WIDTH - strlen(msg)) / 2 - 1);
    }
    va_end(messages);

    // 输出边框
    print_box(top_left_x, top_left_y, bottom_right_x, bottom_right_y);
    print_box(top_left_x + 4, top_left_y + 5, bottom_right_x - 4, top_left_y + 7);
    show_cursor();
    move_cursor_origin(top_left_x + 6, top_left_y + 6);

    int cnt = strlen(init_ans);
    printf("%s", init_ans);
    strcpy(s, init_ans);
    int ch;
    while (true) {
        ch = getch();
        // 回车提交输入
        if (ch == '\n' || ch == '\r') {
            break;
        }
        // esc 取消读入
        if (ch == 27) {
            free(s);
            return NULL;
        }
        // 退格，linux 下为 127，windows 下为 8
        if (ch == 127 || ch == 8) {
            if (cnt >= 1) {
                // 用空格覆盖最后一个字符
                move_cursor_origin(top_left_x + 6 + cnt - 1, top_left_y + 6);
                putchar(' ');
                move_cursor_origin(top_left_x + 6 + cnt - 1, top_left_y + 6);
                cnt --;
            }
        } else if (cnt >= max_length) {
            continue;
        } else if (ch >= 0 && ch < 128 && (isgraph(ch) || ch == ' ')) {
            // ch >= 0 && ch < 128 禁止输入中文，isgraph 判断字符是否可打印
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
