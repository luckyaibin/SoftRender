#include <d3d9.h>
#include <d3dx9.h>

#include "draw.h"
//#include "DirectX9/Include/d3dx9.h"

extern uint32_t g_curr_frame_count;
extern char		g_FPS_str[20];
extern ID3DXFont* g_font;
void show_frame()
{
	static uint64_t g_last_update_time = GetTickCount();
	uint64_t curr_time = GetTickCount();

	RECT rect;
	rect.left=0;
	rect.bottom = 250;
	rect.right = 120;
	rect.top = 0;

	int re = g_font->DrawText(NULL,
		g_FPS_str, // String to draw.
		-1, // Null terminating string.
		&rect, // Rectangle to draw the string in.
		DT_TOP | DT_LEFT|DT_NOCLIP, // Draw in top-left corner of rect.
		0xffffffff); // Black.
	if (curr_time - g_last_update_time > 1000)
	{
		g_last_update_time = curr_time;
		sprintf(g_FPS_str,"fps:%d",g_curr_frame_count);
		g_curr_frame_count = 0;
	}
	else
	{
		g_curr_frame_count++;
	}
}
