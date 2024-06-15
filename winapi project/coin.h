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
    //float velocity_y; // ���� �ӵ� �߰�
    //float gravity; // �߷� �߰�
    //bool is_alive;
    int current_frame;   // ���� �ִϸ��̼� ������
    int coin_total_frames;    // �� ������ ��
    float animation_speed; // �ִϸ��̼� �ӵ�
    float animation_timer; // �ִϸ��̼� Ÿ�̸�
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