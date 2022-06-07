#define _CRT_SECURE_NO_WARNIGNS

#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <string>
#include <wingdi.h>

#pragma comment(lib,"msimg32.lib")


LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc2(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;



struct accesory {
	int x=0;
	int y=0;
	int acc_num = 0;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdLine, int nCmdShow)
{
	HWND hwnd, hwnd2;
	MSG msg;
	WNDCLASS WndClass, WndClass2;
	hInst = hInstance;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDC_WP7);
	WndClass.lpfnWndProc = ChildWndProc;
	WndClass.lpszClassName = _T("Print");
	if(!RegisterClass(&WndClass)) return 1;

	WndClass2 = WndClass;
	WndClass2.lpszMenuName = NULL;
	WndClass2.lpszClassName = _T("Option");
	WndClass2.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass2.lpfnWndProc = ChildWndProc2;
	if (!RegisterClass(&WndClass2))return 1;

	hwnd = CreateWindow(
		_T("Print"),
		_T("Print"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		840,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	hwnd2 = CreateWindow(
		_T("Option"),
		_T("Option"),
		WS_OVERLAPPEDWINDOW,
		840,
		0,
		500,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);


	ShowWindow(hwnd, nCmdShow);
	ShowWindow(hwnd2, nCmdShow);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap, hBase, hBG;
	POINT ptMouse;

	static bool Selection = false;

	static accesory acc[20];
	static int acc_cnt = 0;
	static int acc_num = 0;
	int i = 0;
	int mx, my;

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		hBG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BG));
		SelectObject(memdc, hBG);
		StretchBlt(hdc, 0, 0, 840, 543, memdc, 0, 0, 605, 432,SRCCOPY );

		Rectangle(hdc, 10, 10, 100, 50);
		TextOut(hdc, 15, 20, _T("옷 출력"), _tcslen(_T("옷 출력")));

		SelectObject(memdc, hBase);

		TransparentBlt(hdc, 250, 100, 350, 350, memdc, 0, 0, 800, 700, RGB(42, 255, 0));

		GetCursorPos(&ptMouse);

		if (120 <= ptMouse.x&& ptMouse.x < 220 && 370 <= ptMouse.y&& ptMouse.y <= 470) hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TRASH_ON));
		else hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TRASH_OFF));
		SelectObject(memdc, hBitmap);
		TransparentBlt(hdc, 120, 370, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

		i = 0;
		while (acc[i].acc_num) {
			for (int j = 0; j < 8; j++)
			{
				if (acc[i].acc_num == j + 1)hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc1 + j));		//else if 문 j for문 식으로 줄였습니다
			}
			
			SelectObject(memdc, hBitmap);
			TransparentBlt(hdc, acc[i].x, acc[i].y, 100, 100, memdc, 0, 0, 500, 500, RGB(42,255,0));
			i++;
		}

		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_S_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_0));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_S_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_1));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_S_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_2));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_TS_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_0));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_TS_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_1));
			InvalidateRgn(hwnd, NULL, true);
		case ID_TS_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_2));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_P_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_P_0));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_P_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_P_1));
			InvalidateRgn(hwnd, NULL, true);
		case ID_P_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_P_2));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_SK_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_SK_0));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_SK_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_SK_1));
			InvalidateRgn(hwnd, NULL, true);
		case ID_SK_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_SK_2));
			InvalidateRgn(hwnd, NULL, true);
			break;

		}
	case WM_COPYDATA:
		acc_num = lParam;
		if (acc_num != 0 && acc_cnt<19) {
			acc[acc_cnt].acc_num = acc_num;
			acc[acc_cnt].x = rand() % 440+200;
			acc[acc_cnt].y = rand() % 300+100;
			acc_cnt++;
			InvalidateRgn(hwnd, NULL, true);
		}
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (i = 0; i < acc_cnt; i++) {
			if (acc[i].x+20 <= mx && mx <= acc[i].x + 80 && acc[i].y+20 <= my && my <= acc[i].y + 80) {
				Selection = true;
				acc_num = i;
				InvalidateRgn(hwnd, NULL, true);
				break;
			}

		}
		break;
	
	case WM_LBUTTONUP:
		InvalidateRgn(hwnd, NULL, true);
		Selection = false;

		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (120 <= mx && mx < 220 && 370 <= my && my <= 470) {
			acc[acc_num].acc_num = acc[acc_cnt-1].acc_num;
			acc[acc_num].x = acc[acc_cnt-1].x;
			acc[acc_num].y = acc[acc_cnt-1].y;
			acc[acc_cnt-1].acc_num = 0;
			acc_cnt--;
		}




		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (Selection)
		{
			acc[acc_num].x = mx-50;
			acc[acc_num].y = my-50;
			InvalidateRgn(hwnd, NULL, true);
		}
		break;




	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc2(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{	
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap;
	int mx, my;
	int ACC_NUM = 0;

	HWND Print = ::FindWindow(NULL, "Print");


	switch (iMsg)
	{
		
		

	case WM_CREATE:


		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);


		Rectangle(hdc, 45, 10, 455, 50);
		TextOut(hdc, 150, 15, _T("악세서리"), _tcslen(_T("악세서리")));
		hBitmap = LoadBitmap(hInst,	MAKEINTRESOURCE(IDB_acc1));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 45, 55, 155, 165);
		TransparentBlt(hdc, 50, 60, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0) );
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc2));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 195, 55, 305, 165);
		TransparentBlt(hdc, 200, 60, 100, 100, memdc, 0, 0, 500, 500, RGB(42,255,0));
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc3));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 345, 55, 455, 165);
		TransparentBlt(hdc, 350, 60, 100, 100, memdc, 0, 0, 500, 500, RGB(42,255,0));
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc4));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 45, 175, 155, 285);
		TransparentBlt(hdc, 50, 180, 100, 100, memdc, 0, 0, 500, 500, RGB(42,255,0));
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc5));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 195, 175, 305, 285);
		TransparentBlt(hdc, 200, 180, 100, 100, memdc, 0, 0, 500, 500, RGB(42,255,0));
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc6));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 345, 175, 455, 285);
		TransparentBlt(hdc, 350, 180, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc7));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 45, 295, 155, 405);
		TransparentBlt(hdc, 50, 300, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc8));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 195, 295, 305, 405);
		TransparentBlt(hdc, 200, 300, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc8));
		SelectObject(memdc, hBitmap);
		Rectangle(hdc, 345, 295, 455, 405);
		TransparentBlt(hdc, 350, 300, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

		DeleteDC(memdc);
		
		EndPaint(hwnd, &ps);


	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (50 <= mx && mx <= 150 && 60 <= my && my <= 160)ACC_NUM = 1;
		else if (200 <= mx && mx <= 300 && 60 <= my && my <= 160)ACC_NUM = 2;
		else if (350 <= mx && mx <= 450 && 60 <= my && my <= 160)ACC_NUM = 3;
		else if (50 <= mx && mx <= 150 &&  180<= my && my <= 280)ACC_NUM = 4;
		else if (200 <= mx && mx <= 300 && 180 <= my && my <= 280)ACC_NUM = 5;
		else if (350 <= mx && mx <= 450 && 180 <= my && my <= 280)ACC_NUM = 6;
		else if (50 <= mx && mx <= 150 && 300 <= my && my <= 400)ACC_NUM = 7;
		else if (200 <= mx && mx <= 300 && 300 <= my && my <= 400)ACC_NUM = 8;
		else if (350 <= mx && mx <= 450 && 300 <= my && my <= 400)ACC_NUM = 9;

		if(ACC_NUM!=0)
		SendMessage(Print, WM_COPYDATA, 0, ACC_NUM);

		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}