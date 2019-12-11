#include "Block.h"

void show_block(const BlockType* type, int x, int y) {
    move_cursor(x, y);
    set_color(type->foreground, type->background);
    puts(type->content);
}
