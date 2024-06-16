// map.c
#include "map.h"
#include <atlimage.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define MAX_CLIFFS 3            //stage 1 낭떠러지 영역 몬스터 낭떨어지 안떨어지도록
#define BRICK_WIDTH 16
#define BRICK_HEIGHT 16

Cllif cllif[MAX_CLIFFS] = {
    {0, 20},
    {1100, 1130},
    {1370, 1420}
};

RECT collsion_rects[] = {
    {0, 210, 1106, 236},
    {447, 175, 480, 210},
    {606, 160, 640, 208},
    {736, 145, 768, 210},
    {913, 145, 948, 210},
    {1136, 210, 1375, 236},
    {1425, 210, 2449, 210},
    {2144, 193, 2209, 208},
    {2160, 176, 2209, 193},
    {2175, 159, 2209, 176},
    {2191, 142, 2209, 159},
    {2240, 142, 2255, 210},
    {2255, 159, 2271, 210},
    {2271, 175, 2287, 210},
    {2287, 191, 2303, 210},
    {2368, 193, 2448, 210},
    {2384, 177, 2448, 210},
    {2400, 161, 2448, 210},
    {2416, 145, 2448, 210},
    {2480, 142, 2495, 210},
    {2495, 159, 2510, 210},
    {2510, 175, 2525, 210},
    {2525, 191, 2540, 210},
    {2608, 175, 2640, 210},
    {2864, 175, 2895, 210},
    {2896, 191, 3040, 210},
    {2912, 175, 3040, 210},
    {2928, 159, 3040, 210},
    {2944, 143, 3040, 210},
    {2960, 127, 3040, 210},
    {2976, 111, 3040, 210},
    {2992, 94, 3040, 210},
    {3008, 78, 3040, 210},
    {1423, 210, 2448, 236},
    {2480, 210, 3374, 236}
};

