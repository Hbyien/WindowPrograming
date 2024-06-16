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
    int move_direction; //-1 왼쪽 1오른쪽 0 제자리
    int kumbas_num;
    int game_state; // 시작 로고, 1 맵 이동, 2. 맵1 3. 맵2 4.맵3
    std::vector<Coin*> coins; // 코인 벡터
    int coins_num; // 코인 수
    int coin_count; // 획득한 코인 개수
    int restart_x; // 캐릭터 재시작 위치 x
    int restart_y; // 캐릭터 재시작 위치 y
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
