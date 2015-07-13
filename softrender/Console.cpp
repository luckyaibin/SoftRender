#include "Console.h"
#include "vector.h"
#include <Windows.h>
#include <iostream>
//#include "tool.h"
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
Object g_obj;
UVNCamera g_camera;
vector3 g_camera_target_pos(0,0,5);
int32_t Game_Init()
{
	Init3DLib(g_HInstance, g_WindowHandle, SCREEN_WIDTH, SCREEN_HEIGHT);
	AllocConsole();
	freopen( "CONOUT$","w",stdout);

	ObjectInit(&g_obj);
	//g_camera.world_pos = vector3(0,0,5);

	vector3 init_v(0,1,0);
	InitUVNCamera(&g_camera,
		CT_UVN,
		vector3(0,0,5),
		NULL,
		&g_camera_target_pos,
		&init_v,
		0,
		-1,
		-100,
		PI/2,
		600,
		400
		);

	return 1;
}
int32_t g_drawed = 0;


float g_angle_y = 0;
float g_angle_x = 0;

 

int32_t Game_Main()
{
	 
	StartClock();
	FillSurface(ARGB(0,0,0,0));
	
	g_angle_y += 0.01f;
	if (g_angle_y>2*PI)
	{
		g_angle_y = 0.0;
	}
	g_angle_y = mod_pi(g_angle_y);

	//g_angle_x += 0.003f;
	//g_angle_x = mod_pi(g_angle_x);
	if (g_angle_x>2*PI)
	{
		g_angle_x -= 2*PI;
	}
	//printf("g_angle_x %f \n",g_angle_x / PI * 360);
	//printf("g_angle_y %f \n",g_angle_y / PI * 360);

	Matrix3 m_rotate3x3 = get_matrix_from_x_y_z_axis_angle(g_angle_x,g_angle_y,0);
	Matrix4 m_rotate4x4 = m_rotate3x3;
	/*m_rotate4x4 = Matrix4(1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1);*/
	//ObjectTransform(&g_obj,m_rotate4x4,TT_LOCAL_TO_TRANS);
	CameraUpdateMatrix(&g_camera);
	ObjectWorldTransform(&g_obj,TT_TRANS);
	//g_camera.word_pos.x +=0.1f;
	//if (g_camera.word_pos.x>5)
	//	g_camera.word_pos.x =0;
	ObjectCameraTransform(&g_obj,&g_camera);
	ObjectProjectTransform(&g_obj,&g_camera);

	ObjectScreenTransform(&g_obj,&g_camera);
	ObjectDraw(&g_obj,200,100);

 
	//DrawTriangleWithEdgeEquation
	for(int32_t i=0;i<-1000;i++)
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

