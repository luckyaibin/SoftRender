#include "Console.h"
#include "vector.h"
#include <Windows.h>
#include <iostream>
#include "tool.h"
#include <math>


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

int32_t Game_Init()
{
	Init3DLib(g_HInstance, g_WindowHandle, SCREEN_WIDTH, SCREEN_HEIGHT);
	AllocConsole();
	freopen( "CONOUT$","w",stdout);
	return 1;
}
int32_t g_drawed = 0;
int32_t Game_Main()
{
	StartClock();
	float test_abs_f_pos = 32.1f;
	float test_abs_f_neg = -32.1f;
	uint32_t test_abs_f_format_pos = ((FLOAT_UINT32_UNION*)&test_abs_f_pos)->ui;
	uint32_t test_abs_f_format_neg = ((FLOAT_UINT32_UNION*)&test_abs_f_neg)->ui;

	int32_t test_abs = -32;
	printf("%d",test_abs>>31);
	float flt_epsilon = 1.192092896e-07F;//FLT_EPSILON

	dump_float(PI);
	dump_float(PI/2);
	dump_float(-PI/2);
	dump_float(1.40129846e-45);
	float rad = 30/180.f * 3.1415926535;
	if (IS_FLOAT_INFINITE(rad))
	{
		int32_t s = ( ((FLOAT_FORMAT*)&rad))->sign;
	}
	__todo_fast_tan(rad);
	float f=700.12345f;
	float g = 9.43f;
	int32_t ff = float_to_fixpoint(f,FRACBITS);
	int32_t fg = float_to_fixpoint(g,FRACBITS);

	int32_t f_diviedby_g = float(ff) / (fg>>FRACBITS);
	float converted_value = fixpoint_to_float(f_diviedby_g,FRACBITS);

	fast_sin_cordic(0);
	fast_sin_cordic(PI/2);
	fast_sin_cordic(PI);
	fast_sin_cordic(PI*3/2);
	fast_sin_cordic(PI*2);

	fast_sin_cordic(-PI/2);
	fast_sin_cordic(-PI);
	fast_sin_cordic(-PI*3/2);
	fast_sin_cordic(-PI*2);

	fast_sin_cordic(PI/4);
	fast_sin_cordic(PI*3/4);
	fast_sin_cordic(PI*5/4);
	fast_sin_cordic(PI*7/4);

	fast_sin_cordic(-PI/4);
	fast_sin_cordic(-PI*3/4);
	fast_sin_cordic(-PI*5/4);
	fast_sin_cordic(-PI*7/4);
	unsigned long begin_tick = GetTickCount();
	for ( int32_t i=0;i<100000000;i++)
	{
		int32_t y = ((i)>>31);
		int32_t absv = i^y - y;
		//((x)^((x)>>31) - ((x)>>31))
	}
	unsigned long end_tick = GetTickCount();

	printf("sin time consumed .. %d,value is %5.5f\n",end_tick - begin_tick,sin(rad));
	begin_tick = GetTickCount();
	for ( int32_t i=0;i<100000000;i++)
	{
		int32_t absv = abs(i);
	}
	end_tick = GetTickCount();

	printf("fast_tan2 time consumed .. %d,value is %5.5f\n",end_tick - begin_tick,fast_sin_cos_fix_point_18(rad));
 
	//DrawTriangleWithEdgeEquation
	for(int32_t i=0;i<1000;i++)
	{
		vertex2d v1,v2,v3;
		 int32_t a = rand()%255;
		 int32_t r = rand()%255;
		 int32_t g = rand()%255;
		 int32_t b = rand()%255;
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

int32_t Game_Shutdown()
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

int32_t WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
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

