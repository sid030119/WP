#define _CRT_SECURE_NO_WARNIGNS



#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <string>
#include <wingdi.h>
#include <atlstr.h>

#pragma comment(lib,"msimg32.lib")

#pragma comment(lib,"winmm.lib")

#define PRINT_X 1200
#define PRINT_Y 800
#define OPTION_X 501
#define OPTION_Y 800

//함수 선언부
LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc2(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);

HBITMAP ScreenCapture();
void SaveBitmap(HBITMAP hbit, char* Path);



struct accesory {
	int x = 0;
	int y = 0;
	int acc_num = 0;
};

//전역 변수
HINSTANCE hInst;
HBITMAP g_hBit;
static HBITMAP hBase;
int PRICE_CLOTHES = 25000;
int COST_ACC = 0;
const int PRICE_ACC[9] = {0, 3000,2000,5000,5000,4000, 1500, 1500, 7000};

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
	WndClass.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_logo));
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = CreateSolidBrush(RGB(236, 139, 94));
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDC_WP7);
	WndClass.lpfnWndProc = ChildWndProc;
	WndClass.lpszClassName = _T("Print");
	if (!RegisterClass(&WndClass)) return 1;

	WndClass2 = WndClass;
	WndClass2.lpszMenuName = NULL;
	WndClass2.lpszClassName = _T("Option");
	WndClass2.hbrBackground = CreateSolidBrush(RGB(236, 139, 94));
	WndClass2.lpfnWndProc = ChildWndProc2;
	if (!RegisterClass(&WndClass2))return 1;

	hwnd = CreateWindow(
		_T("Print"),
		_T("Print"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		PRINT_X,
		PRINT_Y,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	hwnd2 = CreateWindow(
		_T("Option"),
		_T("Option"),
		WS_OVERLAPPEDWINDOW,
		PRINT_X-15,
		0,
		OPTION_X,
		OPTION_Y,
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

BOOL inTrashcan(int mx, int my) {
	return ((PRINT_X / 8)  <= mx && mx < (PRINT_X / 8)+100 && (PRINT_Y / 2) + 150 <= my && my <= (PRINT_Y / 2) + 250);
}

LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{

	HDC hdc, memdc;
	PAINTSTRUCT ps;
	static HBITMAP hBitmap, hBG;
	POINT ptMouse;

	static bool Selection = false;
	static bool CanTrash = false;

	static accesory acc[20];

	static int acc_cnt = 0;
	static int acc_num = 0;
	int i = 0;
	int mx, my;

	BITMAP bmp;
	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH] = "";

	HFONT font, oldfont;

	HWND Option = ::FindWindow(NULL, "Option");

	switch (iMsg)
	{

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		//hBG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BG));
		



		//배경 이미지 출력
		//SelectObject(memdc, hBG);
		//StretchBlt(hdc, 0, 0, PRINT_X, PRINT_Y, memdc, 0, 0, 1200, 800, SRCCOPY);

		//펜 설정 및 펜사용 부분
		HPEN hPen, oldPen;
		HBRUSH hBrush, oldBrush;

		hPen = CreatePen(PS_SOLID, 3, RGB(204, 204, 204));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		//Rectangle(hdc, 10, 10, 100, 50);

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		

		//폰트설정 및 글씨 사용부분
		/*/
		AddFontResource("SCDream4.otf");

		font = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("에스코어 드림 4 Regular"));
		oldfont = (HFONT)SelectObject(hdc, font);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(0, 0, 0));

		TextOut(hdc, 17, 15, _T("옷 출력"), _tcslen(_T("옷 출력")));

		SelectObject(hdc, oldfont);
		DeleteObject(font);
		RemoveFontResource("SCDream4.otf");
		*/

		// 타이틀 출력
		AddFontResource("Voga-Medium.otf");


		font = CreateFont(50, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("Voga-Medium"));
		oldfont = (HFONT)SelectObject(hdc, font);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(20, 26, 70));
		TextOut(hdc, 540, 15, _T("Dress Up !"), _tcslen(_T("Dress Up !")));




		//하얀 배경 출력
		hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		
	
		Rectangle(hdc, 150, 100, 1020, 640);

		


		//기본 옷 출력

		if (hBase == NULL) {
			hBase = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_S_0));
		}
		SelectObject(memdc, hBase);

		TransparentBlt(hdc, (PRINT_X/2)-300, (PRINT_Y/2)-320, 600, 600, memdc, 0, 0, 800, 700, RGB(42, 255, 0));

		//쓰레기통 출력
		if (CanTrash)
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TRASH_ON));
		else
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TRASH_OFF));

		SelectObject(memdc, hBitmap);
		TransparentBlt(hdc, (PRINT_X/8), (PRINT_Y/2)+150, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

		//악세서리들 출력
		i = 0;
		while (acc[i].acc_num) {
			for (int j = 0; j < 8; j++)
			{
				if (acc[i].acc_num == j + 1) hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc1 + j));		//else if 문 j for문 식으로 줄였습니다
			}

			SelectObject(memdc, hBitmap);
			TransparentBlt(hdc, acc[i].x, acc[i].y, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));
			i++;
		}
		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDM_EXIT:
			PostQuitMessage(0);
			break;
		case ID_S_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_0));
			PRICE_CLOTHES = 25000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_S_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_1));
			PRICE_CLOTHES = 25000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_S_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_S_2));
			PRICE_CLOTHES = 25000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_TS_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_0));
			PRICE_CLOTHES = 10000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_TS_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_1));
			PRICE_CLOTHES = 10000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_TS_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_TS_2));
			PRICE_CLOTHES = 10000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_P_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_P_0));
			PRICE_CLOTHES = 35000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_P_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_P_1));
			PRICE_CLOTHES = 35000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_P_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_P_2));
			PRICE_CLOTHES = 35000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_SK_0:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_SK_0));
			PRICE_CLOTHES = 30000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_SK_1:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_SK_1));
			PRICE_CLOTHES = 30000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;
		case ID_SK_2:
			hBase = LoadBitmap(hInst,
				MAKEINTRESOURCE(IDB_SK_2));
			PRICE_CLOTHES = 30000;
			InvalidateRgn(hwnd, NULL, true);
			InvalidateRgn(Option, NULL, true);
			break;


		case ID_FILESAVE:
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner = hwnd;
			OFN.lpstrFilter = "Bmp File(*.bmp)\0";
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = 256;
			OFN.lpstrDefExt = "bmp";
			OFN.lpstrTitle = "파일 저장";
			g_hBit = ScreenCapture();
			if (GetSaveFileName(&OFN) != 0)
			{
				SaveBitmap(g_hBit, lpstrFile);
			}
			break;
		}

	case WM_COPYDATA:
		acc_num = lParam;			//악세서리 추가
		if (acc_num != 0 && acc_cnt < 19) {
			acc[acc_cnt].acc_num = acc_num;
			acc[acc_cnt].x = rand() % 440 + 200;
			acc[acc_cnt].y = rand() % 300 + 100;
			acc_cnt++;
			COST_ACC += PRICE_ACC[acc_num];			//악세서리가 추가되어서 가격이 오름
		}
		InvalidateRgn(hwnd, NULL, true);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		for (i = 0; i < acc_cnt; i++) {
			if (acc[i].x + 20 <= mx && mx <= acc[i].x + 80 && acc[i].y + 20 <= my && my <= acc[i].y + 80) {
				Selection = true;
				acc_num = i;
				InvalidateRgn(hwnd, NULL, true);
				break;
			}
		}
		break;

	case WM_LBUTTONUP:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);

		if (inTrashcan(mx, my) && Selection) {
			SendMessage(Option, WM_COPYDATA, 0, acc[acc_num].acc_num);
			acc[acc_num].acc_num = acc[acc_cnt - 1].acc_num;
			acc[acc_num].x = acc[acc_cnt - 1].x;
			acc[acc_num].y = acc[acc_cnt - 1].y;
			acc[acc_cnt - 1].acc_num = 0;
			acc_cnt--;
			CanTrash = false;
			InvalidateRgn(hwnd, NULL, true);
		}
		Selection = false;
		break;

	case WM_MOUSEMOVE:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (Selection)
		{
			if (inTrashcan(mx, my))
				CanTrash = true;
			else
				CanTrash = false;

			acc[acc_num].x = mx - 50;
			acc[acc_num].y = my - 50;

			InvalidateRgn(hwnd, NULL, true);
		}
		break;

	case WM_CREATE:

		PlaySound(MAKEINTRESOURCE(IDR_WAVE1),hInst, SND_RESOURCE | SND_ASYNC);
		break;



	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}

