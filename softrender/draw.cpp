#include "draw.h"

bool Init3DLib(HINSTANCE hInstance, HWND hWnd, int width, int height)
{
	IDirect3D9* d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow              = hWnd;
	d3dpp.Windowed                   = TRUE;
	d3dpp.EnableAutoDepthStencil     = true;
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	long err =d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDevice);
	d3d9->Release();

	err = pDevice->CreateOffscreenPlainSurface(width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pSurface, 0);
	//printf("%d",err);
	return true;
}

void FlipSurface()
{
	// 获取后台缓存
	IDirect3DSurface9* backBuffer = 0;
	pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// 使用自定义表面填充后台缓存
	pDevice->StretchRect(pSurface, 0, backBuffer, 0, D3DTEXF_NONE);

	// GetBackBuffer所得的缓存需要被释放，否则会内存泄露
	backBuffer->Release();

	// 将交换链中的后台缓存显示
	pDevice->Present(0, 0, 0, 0);
}

void Release3DLib()
{
	pSurface->Release();
	pDevice->Release();
}

int DrawPixel(int x,int y, ARGB color)
{
	// 创建并初始化锁定区域
	D3DLOCKED_RECT lr;
	memset(&lr, 0, sizeof(lr));

	// 锁定
	pSurface->LockRect(&lr,NULL,D3DLOCK_DISCARD);

	// 像素着色
	DWORD* pBits = (DWORD*)lr.pBits;
	//printf("pitch:::%d x:%d,y:%d\n",lr.Pitch,x,y);
	pBits[x + y * (lr.Pitch >> 2)] = color;

	// 解锁
	pSurface->UnlockRect();

	return 1;
}
//Bresenham's algorithm 
//不需要浮点数的Bresenham 算法
void DrawLine(int x0,int y0,int x1,int y1,ARGB color)
{
	//printf("x0:%d,y0:%d,x1:%d,y1:%d\n",x0,y0,x1,y1);
	int steep = abs(y1-y0) > abs(x1-x0);
	if(steep)//如果斜率绝对值大于1，交换x和y，变成斜率绝对值小于1的
	{
		MACRO_SWAP(x0,y0,int);
		MACRO_SWAP(x1,y1,int);
	}
	if(x0 > x1)//如果反方向（从右上至左下）,交换起点和终点
	{
		MACRO_SWAP(x0,x1,int);
		MACRO_SWAP(y0,y1,int);
	}

	//printf("x0:%d,y0:%d,x1:%d,y1:%d\n",x0,y0,x1,y1);
	int deltax =  x1 - x0;
	int deltay =  y1 - y0;
	int error = 0;
	int deltaerr =  abs(deltay);
	int ystep;
	if (y0<y1)
		ystep = 1;
	else
		ystep = -1;

	int y = y0;
	int x;
	for (x = x0;x<=x1;x++)
	{
		if (steep)//x 和 y 坐标是调换过了，绘制的时候要再调换回去
			DrawPixel(y,x,color);
		else
			DrawPixel(x,y,color);
		error = error + deltaerr;
		if (2*error>=deltax)
		{
			y= y + ystep;
			error = error - deltax;
		}
	}
}

//Bresenham's algorithm 
void DrawLine_with_float(int x0,int y0,int x1,int y1,ARGB color)
{
	printf("x0:%d,y0:%d,x1:%d,y1:%d\n",x0,y0,x1,y1);
	int steep = abs(y1-y0) > abs(x1-x0);
	if(steep)//如果斜率绝对值大于1，交换x和y，变成斜率绝对值小于1的
	{
		MACRO_SWAP(x0,y0,int);
		MACRO_SWAP(x1,y1,int);
	}
	if(x0 > x1)//如果反方向（从右上至左下）,交换起点和终点
	{
		MACRO_SWAP(x0,x1,int);
		MACRO_SWAP(y0,y1,int);
	}

	printf("x0:%d,y0:%d,x1:%d,y1:%d\n",x0,y0,x1,y1);
	float deltax =  x1 - x0;
	float deltay =  y1 - y0;
	float error = 0;
	float deltaerr =  abs(deltay / deltax);
	int ystep;
	if (y0<y1)
		ystep = 1;
	else
		ystep = -1;

	int y = y0;
	int x;
	for (x = x0;x<=x1;x++)
	{
		if (steep)//x 和 y 坐标是调换过了，绘制的时候要再调换回去
			DrawPixel(y,x,color);
		else
			DrawPixel(x,y,color);
		error = error + deltaerr;
		if (error>=0.5)
		{
			y= y + ystep;
			error = error - 1.0;
		}
	}
}

int DrawPartCircle(int centerx,int centery,int x,int y,ARGB color)
{
	DrawPixel(centerx + x,centery + y,color);
	DrawPixel(centerx + x,centery - y,color);
	DrawPixel(centerx - x,centery + y,color);
	DrawPixel(centerx - x,centery - y,color);
	DrawPixel(centerx + y,centery + x,color);
	DrawPixel(centerx + y,centery - x,color);
	DrawPixel(centerx - y,centery + x,color);
	DrawPixel(centerx - y,centery - x,color);
	return 0;
}

