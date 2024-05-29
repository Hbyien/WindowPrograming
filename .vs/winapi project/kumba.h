#ifndef KUMBA_H
#define KUMBA_H

#include <windows.h>
#include "map.h"
#include <atlimage.h>

typedef struct {
    float x;
    float y;
    int width;
    int height;
    float velocity_x;
    float velocity_y; // ���� �ӵ� �߰�
    float gravity; // �߷� �߰�
    bool is_alive;
    int current_frame;   // ���� �ִϸ��̼� ������
    int total_frames;    // �� ������ ��
    float animation_speed; // �ִϸ��̼� �ӵ�
    float animation_timer; // �ִϸ��̼� Ÿ�̸�
} Kumba;

extern CImage kumba_image;
extern int sprite_width;
extern int sprite_height;
extern int total_frames;    // �� ������ ��


void kumba_init(const wchar_t* image_path, int total_frames, COLORREF transparent_color); // �̹��� �ʱ�ȭ �Լ�
Kumba* kumba_create(float x, float y, int width, int height, float velocity_x, float velocity_y);
void kumba_destroy(Kumba* kumba);
void kumba_move(Kumba* kumba, Map* map, float dt);
int kumba_check_collision(Kumba* kumba, Map* map);
void kumba_render(Kumba* kumba, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height);// ������ �Լ� �߰�


#endif // KUMBA_H
