#ifndef GAME_H
#define GAME_H

#include <windows.h>
#include <vector>
#include "camera.h"
#include "map.h"
#include "character.h"
#include "Logo.h"
#include "MoveMap.h"
#include "coin.h"
#include "kumba.h"

using namespace std;

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
    int move_direction; //-1 ¿ÞÂÊ 1¿À¸¥ÂÊ 0 Á¦ÀÚ¸®
    int kumbas_num;
    int game_state; // ½ÃÀÛ ·Î°í, 1 ¸Ê ÀÌµ¿, 2. ¸Ê1 3. ¸Ê2 4.¸Ê3
    vector<Coin*> coins;
    int coins_num; 
    int coin_count;
} Game;

void game_init(Game* game, HWND hWnd);
void game_update(Game* game);
void game_render(Game* game, HDC hdc);
void game_handle_input(Game* game, WPARAM wParam, LPARAM lParam, int key_down);

#endif // GAME_H
