#ifndef MOVEMAP_H
#define MOVEMAP_H

#include "game.h"

void RenderMap(HDC hDC);
void MoveMapPlayerLeft();
void MoveMapPlayerRight();
void LoadMap();
void FreeMap();
//void UpdatePlayerPosition();
int CheckEnterKey();


#endif