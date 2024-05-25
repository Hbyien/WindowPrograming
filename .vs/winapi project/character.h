#ifndef CHARACTER_H
#define CHARACTER_H

#include <windows.h>
#include "map.h"
typedef struct {
    int x;
    int y;
    int width;
    int height;
    float jump_velocity;
    float gravity;
    bool is_jumping;
    bool is_flying;
} Character;

Character* character_create(int x, int y, int width, int height);
void character_destroy(Character* character);
void character_move(Character* character, Map* map, int dx, int dy);
int character_check_collision(Character* character, Map* map);

#endif // CHARACTER_H