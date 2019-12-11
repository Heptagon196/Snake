#ifndef BLOCK_H
#define BLOCK_H

#include "ConsoleIO.h"

typedef struct {
    int foreground, background;
    const char* content;
} BlockType;

void show_block(const BlockType* type, int x, int y);

#endif
