// NEW1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "NEW1.h"
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
HWND    hDlg = NULL; // 非模态对话框句柄 
#define MAX_LOADSTRING 100
#define TIMER 192

int show ;
int pause;
int v1 ;
int v2;
int index;                                 //地图的下标
int flag;                                  //标记该画哪个小人了
int up;                                    //标记是否有小人跳起来
int dead;                                  //标记小人是否死亡
int trap;                                  //标记陷阱的位置
int height;                                //小人的高度
int th;                                     //小人每次刷新增加的高度
int v;
int score;
// 小人2 的相关变量
int flag1;
int up1;
int dead1;
int height1;
int th1;
struct Point
{
	int index;
	int hole;
};
Point p[11];
int map[10000];
// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Again(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NEW1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NEW1));
	//PlaySound(TEXT("1.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NEW1));
	wcex.hCursor = LoadCursor(wcex.hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_NEW1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, TEXT("酷跑小游戏"), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1000, 550, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HBITMAP person[4], bkg[2],tobj;
	static HBRUSH BrushPenson[4], BrushBkg[2],Brushobj;
	static RECT r,robj;
	static int IBkg[2];
	static int iLength;
	static RECT r1, r2;
	r1.left = 0; r1.right = 1000; r1.top = 60; r1.bottom = 550;
	r2.left = 50; r2.right = 1000; r2.top = 0; r2.bottom = 60;
	static HWND ButtonStart, ButtonPause;
	static TCHAR szBuffer[40];
	switch (message)
	{
	case WM_CREATE:
		bkg[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		bkg[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		person[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		person[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		person[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		person[3] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP9));

		tobj = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		Brushobj = CreatePatternBrush(tobj);
		BrushPenson[0] = CreatePatternBrush(person[0]);
		BrushPenson[1] = CreatePatternBrush(person[1]);
		BrushPenson[2] = CreatePatternBrush(person[2]);
		BrushPenson[3] = CreatePatternBrush(person[3]);
		BrushBkg[0] = CreatePatternBrush(bkg[0]);
		BrushBkg[1] = CreatePatternBrush(bkg[1]);

		ButtonStart = CreateWindow(TEXT("BUTTON"), TEXT("开始"), WS_VISIBLE | WS_CHILD  | BS_PUSHBUTTON,
			0, 0, 50, 30, hWnd, (HMENU)ID_START, hInst, NULL);
		ButtonPause = CreateWindow(TEXT("BUTTON"), TEXT("暂停"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
			0, 30, 50, 30, hWnd, (HMENU)ID_PAUSE, hInst, NULL);

		r.bottom = 550; r.left = 0; r.right = 1000; r.top = 0;
		robj.bottom = 550; robj.top = 360;
		v = 5;                                //背景移动的速度
		IBkg[0] = 0; IBkg[1] = 1000;
		for (int j = 0; j < 11; j++)  //初始化地图
			{
				p[j].hole = 0;
				p[j].index = 100 * j;
			}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_LOWLEVEL:
			v = 2;
			break;
		case ID_MIDLEVEL:
			v = 5;
			break;
		case ID_HIGHLEVEL:
			v = 10;
			break;
		case ID_START:
			SetFocus(hWnd);
			SetTimer(hWnd, TIMER, 10, NULL);
			if (pause == 0)
			{
				score = 0;
				index = 0;                                     //移动到地图的哪个位置了
				v1 = 1;
				v2 = v;
				th = v2;                                         //小人每次刷新增加的高度
				dead = 0;                                         //判断小人是否死亡
				height = 0;                                       //小人的高度
				flag = 0;
				up = 0;                                           //小人是否跳起来

				flag1 = 0;
				up1 = 0;
				dead1 = 0;
				height1 = 0;
				th1 = v2;

				for (int j = 0; j < 11; j++)  //初始化地图
				{
					p[j].hole = 0;
					p[j].index = 100 * j;
				}
				for (int i = 2; i < 10000; i++)  //生成地图
				{
					map[i] = (rand() & 1);  //map[i]=1  代表此处是坑
					if (map[i - 1] == 1 || map[i - 2] == 1)
						map[i] = 0;
				}
			}
			pause = 0;
			break;
		case ID_PAUSE:
			pause = 1;
			if (dead1 == 1 || dead == 1)
				pause = 0;
			KillTimer(hWnd, TIMER);
			break;
		case ID_SHOW:
			show = 1;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			if (pause == 0)
			up1 = 1;
			show = 0;
			break;
		case VK_UP:
			if (pause == 0)
			up = 1;
			show = 0;
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		HDC hdcMem, hdcMem1, hdcMem2, hdcMem3;
		HBITMAP bitmap, bitmap1,bitmap2,bitmap3;
		HBRUSH brush;
		brush = CreateSolidBrush(GetBkColor(hdc));
		hdcMem = CreateCompatibleDC(hdc);
		bitmap = CreateCompatibleBitmap(hdc, 1000, 550);
		SelectObject(hdcMem, bitmap);
		FillRect(hdcMem, &r,brush);//刷干净画布
		//画背景
		hdcMem2 = CreateCompatibleDC(hdc);
		bitmap2 = CreateCompatibleBitmap(hdc, 1000, 550);
		SelectObject(hdcMem2, bitmap2);
		FillRect(hdcMem2, &r, brush);
		FillRect(hdcMem2, &r, BrushBkg[0]);

		hdcMem3 = CreateCompatibleDC(hdc);
		bitmap3 = CreateCompatibleBitmap(hdc, 1000, 550);
		SelectObject(hdcMem3, bitmap3);
		FillRect(hdcMem3, &r, brush);
		FillRect(hdcMem3, &r, BrushBkg[1]);

		BitBlt(hdcMem, IBkg[0], 0, 1000, 550, hdcMem2, 0, 0,SRCCOPY);
		BitBlt(hdcMem, IBkg[1], 0, 1000, 550, hdcMem3, 0, 0, SRCCOPY);



		//画小人
		hdcMem1 = CreateCompatibleDC(hdc);
		bitmap1 = CreateCompatibleBitmap(hdc, 1000, 550);
		SelectObject(hdcMem1, bitmap1);
		FillRect(hdcMem1, &r, brush);

		if (flag>=0&&flag<=200)
		{
			FillRect(hdcMem1, &r, BrushPenson[1]);
			BitBlt(hdcMem, 400, 270-height, 100, 100, hdcMem1, 0, 0, SRCPAINT);
			FillRect(hdcMem1, &r, brush);
			FillRect(hdcMem1, &r, BrushPenson[0]);
			BitBlt(hdcMem, 400, 270-height, 100, 100, hdcMem1, 0, 0, SRCAND);
			flag+=10;
		}
		else
		{
			FillRect(hdcMem1, &r, brush);
			FillRect(hdcMem1, &r, BrushPenson[2]);
			BitBlt(hdcMem, 400, 270-height, 100, 100, hdcMem1, 0, 0, SRCPAINT);
			FillRect(hdcMem1, &r, brush);
			FillRect(hdcMem1, &r, BrushPenson[3]);
			BitBlt(hdcMem, 400, 270-height, 100, 100, hdcMem1, 0, 0, SRCAND);
			flag+=10;
			if (flag >= 400)
				flag = 0;
		}


		//画小人2
		if (flag1 >= 0 && flag1 <= 200)
		{
			FillRect(hdcMem1, &r, brush);
			FillRect(hdcMem1, &r, BrushPenson[2]);
			BitBlt(hdcMem, 200, 270-height1, 100, 100, hdcMem1, 0, 0, SRCPAINT);
			FillRect(hdcMem1, &r, brush);
			FillRect(hdcMem1, &r, BrushPenson[3]);
			BitBlt(hdcMem, 200, 270-height1, 100, 100, hdcMem1, 0, 0, SRCAND);
			flag1 += 10;
		}
		else
		{
			FillRect(hdcMem1, &r, brush);
			FillRect(hdcMem1, &r, BrushPenson[1]);
			BitBlt(hdcMem, 200, 270-height1 , 100, 100, hdcMem1, 0, 0, SRCPAINT);
			FillRect(hdcMem1, &r, brush);
			FillRect(hdcMem1, &r, BrushPenson[0]);
			BitBlt(hdcMem, 200, 270-height1, 100, 100, hdcMem1, 0, 0, SRCAND);
			flag1 += 10;
			if (flag1 >= 400)
				flag1 = 0;
		}

		//画下面运动的物体
		for (int k = 0; k < 11; k++)
		{
			if (p[k].hole == 0)
			{
				robj.left = p[k].index;
				robj.right = robj.left + 100;
				FillRect(hdcMem, &robj, brush);
				FillRect(hdcMem, &robj, Brushobj);
			}
		}
		iLength = wsprintf(szBuffer, TEXT("%i"), score);
		TextOut(hdcMem, 0, 60, szBuffer, iLength);

		BitBlt(hdc, 0, 0, 1000, 500, hdcMem, 0, 0, SRCCOPY);
		DeleteDC(hdcMem);
		DeleteDC(hdcMem1);
		DeleteDC(hdcMem2);
		DeleteDC(hdcMem3);
		DeleteObject(bitmap);
		DeleteObject(brush);
		DeleteObject(bitmap1);
		DeleteObject(bitmap2);
		DeleteObject(bitmap3);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteObject(person[0]);
		DeleteObject(person[1]);
		DeleteObject(person[2]);
		DeleteObject(person[3]);
		DeleteObject(BrushPenson[0]);
		DeleteObject(BrushPenson[1]);
		DeleteObject(BrushPenson[2]);
		DeleteObject(BrushPenson[3]);
		DeleteObject(bkg[0]);
		DeleteObject(bkg[1]);
		DeleteObject(tobj);
		DeleteObject(Brushobj);
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		score +=v2;
		for (int i = 0; i < 2; i++)   //背景变化
		{
			IBkg[i] -= v1;
			if (IBkg[i] == -1000)
				IBkg[i] = 1000;
		}
		for (int j = 0; j < 11; j++)//下面移动物体的变化
		{
			p[j].index -= v2;
			if (p[j].index == -100)
			{
				p[j].index = 1000;
				p[j].hole = map[index];
				index++;
			}
		}
		
		for ( int i = 0; i < 11;i++)             
		{
			trap = 1000;
			if ( p[i].hole == 1)             
			{
				trap = p[i].index;
				if (show == 0)
				{
					int t = 450 - trap;
					if (t>10 && t < 90 && up == 0)    //370 - 430  陷阱范围
						dead = 1;
					int t1 = 250 - trap;
					if (t1>10 && t1 < 90 && up1 == 0)    //170 - 230  陷阱范围
						dead1 = 1;
				}
				else
				{
					if (trap == 460)
						up = 1;
					if (trap == 260)
						up1 = 1;
				}
			}
		}
		if (up == 1&&dead==0)                 //如果已经死亡，是不会跳起来的
		{
			height += th;
			if (height == 100)
				th = -th;
			if (height == 0)
			{
				th = -th;
				up = 0;
			}
		}
		if (dead == 1)
		{
			v1 = 0;
			v2 = 0;
			th = 20;
			height -= th;
			if (height == -200)
			{
				v2 = v;
				KillTimer(hWnd, TIMER);
			}
		}


		if (up1 == 1 && dead1 == 0)                 //如果已经死亡，是不会跳起来的
		{
			height1 += th1;
			if (height1 ==  100)
				th1 = -th1;
			if (height1 == 0)
			{
				th1 = -th1;
				up1 = 0;
			}
		}
		if (dead1 == 1)
		{
			v1 = 0;
			v2 = 0;
			th1 = 20;
			height1 -= th1;
			if (height1 == -200)
			{
				v2 = v;
				KillTimer(hWnd, TIMER);
			}
		}
		if (height == -200 || height1 == -200)
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1),
				hWnd, Again);

		InvalidateRect(hWnd, &r1, FALSE);
		InvalidateRect(hWnd, &r2, FALSE);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
INT_PTR  CALLBACK Again(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND	hwnd;
	hwnd = GetParent(hDlg);
	switch (message)
	{
	case WM_INITDIALOG:
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			SendMessage(hwnd, WM_COMMAND, ID_START, ID_START);
			InvalidateRect(hwnd, NULL, TRUE); //使对话框的父窗口无效
			EndDialog(hDlg, LOWORD(wParam));		// 清除非模态对话框
			hDlg = NULL;			// 将全局变量设置为
			return TRUE;
		case IDCANCEL:
			SendMessage(hwnd,WM_DESTROY, 0, 0);
			EndDialog(hDlg, LOWORD(wParam));	// 清除非模态对话框
			hDlg = NULL;
			return TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
