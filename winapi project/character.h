#ifndef CHARACTER_H
#define CHARACTER_H

#include <windows.h>
#include <atlimage.h>
#include <vector>
#include "map.h"
#include "bullet.h"

typedef enum {
    IDLE,
    WALK,
    JUMP,
    FLY,
    FLAG
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
    bool on_flag;
    CharacterState state;
    Direction direction;
    int move_direction; //-1 ���� 1������ 0 ���ڸ�
    int current_frame;
    float animation_timer;
    float animation_speed;
    CImage images[10]; // �ִϸ��̼� ���� �迭
    std::vector<Bullet*> bullets; // ĳ������ �Ѿ� ����Ʈ
} Character;

Character* character_create(int x, int y, int width, int height);
void character_destroy(Character* character);
void character_move(Character* character, Map* map, int dx, int dy);
void character_render(Character* character, HDC hdc, int camera_x, int camera_y, int window_width, int window_height, int map_height);
void character_handle_input(Character* character, WPARAM wParam, int key_down);
void character_update(Character* character, Map* map, float dt);
void character_fire(Character* character);

#endif // CHARACTER_H
