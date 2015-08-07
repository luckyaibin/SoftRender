#include "Console.h"
#include "vector.h"
#include <Windows.h>
#include <iostream>
//#include "tool.h"
#include "quaternion.h"
#include "euler_rotate.h"
#include "model.h"
#include "../thirdparty/include/libpng/png.h"

#include "texture/texture.h"
#include "crc/crc32.h"

//宏定义
#define WINDOW_CLASS_NAME TEXT("CPPYIN3DWNDCLS")
#define WINDOW_TITLE TEXT("engine")
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32
#define WAIT_TIME 10

	// 全局变量定义
HWND g_WindowHandle;
HINSTANCE g_HInstance;
DWORD g_Clock;

uint32_t g_curr_frame_count = 0;
char g_FPS_str[20];
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
Texture g_texture;
ID3DXFont* g_font=0;
int32_t Game_Init()
{
	Init3DLib(g_HInstance, g_WindowHandle, SCREEN_WIDTH, SCREEN_HEIGHT);
	load_texture_png(&g_texture,"test.png");
	AllocConsole();
	freopen("conin$","r",stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);
	HWND consoleHandle = GetConsoleWindow();
	MoveWindow(consoleHandle,400,1,680,480,1);
	printf("[software renderer] Console initialized.\n");

	D3DXFONT_DESC lf;
	ZeroMemory(&lf, sizeof(D3DXFONT_DESC));
	lf.Height         = 25;    // in logical units
	lf.Width          = 12;    // in logical units 
	lf.Weight         = 500;   // boldness, range 0(light) - 1000(bold)
	lf.Italic         = false;
	lf.CharSet        = DEFAULT_CHARSET;
	lf.Quality        = 0;
	lf.PitchAndFamily = 0;
	strcpy(lf.FaceName, "Times New Roman"); // font style
	//
	// Create an ID3DXFont based on 'lf'.
	D3DXCreateFontIndirect(pDevice, &lf, &g_font);
	 
 
	int la = GetLastError();
	ObjectInit(&g_obj);
	//g_camera.world_pos = vector3(0,0,5);

	vector3 init_v(0,1,0);
	InitUVNCamera(&g_camera,
		CT_UVN,
		vector3(0,0,0),
		NULL,
		&g_obj.world_coord,
		&init_v,
		0,
		-1,
		-100,
		PI/2,
		800,
		600
		);

	return 1;
}
int32_t g_drawed = 0;


 
 
float g_diff = 0.1;

float g_x_value = 2.0f;
float g_y_value = 0;
float g_z_value = 0;


int32_t Game_Main()
{
	StartClock();
	FillSurface(ARGB(0,0,0,0));
	
	Texture t;
	//load_texture_png(&t,"test.png");
	int row=0;
	for (row=0;row<g_texture.h;)
	{
		for (int col=0;col<g_texture.w;)
		{
			
			unsigned int r;
			unsigned int g;
			unsigned int b;
			unsigned int a;
			unsigned char * row_pointer = g_texture.row_pointers[row];
			
			r = g_texture.row_pointers[row][4*col+0];
			g = g_texture.row_pointers[row][4*col+1];
			b = g_texture.row_pointers[row][4*col+2];
			a = g_texture.row_pointers[row][4*col+3];
			
			DrawPixel(col,row, ARGB(a,r,g,b));
			col += 1;
		}
		row++;
	}
	 
	if (KEY_DOWN('S'))
	{
		g_x_value += 0.01;
	}
	if (KEY_DOWN('X'))
	{
		g_x_value -= 0.01;
	}

	if (KEY_DOWN('7'))
	{
		g_y_value += 0.01;
	}
	if (KEY_DOWN('Y'))
	{
		g_y_value -= 0.01;
	}
	if (KEY_DOWN('A'))
	{
		g_z_value += 0.01;
	}
	if (KEY_DOWN('Z'))
	{
		g_z_value -= 0.01;
	}


	if (KEY_DOWN('R'))
	{
		g_x_value = 1;
		g_y_value = 1;
		g_z_value = 1;
	}

	g_camera.world_pos.x = g_x_value;
	g_camera.world_pos.y = g_y_value;
	g_camera.world_pos.z = g_z_value;
	
	//重置坐标轴的坐标，防止计算的误差积累
	for (int i=0;i<6;i++)
	{
		g_obj.obj_coords_transformed[i] = vertex3d(0,0,0,0);
	}
	/*char tip_coord[200]={0};
	sprintf(tip_coord," x:%f,y:%f,z:%f",g_x_value,g_y_value,g_z_value);
	memset(g_FPS_str,0,sizeof(g_FPS_str));
	strcat(&g_FPS_str[0],tip_coord);*/

	pDevice->BeginScene();

	show_frame();

	CameraUpdateMatrix(&g_camera);

	ObjectWorldTransform(&g_obj,TT_LOCAL_TO_TRANS);

	ObjectDeleteBackface(&g_obj,&g_camera);

	ObjectCameraTransform(&g_obj,&g_camera);
	ObjectProjectTransform(&g_obj,&g_camera);
	ObjectScreenTransform(&g_obj,&g_camera);
	ObjectDraw(&g_obj,&g_camera,SCREEN_WIDTH,SCREEN_HEIGHT);
	pDevice->EndScene();
	pDevice->Present(NULL, NULL, NULL, NULL);
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