int DrawCircle(int xc,int yc,int r,ARGB color)
{
	int x=0,y=r;
	int d = 3 - 2*r;
	while(x<y)
	{
		DrawPartCircle(xc,yc,x,y,color);
		x = x + 1;
		if (d < 0)
		{
			d = d + 4*x + 6;
		}
		else
		{
			y = y - 1;
			d = d + 4*(x-y) + 10;
		}
	}
	DrawPartCircle(xc,yc,x,y,color);
	return 0;
}
/*
top triangle:
		p0(x0,y0)
		/\
	   /  \
	  ------
P1(x1,y1)  P2(x2,y2)
  */
int DrawTriangle(  vertex2d v1,   vertex2d v2,  vertex2d v3,ARGB color)
{
	float x1 = v1.x;
	float y1 = v1.y;
	float x2 = v2.x;
	float y2 = v2.y;
	float x3 = v3.x;
	float y3 = v3.y;

	int minx = (int)MACRO_MIN3(x1,x2,x3);
	int maxx = (int)MACRO_MAX3(x1,x2,x3);
	int miny = (int)MACRO_MIN3(y1,y2,y3);
	int maxy = (int)MACRO_MAX3(y1,y2,y3);

	for(int y = miny; y < maxy; y++)
	{
		for(int x = minx; x < maxx; x++)
		{
			// When all half-space functions positive, pixel is in triangle
			if((x1 - x2) * (y - y1) - (y1 - y2) * (x - x1) > 0 &&  (x2 - x3) * (y - y2) - (y2 - y3) * (x - x2) > 0 && (x3 - x1) * (y - y3) - (y3 - y1) * (x - x3) > 0)
			{
				DrawPixel(x,y,color);
			}
		}
	}
	return 0;
}

int DrawPolygon2d(polygon2d * p)
{
	return 0;
}



void fillBottomFlatTriangle(vertex2d v1,vertex2d v2,vertex2d v3,ARGB color)
{
	float invslop1 = (v2.x - v1.x) / (v2.y - v1.y);
	float invslop2 = (v3.x - v1.x) / (v3.y - v1.y);
	float curx1 = v1.x;
	float curx2 = v1.x;
	for (int scalineY = v1.y;scalineY <= v2.y;scalineY++)
	{
		DrawLine(curx1,scalineY,curx2,scalineY,color);
		curx1 += invslop1;
		curx2 += invslop2;
	}
}

void fillTopFlatTriangle(vertex2d v1,vertex2d v2,vertex2d v3,ARGB color)
{
	float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
	float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

	float curx1 = v3.x;
	float curx2 = v3.x;

	for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
	{
		curx1 -= invslope1;
		curx2 -= invslope2;
		DrawLine((int)curx1, scanlineY, (int)curx2, scanlineY,color);
	}
}

void sort_vertex_by_Y(vertex2d v1,vertex2d v2,vertex2d v3,vertex2d * output_v1,vertex2d * output_v2,vertex2d * output_v3)
{
	vertex2d vv1,vv2,vv3;
	// v1 < v2 < v3
	if (v1.y <= v2.y && v2.y <= v3.y)
		vv1 = v1,vv2 = v2,vv3 = v3;
	// v1 < v3 < v2
	else if (v1.y <= v3.y && v3.y <= v2.y)
		 vv1 = v1,vv2=v3,vv3=v2;
	// v2 < v1 < v3
	else if (v2.y <= v1.y && v1.y <= v3.y)
		vv1 = v2,vv2 = v1,vv3 = v3;
	// v2 < v3 < v1
	else if (v2.y <= v3.y && v3.y <= v1.y)
		vv1 = v2,vv2 = v3,vv3 = v1;
	// v3 < v1 < v2
	else if (v3.y <= v1.y && v1.y <= v2.y)
		vv1 = v3,vv2 = v1,vv3 = v2;
	// v3 < v2 < v1
	else if (v3.y <= v2.y && v2.y <= v1.y)
		vv1 = v3,vv2 = v2,vv3 = v1;
	*output_v1 = vv1;
	*output_v2 = vv2;
	*output_v3 = vv3;
}

void drawTriangle(vertex2d v1,vertex2d v2,vertex2d v3,ARGB color)
{
	vertex2d vt1,vt2,vt3;
	sort_vertex_by_Y(v1,v2,v3,&vt1,&vt2,&vt3);
	if (vt2.y == vt3.y)
	{
		fillBottomFlatTriangle(vt1,vt2,vt3,color);
	}
	else if (vt1.y == vt2.y)
	{
		fillTopFlatTriangle(vt1,vt2,vt3,color);
	}
	else
	{
		vertex2d vt4;
		vt4.x = vt1.x + ((float)(vt2.y - vt1.y) / (float)(vt3.y - vt1.y)) * (vt3.x - vt1.x);
		vt4.y = vt2.y;
		fillBottomFlatTriangle(vt1,vt2,vt4,color);
		fillTopFlatTriangle(vt2,vt4,vt3,color);
	}
}


