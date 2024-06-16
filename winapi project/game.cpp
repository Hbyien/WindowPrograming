// game.c
#include "game.h"
#include <iostream>

void game_init(Game* game, HWND hWnd) {

    game->game_state = STATE_LOGO;
    game->timer = 0;
    game->last_update_time = GetTickCount();


    LoadLogo();
    LoadMap();

    { // map 1 초기화 
        game->map = map_create(L"BaseImage//STAGE1-layer.png", L"BaseImage//STAGE1-layer1.png", hWnd);
        ui_init(&game->ui, L"Character//Kirby_Standard.png", L"BaseImage//Coin.png",4,0.1f);
        RECT rect;
        GetClientRect(hWnd, &rect);
        int window_width = rect.right - rect.left;
        int window_height = rect.bottom - rect.top;
        game->camera = camera_create(window_width, window_height, game->map->width, game->map->height);
        game->character = character_create(50, 185, 15, 20); // 캐릭터 초기 위치 및 크기 설정
        game->restart_x = 50; // 재시작 위치 설정
        game->restart_y = 185;
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
        std::cout << " " << game->coins.size() << std::endl;

        game->coin_count = 0;
        game->move_direction = 0;
        game->move_direction = 0;
    }

}

void game_update(Game* game) {
    DWORD current_time = GetTickCount();


    switch (game->game_state) {
    case STATE_LOGO:
        break;
    case STATE_MAP:
        break;
    case STATE_WORLD1:
        float dt = 0.016f;
        for (int i = 0; i < game->kumbas_num; i++) {
            kumba_move(game->kumbas[i], game->map, dt);
            if (game->kumbas[i]->is_alive && character_kumba_check_collision(game->character, game->kumbas[i])) {
                if (game->character->y + game->character->height - 5 < game->kumbas[i]->y + game->kumbas[i]->height / 3) {
                    game->kumbas[i]->is_alive = false; 
                    game->character->jump_velocity = -200; 
                }
                else {
                    character_reset_position(game); // 캐릭터 죽고 재시작 위치로 이동
                }
            }

        }

        character_update(game->character, game->map, dt);
        camera_set(game->camera, game->character);

        map_update(game->map, game->character->move_direction * 100.0f, dt);

        for (auto it = game->coins.begin(); it != game->coins.end(); ) {
            Coin* coin = *it;
            if (coin_character_check_collision(game->character, coin)) {
                game->coin_count++;
                it = game->coins.erase(it); // 충돌된 코인을 벡터에서 제거
                ui_update_coins(&game->ui, game->coin_count);
                free(coin); // 메모리 해제
            }
            else {
                coin_update(coin, dt);

                ++it;
            }
        }
        ui_update(&game->ui, dt);
        game->map->cloud_offset_x += 0.1;
        if (game->map->cloud_offset_x >= game->map->width) {
            game->map->cloud_offset_x = 0;
        }
        if( game->character->y > 250 )
            character_reset_position(game);
        break;
    }
    if (game->game_state == STATE_WORLD1) {
        if (current_time - game->last_update_time >= 1000) {
            game->timer++;
            game->last_update_time = current_time;
        }
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

        //타이머그리기
        std::wstring timer_text = L"Timer: ";
        timer_text += (game->timer < 100) ? L"0" : L"";
        timer_text += (game->timer < 10) ? L"0" : L"";
        timer_text += std::to_wstring(game->timer);

        ui_render(&game->ui, hdc, window_width, window_height);
        SetTextColor(hdc, RGB(255, 255, 0));
        SetBkMode(hdc, TRANSPARENT);

        HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        TextOut(hdc, window_width - 150, 10, timer_text.c_str(), timer_text.length());

       
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);

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
        break;
    }
}

void character_reset_position(Game* game) {
    game->character->x = game->restart_x;
    game->character->y = game->restart_y;
    game->character->is_jumping = false;
    game->character->is_flying = false;
    game->character->jump_velocity = 0;
    game->character->state = IDLE;
    game->character->move_direction = 0;

    ui_update_lives(&game->ui, game->ui.lives - 1);
}