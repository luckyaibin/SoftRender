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

#define  FRACBITS 16

	float f=7.75f;
	int fix_value = float_to_fixpoint(f,FRACBITS);
	float converted_value = fixpoint_to_float(fix_value,FRACBITS);

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
	
	 g_drawed;
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

