#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <ctype.h>
#include <stdarg.h>
#include "ConsoleIO.h"

char* input_box(int max_length, int message_count, ...);

#endif
