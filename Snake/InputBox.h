#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <ctype.h>
#include <stdarg.h>
#include "ConsoleIO.h"

// 输入框，最长输入 max_length 长的字符串。同时有 message_cout 个提示语，依次居中输出
char* input_box(int max_length, int message_count, ...);

#endif
