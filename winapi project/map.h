// map.h
#ifndef MAP_H
#define MAP_H

#include <windows.h>

typedef struct {
    int x;
    int y;
} KumbaSpawnPoint;

typedef struct {
    int x;
    int y;
} CoinSpawnPoint;

typedef struct Cllif {
    int startx;
    int endx;
};


typedef struct Brick {
    RECT brick_collsion;
};

typedef struct {
    HBITMAP map_image;
    int width;
    int height;
    RECT* collision_rects;
    int num_collision_rects;
    bool show_collision;
    Cllif cliffs[3];
    KumbaSpawnPoint* spawn_points; // Kumba ��ȯ ��ġ �迭
    int num_spawn_points; // Kumba ��ȯ ��ġ ��
    RECT* brick_collision_rects;
    int num_brick_collsions;
    CoinSpawnPoint* coin_spawn_points; // Coin ��ȯ ��ġ �迭
    int num_coin_spawn_points; // Coin ��ȯ ��ġ ��
} Map;



Map* map_create(const wchar_t* filename, HWND hWnd);
void map_destroy(Map* map);
void map_render(Map* map, HDC hdc, int x, int y, int window_width, int window_height);
void map_toggle_collision(Map* map);
int map_check_collision(Map* map, int x, int y, int width, int height); // �浹 üũ �Լ�
bool RectsIntersect(const RECT* rect1, const RECT* rect2);
bool check_cliff(Map* map, int new_x);

#endif // MAP_H
