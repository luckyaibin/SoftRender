
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
bool Init3DLib(HINSTANCE hInstance, HWND hWnd, int width, int height);

void FlipSurface();
void Release3DLib();

bool Init3DLib(HINSTANCE hInstance, HWND hWnd, int width, int height);

int DrawPixel(int x,int y, ARGB color);



void DrawLine(int sx,int sy,int ex,int ey,ARGB color);
int DrawPartCircle(int centerx,int centery,int x,int y,ARGB color);
int DrawCircle(int x,int y,int r,ARGB color);
int DrawTriangle( vertex2d v1, vertex2d v2,  vertex2d v3,ARGB color);
void drawTriangle(vertex2d v1,vertex2d v2,vertex2d v3,ARGB color);
#endif