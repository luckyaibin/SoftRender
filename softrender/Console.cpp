#include "Console.h"
#include "vector.h"
#include <Windows.h>
#include <iostream>
#include "tool.h"
#include "quaternion.h"
#include "euler_rotate.h"
#include "model.h"
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
	quaternion q1;
	quaternion q2;
	float r1= PI;
	float r2 = PI/2;

	q1.t = cos(r1);
	q1.x = sin(r1)*sqrt(1/9.0f);
	q1.y = sin(r1)*sqrt(4/9.0f);
	q1.z = sin(r1)*sqrt(4/9.0f);

	q2.dump();
	q2.t = cos(r2);
	q2.dump();
	q2.x = sin(r2)*sqrt(1/81.0f);
	q2.dump();
	q2.y = sin(r2)*sqrt(16/81.0f);
	q2.dump();
	q2.z = sin(r2)*sqrt(64/81.0f);
	q2.dump();
	
	printf("%f	",sqrt(1/81.0f));
	printf("%f	",sin(r2));
	printf("%f	",sqrt(16/81.0f));
	printf("%f	",sin(r2));
	printf("%f	",sqrt(64/81.0f));
	printf("%f	",sin(r2));


	printf("%f,		\n",quaternion_get_norm(q1));
	printf("%f,		\n",quaternion_get_norm(q2));

	quaternion slerped_q=q1;
	float inter_t = 0;
	quaternion pre_slerped_q = q1;
	for(inter_t=0;inter_t<1.0f;)
	{
		
		slerped_q = quaternion_slerp(q1,q2,inter_t);
		float diff_rad = acos(quaternion_dot_mul(slerped_q,pre_slerped_q));
		printf("diff rad :  %f \n",diff_rad);
		pre_slerped_q = slerped_q;
		float acos_angle = acos(slerped_q.t);
		printf("角度%f,t:%f,x:%f,y:%f,z:%f\n",acos_angle,slerped_q.t,slerped_q.x,slerped_q.y,slerped_q.z);
		inter_t+=0.01f;
	}

	unsigned long begin_tick = GetTickCount();
	for ( int32_t i=0;i<10000;i++)
	{
		mod_pi(HALF_PI);
	}
	unsigned long end_tick = GetTickCount();

	printf("mod_pi time consumed .. %d %f\n",end_tick - begin_tick,mod_pi(HALF_PI));
	begin_tick = GetTickCount();
	for ( int32_t i=0;i<10000;i++)
	{
		fmod(HALF_PI,PI);
	}
	end_tick = GetTickCount();

	printf("fmod time consumed .. %d %f\n",end_tick - begin_tick,fmod(HALF_PI,PI));
 
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

