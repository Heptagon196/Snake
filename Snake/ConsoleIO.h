#ifndef CONIOPLUS_H
#define CONIOPLUS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#if defined(linux) || defined(__APPLE__)

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define BLACK 0
#define BLUE 4
#define GREEN 6
#define CYAN 2
#define RED 1
#define PURPLE 5
#define YELLOW 3
#define WHITE 7

bool kbhit(); // check if any key is pressed
int getch(); // get the key which the user has pressed

#else

#include <conio.h>
#include <windows.h>
#include <time.h>
#define BLACK 8
#define BLUE 9
#define GREEN 10
#define CYAN 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15

#endif

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 20

void move_cursor(int x, int y);
void hide_cursor();
void show_cursor();
void set_color(int foreground_color, int background_color); // set the foreground and background color
double get_time(); // get the time the program has run for
int read_in_seconds(double lasting_time); // return the key that user has pressed in given seconds. If there's no keys that are pressed, return 0;
void clear_screen();
void clear_color(); // reset the output color

#endif
