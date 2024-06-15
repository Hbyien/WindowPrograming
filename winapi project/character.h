#ifndef CHARACTER_H
#define CHARACTER_H

#include <windows.h>
#include <atlimage.h>
#include "map.h"


typedef enum {
    IDLE,
    WALK
} CharacterState;

typedef enum {
    LEFT,
    RIGHT
} Direction;

typedef struct {
    int x;
    int y;
    int width;
    int height;
    float jump_velocity;
    float gravity;
    bool is_jumping;
    bool is_flying;
    CharacterState state;
    Direction direction;
    int move_direction; //-1 왼쪽 1오른쪽 0 제자리
    int current_frame;
    float animation_timer;
    float animation_speed;
    CImage images[2]; // IDLE과 WALK 이미지 배열
} Character;

Character* character_create(int x, int y, int width, int height);
void character_destroy(Character* character);
void character_move(Character* character, Map* map, int dx, int dy);
void character_render(Character* character, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height);
void character_handle_input(Character* character, WPARAM wParam, int key_down);
void character_update(Character* character, Map* map, float dt);

#endif // CHARACTER_H
