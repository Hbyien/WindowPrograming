// map.h
#ifndef MAP_H
#define MAP_H

#include <windows.h>

typedef struct {
    HBITMAP map_image;
    int width;
    int height;
    RECT* collision_rects;
    int num_collision_rects;
    bool show_collision;
} Map;

Map* map_create(const wchar_t* filename, HWND hWnd);
void map_destroy(Map* map);
void map_render(Map* map, HDC hdc, int x, int y, int window_width, int window_height);
void map_toggle_collision(Map* map);
int map_check_collision(Map* map, int x, int y, int width, int height); // 충돌 체크 함수
bool RectsIntersect(const RECT* rect1, const RECT* rect2);
#endif // MAP_H
