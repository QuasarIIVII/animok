#include<windows.h>
#include<iostream>
using namespace std;

#define ARGB(A) (((A)>>16&0xFF) | (A)&0xFF00FF00 | ((A)&0xFF)<<16)

typedef struct LTWH{SHORT l,t, w,h;}LTWH;//Left Top Width Height
typedef union RECT$LTWH{
	LTWH ltwh;
	RECT rect;
}RECT$LTWH;
RECT$LTWH wndPos;
#define wndSzW (wndPos.ltwh.w)
#define wndSzE (wndPos.ltwh.h)
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	static struct{
		HDC hdc, hmdc;
		HBITMAP hmbmp;

		HPEN hOldPen;
		HBRUSH hOldBrush;
	} mem;
	static PAINTSTRUCT ps;
LRESULT OnPaint(HWND hwnd) { //NEW
				PAINTSTRUCT ps; HDC hdc = BeginPaint(hwnd, &ps); 
				for (int i = 1; i <= 800 / 50; ++i) {
					MoveToEx(mem.hmdc, i * 50 -1 , 0, nullptr); 
					LineTo(mem.hmdc, i * 50 -1 , 500); 
					} 
				for (int i = 1; i <= 500 / 50; ++i) { 
					MoveToEx(mem.hmdc, 0, i * 50, nullptr); 
					LineTo(mem.hmdc, 800, i * 50); 
					} 
					return EndPaint(hwnd, &ps); 
				}
	switch(Message) {
		case WM_PAINT:{
			BeginPaint(hwnd, &ps);
			BitBlt(ps.hdc, 0,0, wndSzW,wndSzE, mem.hmdc, 0,0, SRCCOPY);
			DefWindowProc(hwnd, Message, wParam, lParam);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_CREATE:{
			//Get Window Size
			GetWindowRect(hwnd,&wndPos.rect);
			wndPos={wndPos.rect.left,
			wndPos.rect.top,
			wndPos.rect.bottom-wndPos.rect.top,
			wndPos.rect.right-wndPos.rect.left};
			cout<<wndPos.ltwh.l<<'\t'<<wndPos.ltwh.t<<'\t'<<wndPos.ltwh.w<<'\t'<<wndPos.ltwh.h<<endl;
			//DrawContext	
			mem.hdc=GetDC(hwnd);
			mem.hmdc=CreateCompatibleDC(mem.hdc);
			mem.hmbmp=CreateCompatibleBitmap(mem.hdc,wndPos.ltwh.w,wndPos.ltwh.h);	//Compatible Bitmap
			SelectObject(mem.hmdc,mem.hmbmp);

			mem.hOldPen=(HPEN)		SelectObject(mem.hmdc, GetStockObject(DC_PEN));
			mem.hOldBrush=(HBRUSH)	SelectObject(mem.hmdc, GetStockObject(DC_BRUSH));
			
			SetDCBrushColor(mem.hmdc, ARGB(0xffffff));
			//
			LRESULT OnPaint(HWND hwnd){
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hwnd, &ps);
				for (int i = 1; i <= 800 / 50; ++i){
					MoveToEx(mem.hmdc, i * 50 -1 , 0, nullptr);
					LineTo(mem.hmdc, i * 50 -1 , 500);
					}
				for (int i = 1; i <= 500 / 50; ++i){
					MoveToEx(mem.hmdc, 0, i * 50, nullptr);
					LineTo(mem.hmdc, 800, i * 50);
					}
				return EndPaint(hwnd, &ps);
			} 
			//
			break;
		}
		case WM_DESTROY: {
			SelectObject(mem.hmdc, mem.hOldPen);
			SelectObject(mem.hmdc, mem.hOldBrush);
			
			DeleteObject(mem.hmbmp);
			DeleteDC(mem.hmdc);
			ReleaseDC(hwnd,mem.hdc);

			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+3);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","animok",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		0,
		0,
		900,
		900,
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL){
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
