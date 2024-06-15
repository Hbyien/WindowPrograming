#ifndef COIN_H
#define COIN_H

#include <windows.h>
#include <atlimage.h>
#include "character.h"

typedef struct {
    int x;
    int y;
    int width;
    int height;
    int current_frame;
    int total_frames;
    float animation_speed;
    float animation_timer;
    CImage image;
} Coin;

Coin* coin_create(int x, int y, int width, int height, const wchar_t* image_path, int total_frames, float animation_speed);
void coin_destroy(Coin* coin);
void coin_update(Coin* coin, float dt);
void coin_render(Coin* coin, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height);
bool coin_character_check_collision(Character* character, Coin* coin);

#endif // COIN_H