void SubViewFrame(HDC hdc);
void SubViewBitmap(HDC hdc, HDC memdc);
void SubViewFont(HDC hdc);

void CHANGECOLOR(COLORREF);

LRESULT CALLBACK ChildWndProc2(HWND hwnd, UINT iMsg,
	WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	int mx, my;
	int ACC_NUM = 0;

	HWND Print = ::FindWindow(NULL, "Print");

	static HBITMAP hBG2;


	switch (iMsg)
	{

	case WM_CREATE:
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		//hBG2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BG2));


		//배경 이미지 출력
		//SelectObject(memdc, hBG2);
		//StretchBlt(hdc, 0, 0, OPTION_X, OPTION_Y, memdc, 0, 0, 501, 800, SRCCOPY);



		SubViewFrame(hdc);          //비트맵 배경 순차 출력입니다.
		SubViewBitmap(hdc, memdc);  //비트맵 순차 출력입니다.
		SubViewFont(hdc);           //폰트 출력입니다.

		DeleteDC(memdc);
		EndPaint(hwnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (35 <= mx && mx <= 135 && 55 <= my && my <= 155)ACC_NUM = 1;
		else if (140 <= mx && mx <= 240 && 55 <= my && my <= 155)ACC_NUM = 2;
		else if (245 <= mx && mx <= 350 && 55 <= my && my <= 155)ACC_NUM = 3;
		else if (355 <= mx && mx <= 460 && 55 <= my && my <= 155)ACC_NUM = 4;
		else if (35 <= mx && mx <= 135 && 160 <= my && my <= 260)ACC_NUM = 5;
		else if (140 <= mx && mx <= 240 && 160 <= my && my <= 260)ACC_NUM = 6;
		else if (245 <= mx && mx <= 350 && 160 <= my && my <= 260)ACC_NUM = 7;
		else if (355 <= mx && mx <= 460 && 160 <= my && my <= 260)ACC_NUM = 8;
		else if (60 <= mx && mx <= 100 && 340 <= my && my <= 380)CHANGECOLOR(RGB(240, 80, 80));
		else if (175 <= mx && mx <= 205 && 340 <= my && my <= 380)CHANGECOLOR(RGB(0, 255, 0));
		else if (280 <= mx && mx <= 310 && 340 <= my && my <= 380)CHANGECOLOR(RGB(127, 255, 212));
		else if (385 <= mx && mx <= 415 && 340 <= my && my <= 380)CHANGECOLOR(RGB(255, 255, 255));
		else if (60 <= mx && mx <= 100 && 395 <= my && my <= 435)CHANGECOLOR(RGB(211, 211, 211));
		else if (175 <= mx && mx <= 205 && 395 <= my && my <= 435)CHANGECOLOR(RGB(255, 215, 0));
		else if (280 <= mx && mx <= 310 && 395 <= my && my <= 435)CHANGECOLOR(RGB(238, 82, 238));
		else if (385 <= mx && mx <= 415 && 395 <= my && my <= 435)CHANGECOLOR(RGB(255, 255, 240));

		SendMessage(Print, WM_COPYDATA, 0, ACC_NUM);
		InvalidateRgn(hwnd, NULL, true);

		break;

	case WM_COPYDATA:
		COST_ACC -= PRICE_ACC[lParam];
		InvalidateRgn(hwnd, NULL, true);

		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}

void SubViewFrame(HDC hdc) {
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	hPen = CreatePen(PS_SOLID, 3, RGB(236, 139, 94));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//악세서리

	Rectangle(hdc, 35, 10, 450, 50);
	Rectangle(hdc, 35, 280, 450, 320);
	Rectangle(hdc, 35, 450, 450, 490);


	hPen = CreatePen(PS_SOLID, 3, RGB(240, 240, 240));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(220, 220, 220));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 4; x++) {
			Ellipse(hdc, 40 + 105 * x, 60 + 105 * y, 130 + 105 * x, 150 + 105 * y);
		}
	}

	//염색
	int i = 0;
	int r = 0;
	int g = 0;
	int b = 0;

	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 4; x++) {
			switch (i)
			{
			case 0:
				r = 240;g = 80;b = 80;
				break;
			case 1:
				r = 0;g = 255;b = 0;
				break;
			case 2:
				r = 127;g = 255;b = 212;
				break;
			case 3:
				r = 255;g = 255;b = 255;
				break;
			case 4:
				r = 211;g = 211;b = 211;
				break;
			case 5:
				r = 255;g = 215;b = 0;
				break;
			case 6:
				r = 238;g = 82;b = 238;
				break;
			case 7:
				r = 255;g = 255;b = 240;
				break;
			}
			
			hPen = CreatePen(PS_SOLID, 3, RGB(204, 204, 204));
			oldPen = (HPEN)SelectObject(hdc, hPen);

			hBrush = CreateSolidBrush(RGB(r, g, b));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

			Ellipse(hdc, 60 + 105 * x, 340 + 55 * y, 100 + 105 * x, 380 + 55 * y);
			i++;

		}
	}

	//계산

	hPen = CreatePen(PS_SOLID, 3, RGB(236, 139, 94));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(255, 255, 255));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 2; x++) {
			Rectangle(hdc, 35 + 210 * x, 495 + 45 * y, 240 + 210* x, 535 + 45 * y);
		}
	}

	Rectangle(hdc, 200, 600, 450, 650);



	SelectObject(hdc, oldPen);
	DeleteObject(hPen);
	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);
}

