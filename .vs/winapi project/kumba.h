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
    float velocity_y; // 수직 속도 추가
    float gravity; // 중력 추가
    bool is_alive;
    int current_frame;   // 현재 애니메이션 프레임
    int total_frames;    // 총 프레임 수
    float animation_speed; // 애니메이션 속도
    float animation_timer; // 애니메이션 타이머
} Kumba;

extern CImage kumba_image;
extern int sprite_width;
extern int sprite_height;
extern int total_frames;    // 총 프레임 수


void kumba_init(const wchar_t* image_path, int total_frames, COLORREF transparent_color); // 이미지 초기화 함수
Kumba* kumba_create(float x, float y, int width, int height, float velocity_x, float velocity_y);
void kumba_destroy(Kumba* kumba);
void kumba_move(Kumba* kumba, Map* map, float dt);
int kumba_check_collision(Kumba* kumba, Map* map);
void kumba_render(Kumba* kumba, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height);// 렌더링 함수 추가


#endif // KUMBA_H
