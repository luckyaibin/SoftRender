
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
#define ARGB_MASK 0xff

#define ARGB(a, r, g, b) ((b&ARGB_MASK) + ((g&ARGB_MASK) << 8) + ((r&ARGB_MASK) << 16) + ((a&ARGB_MASK) << 24))

#define GET_ARGB_A(argb) ( (argb>>24) & ARGB_MASK)
#define GET_ARGB_R(argb) ( (argb>>16) & ARGB_MASK)
#define GET_ARGB_G(argb) ( (argb>>8) & ARGB_MASK)
#define GET_ARGB_B(argb) ( (argb>>0) & ARGB_MASK)
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