void SubViewBitmap(HDC hdc, HDC memdc) {
	static HBITMAP hBitmap;


	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc1));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 35, 55, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc2));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 140, 55, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc3));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 245, 55, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc4));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 350, 55, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc5));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 35, 160, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc6));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 140, 160, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc7));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 245, 160, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_acc8));
	SelectObject(memdc, hBitmap);
	TransparentBlt(hdc, 350, 160, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));
}

void SubViewFont(HDC hdc) {
	HFONT font, oldfont;

	AddFontResource("SCDream4.otf");

	font = CreateFont(30, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("에스코어 드림 4 Regular"));
	oldfont = (HFONT)SelectObject(hdc, font);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	TextOut(hdc, 200, 15, _T("악세서리"), _tcslen(_T("악세서리")));

	TextOut(hdc, 220, 285, _T("염색"), _tcslen(_T("염색")));
	/*
	TextOut(hdc, 55, 335, _T("Coral"), _tcslen(_T("Coral")));
	TextOut(hdc, 165, 335, _T("Lime"), _tcslen(_T("Lime")));
	TextOut(hdc, 265, 335, _T("Aqua"), _tcslen(_T("Aqua")));
	TextOut(hdc, 365, 335, _T("White"), _tcslen(_T("White")));
	TextOut(hdc, 60, 390, _T("Grey"), _tcslen(_T("Grey")));
	TextOut(hdc, 165, 390, _T("Gold"), _tcslen(_T("Gold")));
	TextOut(hdc, 265, 390, _T("Violet"), _tcslen(_T("Violet")));
	TextOut(hdc, 370, 390, _T("Ivory"), _tcslen(_T("Ivory")));
	*/

	TextOut(hdc, 220, 455, _T("계산"), _tcslen(_T("계산")));

	SetTextAlign(hdc, TA_RIGHT);
	TextOut(hdc, 205, 500, _T("의상 비용"), _tcslen(_T("의상 비용")));
	TextOut(hdc, 415, 500, _T("악세서리 비용"), _tcslen(_T("악세서리 비용")));
	CString str, str1, str2;

	str.Format("%d", PRICE_CLOTHES);
	str1.Format("%d", COST_ACC);
	str2.Format("%d", PRICE_CLOTHES + COST_ACC);

	TextOut(hdc, 205, 545, str, _tcslen(str));
	TextOut(hdc, 415, 545, str1, _tcslen(str1));
	TextOut(hdc, 415, 610, str2, _tcslen(str2));


	SelectObject(hdc, oldfont);
	DeleteObject(font);
}

