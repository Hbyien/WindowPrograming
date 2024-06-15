// game.c
#include "game.h"
#include <iostream>
    
void game_init(Game* game, HWND hWnd) {

    game->game_state = STATE_LOGO;
    LoadLogo();
    LoadMap();

    { // map 1 초기화 
        game->map = map_create(L"BaseImage//STAGE1.png", hWnd);
        RECT rect;
        GetClientRect(hWnd, &rect);
        int window_width = rect.right - rect.left;
        int window_height = rect.bottom - rect.top;
        game->camera = camera_create(window_width, window_height, game->map->width, game->map->height);
        game->character = character_create(50, 185, 15, 20); // 캐릭터 초기 위치 및 크기 설정
        kumba_init(L"enemy//kumba.png", 8, RGB(120, 182, 255));
        game->kumbas_num = game->map->num_spawn_points;
        game->kumbas = (Kumba**)malloc(game->kumbas_num * sizeof(Kumba*));
        for (int i = 0; i < game->kumbas_num; i++) {
            std::cout << game->kumbas_num << std::endl;
            game->kumbas[i] = kumba_create(
                (float)game->map->spawn_points[i].x,
                (float)game->map->spawn_points[i].y,
                20, 20, 50.0f, 0.016f);
        }
        
        int num_coins = 10;
        for (int i = 0; i < num_coins; i++) {
            Coin* coin = coin_create(game->map->coin_spawn_points[i].x, game->map->coin_spawn_points[i].y, 16, 16, L"BaseImage//Coin.png", 4, 0.1f);
            game->coins.push_back(coin);
        }
        std::cout << "코인 개수는 :" << game->coins.size() << endl;

        game->coin_count = 0; // 코인 개수 초기화
        game->move_direction = 0;
    }

}

void game_update(Game* game) {
    switch (game->game_state) {
    case STATE_LOGO:
        break;
    case STATE_MAP:
        break;
    case STATE_WORLD1:
        float dt = 0.016f;
        for (int i = 0; i < game->kumbas_num; i++) {
            kumba_move(game->kumbas[i], game->map, dt);
        }
        character_update(game->character, game->map, dt);
        camera_set(game->camera, game->character);

        for (auto& coin : game->coins) {
            coin_update(coin, dt); // 코인 업데이트 추가
        }

        for (auto it = game->coins.begin(); it != game->coins.end(); ) {
            if (coin_character_check_collision(game->character, (*it))) {
                game->coin_count++;
                it = game->coins.erase(it); // 충돌된 코인을 벡터에서 제거
            }
            else {
                ++it;
            }
        }

        break;
    }
}


void game_render(Game* game, HDC hdc) {
    int window_width = 800;
    int window_height = 600;
    switch (game->game_state){
        case STATE_LOGO: 
        RenderLogo(hdc);
        break;
        case STATE_MAP:
        RenderMap(hdc);
        break;
        case STATE_WORLD1: 
        map_render(game->map, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height);
        for (int i = 0; i < game->kumbas_num; ++i) {
            kumba_render(game->kumbas[i], hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
        }
        for (auto& coin : game->coins) {
            coin_render(coin, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
        }
        character_render(game->character, hdc, camera_get_x(game->camera), camera_get_y(game->camera), window_width, window_height, game->map->height);
        
        break;
        
    }
    
}

void game_handle_input(Game* game, WPARAM wParam, LPARAM lParam, int key_down) {
    switch (game->game_state) {
    case STATE_LOGO: {
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);
        std::cout << xPos << " " << yPos;
        if (xPos >= 530 && xPos <= 780 && yPos >= 350 && yPos <= 410) {
            game->game_state = STATE_MAP;
        }
        else if (xPos >= 530 && xPos <= 780 && yPos >= 430 && yPos <= 490) {
            PostQuitMessage(0);
        }
        break;
    }
    case STATE_MAP:
        if (key_down) {
            switch (wParam) {
            case VK_LEFT:
                MoveMapPlayerLeft();
                break;
            case VK_RIGHT:
                MoveMapPlayerRight();
                break;
            case VK_RETURN: {
                int world_num = 0;
                world_num = CheckEnterKey();
                std::cout << world_num << std::endl;
                if (world_num == 1) {
                    game->game_state = STATE_WORLD1;
                }
                else if (world_num == 2) {
                    game->game_state = STATE_WORLD2;
                }
                else if (world_num == 3) {
                    game->game_state = STATE_WORLD3;
                }
                break;
            }
            }

        }
        break;

    case STATE_WORLD1:
        character_handle_input(game->character, wParam, key_down);
        if (key_down && (wParam == 'C' || wParam == 'c')) {
            map_toggle_collision(game->map);
        }

        if (key_down && (wParam == 'n' || wParam == 'N'))
            game->coins.size();

        break;
    }
}

