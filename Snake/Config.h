#ifndef CONFIG_H
#define CONFIG_H
#include <string.h>
#include "Snake.h"
#include "Menu.h"

void edit_config(SnakeGameData* data);
void save_config(SnakeGameData* data, const char* config_filename);

#endif