HBITMAP ScreenCapture() {
	int width = PRINT_X;
	int height = PRINT_Y;
	// 화면전체 DC를 얻는다
	HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// device context에 넣기
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, 700, 500);

	// 새로운 비트맵 생성
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

	BitBlt(hMemoryDC, -255, -180, 955, 680, hScreenDC, 0, 0, SRCCOPY);		//스크린샷을 할 특정 위치
	hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

	DeleteDC(hMemoryDC);
	DeleteDC(hScreenDC);

	return hBitmap;
}

void SaveBitmap(HBITMAP hbit, char* Path)
{
	BITMAPFILEHEADER bFile;
	BITMAPINFOHEADER bInfo;
	BITMAP bit;
	BITMAPINFO* pih;
	int PalSize;
	HANDLE hFile;
	DWORD dwWritten, Size;
	HDC hdc;

	hdc = GetDC(NULL);

	//비트맵 정보로부터 정보 구조체 초기화
	GetObject(hbit, sizeof(BITMAP), &bit);
	bInfo.biSize = sizeof(BITMAPINFOHEADER);
	bInfo.biWidth = bit.bmWidth;
	bInfo.biHeight = bit.bmHeight;
	bInfo.biPlanes = 1;
	bInfo.biBitCount = bit.bmPlanes * bit.bmBitsPixel;
	if (bInfo.biBitCount > 8) bInfo.biBitCount = 24;
	bInfo.biCompression = BI_RGB;
	bInfo.biSizeImage = 0;
	bInfo.biXPelsPerMeter = 0;
	bInfo.biYPelsPerMeter = 0;
	bInfo.biClrUsed = 0;
	bInfo.biClrImportant = 0;

	//정보 구조체 + 팔레트 크기만큼 메모리 활당 정보 구조체를 복사
	PalSize = (bInfo.biBitCount == 24 ? 0 : 1 << bInfo.biBitCount) * sizeof(RGBQUAD);
	pih = (BITMAPINFO*)malloc(bInfo.biSize + PalSize);
	pih->bmiHeader = bInfo;

	//비트맵의 크기 구함
	GetDIBits(hdc, hbit, 0, bit.bmHeight, NULL, pih, DIB_RGB_COLORS);
	bInfo = pih->bmiHeader;

	//비트맵 크기가 구해지지 않았을 때 수작업으로 직접 계산
	if (bInfo.biSizeImage == 0)
	{
		bInfo.biSizeImage = ((((bInfo.biWidth * bInfo.biBitCount) + 31) & ~31) >> 3) * bInfo.biHeight;
	}

	//래스터 데이터를 읽기 위해 메모리 재할당
	Size = bInfo.biSize + PalSize + bInfo.biSizeImage;
	pih = (BITMAPINFO*)realloc(pih, Size);

	//래스터 데이터 읽기.
	GetDIBits(hdc, hbit, 0, bit.bmHeight, (PBYTE)pih + bInfo.biSize + PalSize, pih, DIB_RGB_COLORS);

	//파일 헤더
	bFile.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + PalSize;
	bFile.bfReserved1 = 0;
	bFile.bfReserved2 = 0;
	bFile.bfSize = Size + sizeof(BITMAPFILEHEADER);
	bFile.bfType = 0x4d42;

	//파일을 생성하고 파일 헤더와 정보 구조체, 팔레트, 래스터 데이터를 출력
	hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &bFile, sizeof(bFile), &dwWritten, NULL);
	WriteFile(hFile, pih, Size, &dwWritten, NULL);

	ReleaseDC(NULL, hdc);
	CloseHandle(hFile);
}

