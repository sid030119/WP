#define _CRT_SECURE_NO_WARNIGNS

#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <string>




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
		500,
		800,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	hwnd2 = CreateWindow(
		_T("Option"),
		_T("Option"),
		WS_OVERLAPPEDWINDOW,
		500,
		0,
		500,
		800,
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
	static HBITMAP hBitmap, hBase;

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
		SelectObject(memdc, hBase);

		Rectangle(hdc, 10, 10, 100, 50);
		TextOut(hdc, 15, 20, _T("옷 출력"), _tcslen(_T("옷 출력")));

		StretchBlt(hdc, 0, 100, 500, 500, memdc, 0, 0, 800, 800, SRCCOPY);

		i = 0;
		while (acc[i].acc_num) {
			for (int j = 0; j < 6; j++)
			{
				if (acc[i].acc_num == j + 1)hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc1 + j));		//else if 문 j for문 식으로 줄였습니다
			}
			
			SelectObject(memdc, hBitmap);
			StretchBlt(hdc, acc[i].x, acc[i].y, 100, 100, memdc, 0, 0, 500, 500, SRCCOPY);
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

		}
	case WM_COPYDATA:
		acc_num = lParam;
		if (acc_num != 0) {
			acc[acc_cnt].acc_num = acc_num;
			acc[acc_cnt].x = rand() % 500;
			acc[acc_cnt].y = rand() % 500;
			acc_cnt++;
			InvalidateRgn(hwnd, NULL, true);
		}
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (i = 0; i < acc_cnt; i++) {
			if (acc[i].x <= mx && mx <= acc[i].x + 100 && acc[i].y <= my && my <= acc[i].y + 100) {
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

		Rectangle(hdc, 50, 10, 450, 50);
		TextOut(hdc, 150, 15, _T("악세서리"), _tcslen(_T("악세서리")));
		hBitmap = LoadBitmap(hInst,	MAKEINTRESOURCE(IDB_acc1));
		SelectObject(memdc, hBitmap);
		StretchBlt(hdc, 50, 60, 100, 100, memdc, 0, 0, 500, 500, SRCCOPY);
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc2));
		SelectObject(memdc, hBitmap);
		StretchBlt(hdc, 200, 60, 100, 100, memdc, 0, 0, 500, 500, SRCCOPY);
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc3));
		SelectObject(memdc, hBitmap);
		StretchBlt(hdc, 350, 60, 100, 100, memdc, 0, 0, 500, 500, SRCCOPY);
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc4));
		SelectObject(memdc, hBitmap);
		StretchBlt(hdc, 50, 130, 100, 100, memdc, 0, 0, 770, 330, SRCCOPY);
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc5));
		SelectObject(memdc, hBitmap);
		StretchBlt(hdc, 200, 130, 100, 100, memdc, 0, 0, 500, 500, SRCCOPY);

		DeleteDC(memdc);
		
		EndPaint(hwnd, &ps);


	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (50 <= mx && mx <= 150 && 60 <= my && my <= 160)ACC_NUM = 1;
		else if (200 <= mx && mx <= 300 && 60 <= my && my <= 160)ACC_NUM = 2;
		else if (350 <= mx && mx <= 450 && 60 <= my && my <= 160)ACC_NUM = 3;
		else if (50 <= mx && mx <= 150 &&  130<= my && my <= 230)ACC_NUM = 4;
		else if (200 <= mx && mx <= 300 && 130 <= my && my <= 230)ACC_NUM = 5;
		if(ACC_NUM!=0)
		SendMessage(Print, WM_COPYDATA, 0, ACC_NUM);

		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}