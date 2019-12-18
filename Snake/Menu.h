#ifndef MENU_H
#define MENU_H

#include <stdarg.h>
#include <string.h>
#include "ConsoleIO.h"
#include "List.h"

// 显示有 count 个选项的菜单
int Menu(int count, int init_option, ...);

#endif