void CHANGECOLOR(COLORREF RGB) {
	int x = 800; 
	int y = 700;
	BYTE red = GetRValue(RGB);
	BYTE blue = GetBValue(RGB);
	BYTE green = GetGValue(RGB);
	BITMAP bmap;
	unsigned char* changed_bmp = new unsigned char[x * y * 4];

	::GetObject(hBase, sizeof(BITMAP), &bmap);

	BYTE* m_pImageData = new BYTE[bmap.bmWidthBytes * bmap.bmHeight * (bmap.bmBitsPixel/8)];
	memset(m_pImageData, 0, bmap.bmWidthBytes * bmap.bmHeight * (bmap.bmBitsPixel/8));

	//비트맵의 픽셀을 저장
	GetBitmapBits(hBase, bmap.bmWidthBytes * bmap.bmHeight, m_pImageData);

	for (int i = 0; i < x* y * 4; i += 4) {
		
		if ((m_pImageData[i],m_pImageData[i + 1] , m_pImageData[i + 2]) == (0,255,42)) {
			changed_bmp[i] = m_pImageData[i];
			changed_bmp[i + 1] = m_pImageData[i + 1];
			changed_bmp[i + 2] = m_pImageData[i + 2];
		}
		else if (m_pImageData[i]<100 && m_pImageData[i + 1]<100&& m_pImageData[i + 2]<100) {
			changed_bmp[i] = m_pImageData[i];
			changed_bmp[i + 1] = m_pImageData[i + 1];
			changed_bmp[i + 2] = m_pImageData[i + 2];
		}
		
		else {

			changed_bmp[i] = blue;
			changed_bmp[i + 1] = green;
			changed_bmp[i + 2] = red;
		}
	}
	SetBitmapBits(hBase, x * y * 4, changed_bmp);
}