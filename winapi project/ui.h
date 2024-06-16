#ifndef UI_H
#define UI_H

#include <windows.h>
#include <atlimage.h>

typedef struct {
    int lives; // 캐릭터의 목숨 개수
    int coins; // 획득한 코인 개수
    CImage life_image; // 캐릭터 생명을 나타내는 이미지
    CImage coin_image; // 코인 이미지
    int coin_current_frame; // 현재 코인 애니메이션 프레임
    int coin_total_frames; // 총 코인 애니메이션 프레임 수
    float coin_animation_speed; // 코인 애니메이션 속도
    float coin_animation_timer; // 코인 애니메이션 타이머
} UI;

void ui_init(UI* ui, const wchar_t* life_image_path, const wchar_t* coin_image_path, int coin_total_frames, float coin_animation_speed);
void ui_destroy(UI* ui);
void ui_render(UI* ui, HDC hdc, int window_width, int window_height);
void ui_update(UI* ui, float dt);
void ui_update_lives(UI* ui, int lives);
void ui_update_coins(UI* ui, int coins);

#endif // UI_H
