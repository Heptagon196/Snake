#include "ConsoleIO.h"

#if defined(linux) || defined(__APPLE__)
// implement some functions in 'conio.h' on Windows for Linux and MacOS

// using linux api
bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int getch() {
     struct termios tm, tm_old;
     int fd = 0, ch;
     if (tcgetattr(fd, &tm) < 0) {
          return -1;
     }
     tm_old = tm;
     cfmakeraw(&tm);
     if (tcsetattr(fd, TCSANOW, &tm) < 0) {
          return -1;
     }
     ch = getchar();
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) {
          return -1;
     }
     return ch;
}

// using terminal control code
void move_cursor(int x, int y) {
    printf("\033[%d;%dH", y, x * 2 - 1);
}

void hide_cursor() {
    printf("\033[?25l");
}

void show_cursor() {
    printf("\033[?25h");
}

void set_color(int foreground_color, int background_color) {
    printf("\033[%dm\033[%dm", background_color + 40, foreground_color + 30);
}

void clear_screen() {
    system("clear");
}

void clear_color() {
    printf("\033[0m");
}

#else

#define color_calc(fg, bg) ((fg) + 16 * (bg))

// using windows api
void move_cursor(int x, int y) {
    COORD c;
    c.X = x * 2 - 2;
    c.Y = y - 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hide_cursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void show_cursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 25};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void set_color(int foreground_color, int background_color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_calc(foreground_color, background_color));
}

void clear_color() {
    set_color(WHITE, BLACK);
}

void clear_screen() {
    system("cls");
}

#endif

double get_time() {
    return (double) clock() / CLOCKS_PER_SEC;
}

int read_in_seconds(double lasting_time) {
    double last_time_point = get_time();
    while (kbhit()) {
        getch();
    }
    while (!kbhit() && (get_time() - last_time_point < lasting_time)) {
        continue;
    }
    if (!kbhit()) {
        return 0;
    }
    while (get_time() - last_time_point < lasting_time) {
        continue;
    }
    return getch();
}

