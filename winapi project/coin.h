#ifndef COIN_H
#define COIN_H


#include <windows.h>
#include "map.h"
#include <atlimage.h>


typedef struct {
    float x;
    float y;
    int width;
    int height;
    //float velocity_x;
    //float velocity_y; // 수직 속도 추가
    //float gravity; // 중력 추가
    //bool is_alive;
    int current_frame;   // 현재 애니메이션 프레임
    int coin_total_frames;    // 총 프레임 수
    float animation_speed; // 애니메이션 속도
    float animation_timer; // 애니메이션 타이머
} Coin;


extern CImage coin_image;
extern int coin_sprite_width;
extern int coin_sprite_height;
extern int coin_total_frames;

void coin_init(const wchar_t* image_path, int frames, COLORREF transparent_color);
Coin* coin_create(float x, float y, int width, int height);
void coin_destroy(Coin* coin);
int coin_check_collision(Coin* coin, Map* map);
void coin_render(Coin* coin, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height);
void coin_update(Coin* coin, float dt);

#endif