// map.c
#include "map.h"
#include <atlimage.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

RECT collsion_rects[] = {
    {0, 208, 1106, 236},
    {447, 175, 480, 208},
    {606,160,640,208},
    {736, 145, 768,208},
    {913,145,948,208},
    {1136,208,1375,236},
    {1425,208,2449,208},

    {2144,193,2209,208},
    {2160, 176,2209,193},
    {2175,159, 2209,176},
    {2191,142,2209,159} ,

    {2240,142,2255,208},
    {2255,159,2271,208},
    {2271,175,2287,208},
    {2287,191,2303,208},

    {2368,193,2448,208},
    {2384,177,2448,208},
    {2400,161,2448,208},
    {2416,145,2448,208},

    {2480, 142 ,2495,208}, 
    {2495, 159,2510,208},
    {2510,175,2525,208},
    {2525,191,2540,208}
        
  , {2608,175,2640,208},
    {2864,175,2895,208},

    {2896,191,3040,208},
    {2912,175,3040,208},
    {2928,159,3040,208},
    {2944,143,3040,208},
    {2960,127,3040,208},
    {2976,111,3040,208},
    {2992,94,3040,208},
    {3008,78,3040,208},
    {1423,208,2448,236 },
    {2480,208,3374,236 }
    
};

Map* map_create(const wchar_t* filename, HWND hWnd) {
    Map* map = (Map*)malloc(sizeof(Map));
    CImage image;
    HRESULT hr = image.Load(filename);
    if (FAILED(hr)) {
        MessageBox(hWnd, L"Failed to load map image", L"Error", MB_OK);
        exit(EXIT_FAILURE);
    }

    map->num_collision_rects = sizeof(collsion_rects) / sizeof(collsion_rects[0]);
    map->collision_rects = (RECT*)malloc(map->num_collision_rects * sizeof(RECT));
    for (int i = 0; i < map->num_collision_rects; ++i) {
        map->collision_rects[i] = collsion_rects[i];
    }

    map->width = image.GetWidth();
    map->height = image.GetHeight();
    map->map_image = image.Detach();
    map->show_collision = true;
    return map;
}

void map_destroy(Map* map) {
    DeleteObject(map->map_image);
    free(map->collision_rects);
    free(map);
}

void map_render(Map* map, HDC hdc, int x, int y, int window_width, int window_height) {
    HDC hMemDC = CreateCompatibleDC(hdc);
    SelectObject(hMemDC, map->map_image);

    int map_width = map->width;
    int map_height = map->height;

    StretchBlt(hdc, 0, 0, window_width, window_height, hMemDC, x, y, map_height, map_height, SRCCOPY);

    if (map->show_collision) {
        HPEN pen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0)); // 두께 3의 빨간색 펜으로 변경
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

        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
    }

    DeleteDC(hMemDC);
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


int map_check_collision(Map* map, int x, int y, int width, int height) {
    RECT obj_rect = { x, y, x + width, y + height }; // 캐릭터 좌표 받기

    // 디버깅 출력
    std::cout << "Checking collision at (" << x << ", " << y << ", " << x + width << ", " << y + height << ")" << std::endl;

    for (int i = 0; i < map->num_collision_rects; ++i) {
        std::cout << "Against collision rect ("
            << map->collision_rects[i].left << ", "
            << map->collision_rects[i].top << ", "
            << map->collision_rects[i].right << ", "
            << map->collision_rects[i].bottom << ")" << std::endl;

        if (RectsIntersect(&obj_rect, &map->collision_rects[i])) {
            std::cout << "충돌 발생 " << std::endl;
            return 1; // 충돌 발생
        }
    }
    std::cout << "No collision." << std::endl;
    return 0; // 충돌 없음
}