RECT brick_collision[] = {
    {320, 143, 320 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {352, 143, 354 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {384, 143, 368 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1231, 143, 1231 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1263, 143, 1263 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1280, 79, 1280 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1296, 79, 1296 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1312, 79, 1312 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1328, 79, 1328 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1344, 79, 1344 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1360, 79, 1360 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1376, 79, 1376 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1392, 79, 1392 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1455, 79, 1455 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1471, 79, 1471 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1487, 79, 1487 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1504, 144, 1504 + BRICK_WIDTH, 144 + BRICK_HEIGHT},
    {383, 143, 382 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1598, 143, 1598 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1614, 143, 1611 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1887, 143, 1887 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1935, 79, 1935 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1951, 79, 1951 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1967, 79, 1967 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {2063, 143, 2063 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {2079, 143, 2079 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {2047, 79, 2047 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {2095, 79, 2095 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {2687, 143, 2687 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {2703, 143, 2703 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {2735, 143, 2735 + BRICK_WIDTH, 143 + BRICK_HEIGHT}
};

RECT item_brick_collision[] = {
    {256, 143, 253 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {336, 143, 336 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {369, 143, 367 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {352, 79, 352 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1248, 143, 1248 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1504, 79, 1504 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {1694, 143, 1694 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1744, 143, 1744 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1791, 143, 1791 + BRICK_WIDTH, 143 + BRICK_HEIGHT},
    {1744, 79, 1744 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {2063, 79, 2063 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {2080, 79, 2080 + BRICK_WIDTH, 79 + BRICK_HEIGHT},
    {2720, 143, 2720 + BRICK_WIDTH, 143 + BRICK_HEIGHT}
};

KumbaSpawnPoint spawn_points[] = {
    {420, 185},
    {550, 185},
    {850, 185},
    {1050, 185},
    {1200, 185},
    {1600, 185},
    {1900, 185},
    {2777, 185}
    // 추가적인 소환 위치를 여기서 정의
};

CoinSpawnPoint coin_spawn_points[] = {
    {352, 62},
    {322, 126},
    {459, 154},
    {923, 124},
    {1295, 62},
    {1613, 126},
    {1282, 128},
    {2225, 110},
    {2740, 128},
    {2874, 49}
};

Map* map_create(const wchar_t* filename, const wchar_t* cloud_filename, HWND hWnd) {
    Map* map = (Map*)malloc(sizeof(Map));
    CImage image;
    HRESULT hr = image.Load(filename);
    if (FAILED(hr)) {
        MessageBox(hWnd, L"Failed to load map image", L"Error", MB_OK);
        free(map);
        return NULL;
    }

    // 충돌 영역 초기화
    map->num_collision_rects = sizeof(collsion_rects) / sizeof(collsion_rects[0]);
    map->collision_rects = (RECT*)malloc(map->num_collision_rects * sizeof(RECT));
    for (int i = 0; i < map->num_collision_rects; ++i) {
        map->collision_rects[i] = collsion_rects[i];
    }

    map->width = image.GetWidth();
    map->height = image.GetHeight();
    map->map_image = image.Detach();
    map->show_collision = true;

    // 절벽 초기화
    for (int i = 0; i < MAX_CLIFFS; ++i) {
        map->cliffs[i] = cllif[i];
    }

    // 구름 레이어 초기화
    CImage cloudImage;
    hr = cloudImage.Load(cloud_filename);
    if (FAILED(hr)) {
        MessageBox(NULL, L"Failed to load cloud layer image", L"Error", MB_OK);
        DeleteObject(map->map_image);
        free(map);
        return NULL;
    }
    map->cloud_layer_image = cloudImage.Detach();
    map->cloud_offset_x = 0;

    // Kumba 소환 위치 초기화
    map->num_spawn_points = sizeof(spawn_points) / sizeof(spawn_points[0]);
    map->spawn_points = (KumbaSpawnPoint*)malloc(map->num_spawn_points * sizeof(KumbaSpawnPoint));
    for (int i = 0; i < map->num_spawn_points; ++i) {
        map->spawn_points[i] = spawn_points[i];
    }

    // 벽돌 충돌 영역 초기화
    map->num_brick_collisions = sizeof(brick_collision) / sizeof(brick_collision[0]);
    map->brick_collision_rects = (RECT*)malloc(map->num_brick_collisions * sizeof(RECT));
    for (int i = 0; i < map->num_brick_collisions; ++i) {
        map->brick_collision_rects[i] = brick_collision[i];
    }

    // 아이템 벽돌 충돌 영역 초기화
    map->num_item_brick_collisions = sizeof(item_brick_collision) / sizeof(item_brick_collision[0]);
    map->item_brick_collision_rects = (RECT*)malloc(map->num_item_brick_collisions * sizeof(RECT));
    for (int i = 0; i < map->num_item_brick_collisions; ++i) {
        map->item_brick_collision_rects[i] = item_brick_collision[i];
    }

    // 코인 소환 위치 초기화
    map->num_coin_spawn_points = sizeof(coin_spawn_points) / sizeof(coin_spawn_points[0]);
    map->coin_spawn_points = (CoinSpawnPoint*)malloc(map->num_coin_spawn_points * sizeof(CoinSpawnPoint));
    for (int i = 0; i < map->num_coin_spawn_points; ++i) {
        map->coin_spawn_points[i] = coin_spawn_points[i];
    }

    return map;
}

void map_destroy(Map* map) {
    DeleteObject(map->map_image);
    free(map->collision_rects);
    free(map->brick_collision_rects);
    free(map->item_brick_collision_rects); // 추가
    free(map->coin_spawn_points);
    free(map->spawn_points);

    free(map);
}

void map_render(Map* map, HDC hdc, int x, int y, int window_width, int window_height) {
    // 구름 레이어를 그립니다.
    HDC hCloudDC = CreateCompatibleDC(hdc);
    HBITMAP oldCloudBitmap = (HBITMAP)SelectObject(hCloudDC, map->cloud_layer_image);

    int cloud_width = map->width;
    int cloud_height = map->height;

    int cloud_x = (int)map->cloud_offset_x % cloud_width;

    // 구름 레이어를 StretchBlt를 사용하여 스케일링 및 이동하여 그립니다.
    StretchBlt(hdc, 0, 0, window_width, window_height, hCloudDC, cloud_x, 0, cloud_height, cloud_height, SRCCOPY);
    if (cloud_x + cloud_height > cloud_width) {
        cloud_x = 0;
    }
    SelectObject(hCloudDC, oldCloudBitmap);

    DeleteDC(hCloudDC);

    // 맵 레이어를 그립니다.
    HDC hMapDC = CreateCompatibleDC(hdc);
    HBITMAP oldMapBitmap = (HBITMAP)SelectObject(hMapDC, map->map_image);

    int map_width = map->width;
    int map_height = map->height;

    // 임시 DC 생성
    HDC hTempDC = CreateCompatibleDC(hdc);
    HBITMAP hTempBitmap = CreateCompatibleBitmap(hdc, window_width, window_height);
    HBITMAP oldTempBitmap = (HBITMAP)SelectObject(hTempDC, hTempBitmap);

    // StretchBlt를 사용하여 임시 DC에 비트맵을 복사합니다.
    StretchBlt(hTempDC, 0, 0, window_width, window_height, hMapDC, x, y, map_height, map_height, SRCCOPY);

    // 투명색을 흰색으로 설정하고 TransparentBlt를 사용하여 최종적으로 화면에 그립니다.
    TransparentBlt(hdc, 0, 0, window_width, window_height, hTempDC, 0, 0, window_width, window_height, RGB(255, 255, 255));

    // 리소스 해제
    SelectObject(hTempDC, oldTempBitmap);
    DeleteObject(hTempBitmap);
    DeleteDC(hTempDC);

    SelectObject(hMapDC, oldMapBitmap);
    DeleteDC(hMapDC);

    // 충돌 영역을 그립니다.
    if (map->show_collision) {
        HPEN pen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0)); // 두께 4의 빨간색 펜으로 변경
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HBRUSH hBrush = (HBRUSH)GetStockObject(HOLLOW_BRUSH); // 채우지 않는 브러쉬로 설정
        HGDIOBJ oldBrush = SelectObject(hdc, hBrush);

        float scale_x = (float)window_width / map_height;
        float scale_y = (float)window_height / map_height;

        for (int i = 0; i < map->num_collision_rects; ++i) {
            RECT rect = map->collision_rects[i];
            RECT scaled_rect = {
                (int)((rect.left - x) * scale_x),
                (int)((rect.top - y) * scale_y),
                (int)((rect.right - x) * scale_x),
                (int)((rect.bottom - y) * scale_y)
            };
            Rectangle(hdc, scaled_rect.left, scaled_rect.top, scaled_rect.right, scaled_rect.bottom); // 사각형 테두리를 그립니다
        }

        for (int i = 0; i < map->num_brick_collisions; ++i) {
            RECT rect = map->brick_collision_rects[i];
            RECT scaled_rect = {
                (int)((rect.left - x) * scale_x),
                (int)((rect.top - y) * scale_y),
                (int)((rect.right - x) * scale_x),
                (int)((rect.bottom - y) * scale_y)
            };
            Rectangle(hdc, scaled_rect.left, scaled_rect.top, scaled_rect.right, scaled_rect.bottom); // 사각형 테두리를 그립니다
        }

        for (int i = 0; i < map->num_item_brick_collisions; ++i) {
            RECT rect = map->item_brick_collision_rects[i];
            RECT scaled_rect = {
                (int)((rect.left - x) * scale_x),
                (int)((rect.top - y) * scale_y),
                (int)((rect.right - x) * scale_x),
                (int)((rect.bottom - y) * scale_y)
            };
            Rectangle(hdc, scaled_rect.left, scaled_rect.top, scaled_rect.right, scaled_rect.bottom); // 사각형 테두리를 그립니다
        }

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
    }
}

void map_toggle_collision(Map* map) {
    map->show_collision = !map->show_collision;
}

bool RectsIntersect(const RECT* rect1, const RECT* rect2) {
    return !(rect1->right < rect2->left ||
        rect1->left > rect2->right ||
        rect1->bottom < rect2->top ||
        rect1->top > rect2->bottom);
}

void map_update(Map* map, float player_velocity_x, float dt) {
    // 플레이어 속도의 절반으로 구름 레이어 이동
    map->cloud_offset_x += player_velocity_x * 0.5f * dt;
    if (map->cloud_offset_x >= map->width) {
        map->cloud_offset_x = 0;
    }
}

int map_check_collision(Map* map, int x, int y, int width, int height) {
    RECT obj_rect = { x, y, x + width, y + height }; // 캐릭터 좌표 받기

    for (int i = 0; i < map->num_collision_rects; ++i) {
        if (RectsIntersect(&obj_rect, &map->collision_rects[i])) {
            return 1; // 충돌 발생
        }
    }
    for (int i = 0; i < map->num_brick_collisions; ++i) { // 필드명 수정
        if (RectsIntersect(&obj_rect, &map->brick_collision_rects[i])) {
            return 1; // 충돌 발생
        }
    }

    for (int i = 0; i < map->num_item_brick_collisions; ++i) { // 필드명 추가
        if (RectsIntersect(&obj_rect, &map->item_brick_collision_rects[i])) {
            return 1; // 충돌 발생
        }
    }

    return 0; // 충돌 없음
}

bool check_cliff(Map* map, int new_x) {
    for (int i = 0; i < MAX_CLIFFS; ++i) {
        if (new_x >= map->cliffs[i].startx && new_x <= map->cliffs[i].endx) {
            return true; // 낭떠러지 영역 내에 있음
        }
    }
    return false; // 낭떠러지 영역 내에 없음
}
