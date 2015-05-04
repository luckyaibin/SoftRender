#include "Console.h"
#include "vector.h"
#include <Windows.h>
#include <iostream>




//宏定义
#define WINDOW_CLASS_NAME TEXT("CPPYIN3DWNDCLS")
#define WINDOW_TITLE TEXT("CPPYIN3DCONSOLE")
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

	// 画图
	/*for (int x = 0; x < 200; ++x)
	{
	for (int y = 0; y < 200; ++y)
	{
	DrawPixel(x, y, ARGB(0, x*y%255, x*y%255, x*y%255));
	}
	}*/
	int sx = rand() % 300 + 100;
	int sy = rand() % 300 + 100;
	int ex = rand() % 300 + 100;
	int ey = rand() % 300 + 100;
	//DrawPixel(43, 613, ARGB(0,255,0,0));
	printf("圆心坐标sx,sy:%d,%d\n",sx,sy);
	if(!g_drawed)
	{
		g_drawed = 1;
		DrawCircle(400,400,150,ARGB(0,0,255,0));
	}

	vertex2d v1,v2,v3;
	v1.x = 0;
	v1.y = 0;
	v2.x = 200;
	v2.y = 0;
	v3.x = 100;
	v3.y = 100;


	DrawTriangleWithEdgeEquation(v1,v2,v3,ARGB(0,255,0,0));
	//DrawTriangleWithEdgeEquation
	for(int i=0;i<1000;i++)
	{
		vertex2d p;
		p.x = rand()%500;
		p.y = rand()%500;

		/*if (!is_point_in_triangle_by_crossproduct(p,v1,v2,v3))
		{
		DrawPixel(p.x,p.y,ARGB(255,0,255,0));
		}*/
		if (is_point_in_triangle_by_barycentric_algo(p,v1,v2,v3))
		{
			DrawPixel(p.x,p.y,ARGB(255,0,255,0));
		}
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

