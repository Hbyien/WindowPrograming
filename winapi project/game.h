#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include "camera.h"
#include "map.h"
#include "character.h"
#include "kumba.h"
#include "Logo.h"
#include "MoveMap.h"
#include <vector>
#include "Coin.h"
#include <string>
#include "ui.h"

enum GameState {
    STATE_LOGO,
    STATE_MAP,
    STATE_WORLD1,
    STATE_WORLD2,
    STATE_WORLD3
};

typedef struct {
    Map* map;
    Camera* camera;
    Character* character;
    Kumba** kumbas;
    int move_direction; //-1 ���� 1������ 0 ���ڸ�
    int kumbas_num;
    int game_state; // ���� �ΰ�, 1 �� �̵�, 2. ��1 3. ��2 4.��3
    std::vector<Coin*> coins; // ���� ����
    int coins_num; // ���� ��
    int coin_count; // ȹ���� ���� ����
    int restart_x; // ĳ���� ����� ��ġ x
    int restart_y; // ĳ���� ����� ��ġ y
    int timer;
    DWORD last_update_time;
    UI ui;

} Game;

void game_init(Game* game, HWND hWnd);
void game_update(Game* game, HWND hWnd);
void game_render(Game* game, HDC hdc);
void game_handle_input(Game* game, WPARAM wParam, LPARAM lParam, int key_down);
void character_reset_position(Game* game,HWND hWnd);
bool bullet_kumba_check_collision(Bullet* bullet, Kumba* kumba);

#endif // GAME_H
