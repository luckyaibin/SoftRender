#include "Console.h"
#include "vector.h"
#include <Windows.h>
#include <iostream>
#include "tool.h"



//宏定义
#define WINDOW_CLASS_NAME TEXT("CPPYIN3DWNDCLS")
#define WINDOW_TITLE TEXT("engine")
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32
#define WAIT_TIME 30

	// 全局变量定义
HWND g_WindowHandle;
HINSTANCE g_HInstance;
DWORD g_Clock;

// 宏脚本
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// 函数定义
DWORD GetClock()
{
	return GetTickCount(); 
}

void StartClock()
{
	g_Clock = GetClock();
}

void WaitClock()
{
	while((GetClock() - g_Clock) < WAIT_TIME)
	{
		Sleep(5);
	}
}

int Game_Init()
{
	Init3DLib(g_HInstance, g_WindowHandle, SCREEN_WIDTH, SCREEN_HEIGHT);
	AllocConsole();
	freopen( "CONOUT$","w",stdout);
	return 1;
}
int g_drawed = 0;
int Game_Main()
{
	
	StartClock();
	srand(GetTickCount());
	for(int cnt=0;cnt < 100000;cnt++)
	{
		int i = rand();
		i = (i+3)*i;
		int highest_bit_pos = get_msb2(i);
	}

//#define  FRACBITS 16
	float rad = 30/180.f * 3.1415926;
	fast_tan(rad);
	float f=700.12345f;
	float g = 9.43f;
	int ff = float_to_fixpoint(f,FRACBITS);
	int fg = float_to_fixpoint(g,FRACBITS);

	int f_diviedby_g = float(ff) / (fg>>FRACBITS);
	float converted_value = fixpoint_to_float(f_diviedby_g,FRACBITS);

	for (float f=-100.0f;f<100.0f;f=f+1.0f)
	{
		float mod = fmod(f,1.7f);
		printf("%5.5f\n",mod);
	}
	/*
	float k_accu_table[21] = {
		0.70710678118655f,
		0.63245553203368f,
		0.6135719910779f,
		0.60883391251775f,
		0.60764825625617f,
		0.6073517701413f,
		0.60727764409353f,
		0.60725911229889f,
		0.60725447933256f,
		0.60725332108988f,
		0.60725303152913f,
		0.60725295913894f,
		0.6072529410414f,
		0.60725293651701f,
		0.60725293538591f,
		0.60725293510314f,
		0.60725293503245f,
		0.60725293501477f,
		0.60725293501035f,
		0.60725293500925f,
		0.60725293500897f
	};

	for ( int i=0;i<21;i++)
	{
		printf("k_accu_table %d,%d,\n",i,float_to_fixpoint(k_accu_table[i],18));
	}

	float atan_rad_table[21] = {
		0.78539816339745f,
		0.46364760900081f,
		0.24497866312686f,
		0.12435499454676f,
		0.062418809995957f,

		0.031239833430268f,
		0.015623728620477f,
		0.0078123410601011f,
		0.003906230131967f,
		0.0019531225164788f,

		0.00097656218955932f,
		0.0004882812111949f,
		0.00024414062014936f,
		0.00012207031189367f,
		6.1035156174209e-005f,

		3.0517578115526e-005f,
		1.5258789061316e-005f,
		7.629394531102e-006f,
		3.8146972656065e-006f,
		1.9073486328102e-006f,

		9.5367431640596e-007f,
	};

	for ( int i=0;i<21;i++)
	{
		printf("atan_rad_table %d,%d,\n",i,float_to_fixpoint(atan_rad_table[i],18));
	}
	*/
	unsigned long begin_tick = GetTickCount();
	for ( int i=0;i<100000;i++)
	{
		fast_tan2((float)i);
	}
	unsigned long end_tick = GetTickCount();

	printf("sin time consumed .. %d,value is %5.5f\n",end_tick - begin_tick,sin(rad));
	begin_tick = GetTickCount();
	for ( int i=0;i<100000;i++)
	{
		fast_tan_fix_point_18((float)i);
	}
	end_tick = GetTickCount();

	printf("fast_tan2 time consumed .. %d,value is %5.5f\n",end_tick - begin_tick,fast_tan_fix_point_18(rad));


	

	/*if (g_drawed%3==0)
	{
	vertex2d v1,v2,v3;
	v1.x = 100;
	v1.y = 100;
	v1.color=ARGB(0,255,0,0);
	v2.x = 300;
	v2.y = 100;
	v2.color=ARGB(0,0,255,0);
	v3.x = 100;
	v3.y = 300;
	v3.color=ARGB(0,0,0,255);

	DrawTriangleWithEdgeEquation(v1,v2,v3,ARGB(0,255,0,0));
	}*/
	
	if (g_drawed%3==1)
	{
		vertex2d v11,v22,v33;
		v11.x = 400;
		v11.y = 200;
		v11.color=ARGB(0,255,0,0);
		v22.x = 200;
		v22.y = 200;
		v22.color=ARGB(0,0,255,0);
		v33.x = 200;
		v33.y = 400;
		v33.color=ARGB(0,0,0,255);

		DrawTriangleWithEdgeEquation(v11,v22,v33,ARGB(0,255,0,0));
	}
	
	if (g_drawed%3==2)
	{
		vertex2d v111,v222,v333;
		v111.x = 300;
		v111.y = 500;
		v111.color=ARGB(0,255,0,0);
		v222.x = 500;
		v222.y = 300;
		v222.color=ARGB(0,0,255,0);
		v333.x = 300;
		v333.y = 300;
		v333.color=ARGB(0,0,0,255);

		DrawTriangleWithEdgeEquation(v111,v222,v333,ARGB(0,255,0,0));
	}
	
	 g_drawed++;
	//DrawTriangleWithEdgeEquation
	for(int i=0;i<1000;i++)
	{
		vertex2d v1,v2,v3;
		 int a = rand()%255;
		 int r = rand()%255;
		 int g = rand()%255;
		 int b = rand()%255;
		v1.x = rand() % 400;
		v1.y = rand() % 400;
		v1.color = ARGB(a,255,0,0);

		a = rand()%255;
		r = rand()%255;
		g = rand()%255;
		b = rand()%255;
		v2.x = rand() % 400;
		v2.y = rand() % 400;
		v2.color = ARGB(a,0,255,0);
		a = rand()%255;
		r = rand()%255;
		g = rand()%255;
		b = rand()%255;
		v3.x = rand() % 400;
		v3.y = rand() % 400;
		v3.color = ARGB(a,0,0,255);

		DrawTriangleWithEdgeEquation(v1,v2,v3,ARGB(0,rand()% 255,rand()%255,rand()%255));
		// DrawTriangle(v1,v2,v3,ARGB(0,rand()% 255,rand()%255,rand()%255));
		// drawTriangle(v1,v2,v3,ARGB(0,rand()% 255,rand()%255,rand()%255));
	}

	// 输出
	FlipSurface();

	WaitClock();
	return 1;
}

int Game_Shutdown()
{
	Release3DLib();
	FreeConsole();
	return 1;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(msg)
	{
	case WM_CREATE: 
		{
			return 0;
		} break;
	case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE)
			{
				PostMessage(g_WindowHandle, WM_DESTROY, 0, 0);
			} break;
		}
	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			return 0;
		} break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	default:break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	WNDCLASS winclass;
	HWND hwnd;
	MSG msg;

	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hInstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClass(&winclass))
	{
		return 0;
	}

	if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL)))
	{
		return 0;
	}

	g_HInstance = hInstance;
	g_WindowHandle = hwnd;

	if (!Game_Init())
	{
		return 0;
	}

	while(TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Game_Main();
	}

	Game_Shutdown();

	return msg.wParam;
}

