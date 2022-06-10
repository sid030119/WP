#define _CRT_SECURE_NO_WARNIGNS



#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include <string>
#include <wingdi.h>


#pragma comment(lib,"msimg32.lib")

#pragma comment(lib,"winmm.lib")


LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc2(HWND hDlg, UINT iMsg,
	WPARAM wParam, LPARAM lParam);
HINSTANCE hInst;

HBITMAP ScreenCapture();
void SaveBitmap(HBITMAP hbit, char* Path);
HBITMAP g_hBit;
static HBITMAP hBase;


struct accesory {
	int x = 0;
	int y = 0;
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
	if (!RegisterClass(&WndClass)) return 1;

	WndClass2 = WndClass;
	WndClass2.lpszMenuName = NULL;
	WndClass2.lpszClassName = _T("Option");
	WndClass2.hbrBackground = CreateSolidBrush(RGB(99, 49, 34));
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

BOOL inTrashcan(int mx, int my) {
	return (120 <= mx && mx < 220 && 370 <= my && my <= 470);
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
	char fontaddress[] = "COOKIERUN BLACK.TTF";

	AddFontResource(fontaddress);


	switch (iMsg)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);
		hBG = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BG));

		//배경 이미지 출력
		SelectObject(memdc, hBG);
		StretchBlt(hdc, 0, 0, 840, 543, memdc, 0, 0, 605, 432, SRCCOPY);

		//펜 설정 및 펜사용 부분
		HPEN hPen, oldPen;
		HBRUSH hBrush, oldBrush;

		hPen = CreatePen(PS_SOLID, 3, RGB(125, 88, 84));
		oldPen = (HPEN)SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(249, 235, 222));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, 10, 10, 100, 50);

		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);


		//폰트설정 및 글씨 사용부분
		font = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("COOKIERUN BLACK"));
		oldfont = (HFONT)SelectObject(hdc, font);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(125, 88, 84));

		TextOut(hdc, 17, 15, _T("옷 출력"), _tcslen(_T("옷 출력")));

		SelectObject(hdc, oldfont);
		DeleteObject(font);
		RemoveFontResource(fontaddress);


		//기본 옷 출력

		SelectObject(memdc, hBase);

		TransparentBlt(hdc, 250, 100, 350, 350, memdc, 0, 0, 800, 700, RGB(42, 255, 0));

		//쓰레기통 출력
		if (CanTrash)
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TRASH_ON));
		else
			hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TRASH_OFF));

		SelectObject(memdc, hBitmap);
		TransparentBlt(hdc, 120, 370, 100, 100, memdc, 0, 0, 500, 500, RGB(42, 255, 0));

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
		acc_num = lParam;
		if (acc_num != 0 && acc_cnt < 19) {
			acc[acc_cnt].acc_num = acc_num;
			acc[acc_cnt].x = rand() % 440 + 200;
			acc[acc_cnt].y = rand() % 300 + 100;
			acc_cnt++;
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

	switch (iMsg)
	{

	case WM_CREATE:
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		memdc = CreateCompatibleDC(hdc);

		SubViewFrame(hdc);          //비트맵 배경 순차 출력입니다.
		SubViewBitmap(hdc, memdc);  //비트맵 순차 출력입니다.
		SubViewFont(hdc);           //폰트 출력입니다.

		DeleteDC(memdc);
		EndPaint(hwnd, &ps);


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
		else if (35 <= mx && mx <= 135 && 320 <= my && my <= 370)CHANGECOLOR(RGB(255, 0, 0));
		else if (140 <= mx && mx <= 240 && 320 <= my && my <= 370)CHANGECOLOR(RGB(0, 255, 0));
		else if (245 <= mx && mx <= 350 && 320 <= my && my <= 370)CHANGECOLOR(RGB(0, 0, 255));
		else if (355 <= mx && mx <= 460 && 320 <= my && my <= 370)CHANGECOLOR(RGB(255, 255, 255));

		SendMessage(Print, WM_COPYDATA, 0, ACC_NUM);

		break;
	case WM_DESTROY:
		return 0;
	}
	return DefMDIChildProc(hwnd, iMsg, wParam, lParam);
}

void SubViewFrame(HDC hdc) {
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;

	hPen = CreatePen(PS_SOLID, 3, RGB(125, 88, 84));
	oldPen = (HPEN)SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(249, 235, 222));
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	//악세서리
	Rectangle(hdc, 35, 10, 450, 50);

	for (int y = 0; y < 2; y++) {
		for (int x = 0; x < 4; x++) {
			Rectangle(hdc, 35 + 105 * x, 55 + 105 * y, 135 + 105 * x, 155 + 105 * y);
		}
	}

	//염색
	Rectangle(hdc, 35, 270, 450, 310);

	for (int y = 0; y < 1; y++) {
		for (int x = 0; x < 4; x++) {
			Rectangle(hdc, 35 + 105 * x, 315 + 105 * y, 135 + 105 * x, 365 + 105 * y);
		}
	}



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
	char fontaddress[] = "COOKIERUN BLACK.TTF";

	AddFontResource(fontaddress);
	SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);

	font = CreateFont(30, 0, 0, 0, FW_BOLD, 0, 0, 0, HANGUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, TEXT("COOKIERUN BLACK"));
	oldfont = (HFONT)SelectObject(hdc, font);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(125, 88, 84));
	TextOut(hdc, 200, 15, _T("악세서리"), _tcslen(_T("악세서리")));

	TextOut(hdc, 200, 275, _T("염색"), _tcslen(_T("염색")));
	TextOut(hdc, 65, 325, _T("Red"), _tcslen(_T("Red")));
	TextOut(hdc, 155, 325, _T("Green"), _tcslen(_T("Green")));
	TextOut(hdc, 270, 325, _T("Blue"), _tcslen(_T("Blue")));
	TextOut(hdc, 365, 325, _T("White"), _tcslen(_T("White")));

	SelectObject(hdc, oldfont);
	DeleteObject(font);
	RemoveFontResource(fontaddress);
}

HBITMAP ScreenCapture() {
	int width = 500;
	int height = 430;
	// 화면전체 DC를 얻는다
	HDC hScreenDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	// device context에 넣기
	HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

	HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

	// 새로운 비트맵 생성
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

	BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 285, 190, SRCCOPY);		//스크린샷을 할 특정 위치
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