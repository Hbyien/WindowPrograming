#ifndef UI_H
#define UI_H

#include <windows.h>
#include <atlimage.h>

typedef struct {
    int lives; // ĳ������ ��� ����
    int coins; // ȹ���� ���� ����
    CImage life_image; // ĳ���� ������ ��Ÿ���� �̹���
    CImage coin_image; // ���� �̹���
    int coin_current_frame; // ���� ���� �ִϸ��̼� ������
    int coin_total_frames; // �� ���� �ִϸ��̼� ������ ��
    float coin_animation_speed; // ���� �ִϸ��̼� �ӵ�
    float coin_animation_timer; // ���� �ִϸ��̼� Ÿ�̸�
} UI;

void ui_init(UI* ui, const wchar_t* life_image_path, const wchar_t* coin_image_path, int coin_total_frames, float coin_animation_speed);
void ui_destroy(UI* ui);
void ui_render(UI* ui, HDC hdc, int window_width, int window_height);
void ui_update(UI* ui, float dt);
void ui_update_lives(UI* ui, int lives);
void ui_update_coins(UI* ui, int coins);

#endif // UI_H
