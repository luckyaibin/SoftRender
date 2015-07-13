
#ifndef __DRAW_H__
#define __DRAW_H__
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include "comm_headers.h"
#include "triangle.h"
// 全局变量
static IDirect3DDevice9* pDevice = 0;
static IDirect3DSurface9* pSurface = 0;

// 函数
bool Init3DLib(HINSTANCE hInstance, HWND hWnd, int32_t width, int32_t height);

void FlipSurface();
void Release3DLib();

int FillSurface(ARGB color);

bool Init3DLib(HINSTANCE hInstance, HWND hWnd, int32_t width, int32_t height);

int32_t DrawPixel(int32_t x,int32_t y, ARGB color);



void DrawLine(int32_t sx,int32_t sy,int32_t ex,int32_t ey,ARGB color);
int32_t DrawPartCircle(int32_t centerx,int32_t centery,int32_t x,int32_t y,ARGB color);
int32_t DrawCircle(int32_t x,int32_t y,int32_t r,ARGB color);
int32_t DrawTriangle( vertex2d v1, vertex2d v2,  vertex2d v3,ARGB color);
void drawTriangle(vertex2d v1,vertex2d v2,vertex2d v3,ARGB color);
#endif