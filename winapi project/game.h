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
} Game;

void game_init(Game* game, HWND hWnd);
void game_update(Game* game);
void game_render(Game* game, HDC hdc);
void game_handle_input(Game* game, WPARAM wParam, LPARAM lParam, int key_down);

#endif // GAME_H
