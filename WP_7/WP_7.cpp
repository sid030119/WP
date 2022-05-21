#include <windows.h>
#include <TCHAR.H>
#include "resource.h"

LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc2(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;



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
	static HBITMAP hBitmap;

	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);

		Rectangle(hdc, 10, 10, 100, 50);
		TextOut(hdc, 15, 20, _T("옷 출력"), _tcslen(_T("옷 출력")));

		StretchBlt(hdc, 0, 100, 500, 500, memdc, 0, 0, 800, 800, SRCCOPY);
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_S_0:
			hBitmap = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_0));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_S_1:
			hBitmap = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_1));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_S_2:
			hBitmap = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_2));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_TS_0:
			hBitmap = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_0));
			InvalidateRgn(hwnd, NULL, true);
			break;
		case ID_TS_1:
			hBitmap = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_1));
			InvalidateRgn(hwnd, NULL, true);
		case ID_TS_2:
			hBitmap = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_2));
			InvalidateRgn(hwnd, NULL, true);
			break;

		}

	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc2(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
		HDC hdc;
		PAINTSTRUCT ps;
	case WM_CREATE:


		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);

		Rectangle(hdc, 10, 10, 410, 50);
		TextOut(hdc, 150, 15, _T("악세서리"), _tcslen(_T("악세서리")));
		Rectangle(hdc, 10, 80, 410, 120);
		Rectangle(hdc, 10, 150, 410, 190);
		Rectangle(hdc, 10, 220, 410, 260);
		Rectangle(hdc, 10, 290, 410, 330);

		EndPaint(hwnd, &ps);


	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}