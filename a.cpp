#include<windows.h>
#include<iostream>
using namespace std;

#define ARGB(A) (((A)>>16&0xFF) | (A)&0xFF00FF00 | ((A)&0xFF)<<16)
int memoryBitmapW;
#define MoveToEx(DEST, T0, X, T1, T2) MoveToEx(T0, ((DEST)*memoryBitmapW) + (X), T1, T2)
#define LineTo(DEST, T0, X, T1) LineTo(T0, ((DEST)*memoryBitmapW) + (X), T1)
#define Rectangle(DEST, A, B, C, D, E) Rectangle(A, ((DEST)*memoryBitmapW) + (B), C, ((DEST)*memoryBitmapW) + (D), E)
#define Ellipse(DEST, A, B, C, D, E) Ellipse(A, ((DEST)*memoryBitmapW) + (B), C, ((DEST)*memoryBitmapW) + (D), E)

typedef struct LTWH{SHORT l,t, w,h;}LTWH;//Left Top Width Height
typedef union RECT$LTWH{
	LTWH ltwh;
	RECT rect;
}RECT$LTWH;
RECT$LTWH wndPos;
#define wndSzW (wndPos.ltwh.w)
#define wndSzH (wndPos.ltwh.h)

POINT mousePointer;
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	static struct{
		HDC hdc, hmdc;
		HBITMAP hmbmp;

		HPEN hOldPen;
		HBRUSH hOldBrush;
	} mem;
	static struct S0{
		COORD size, lt, num; //left-top, number of columns and rows
	}grid;
	static COORD pieceSize;
	static COORD pos;
	static PAINTSTRUCT ps;
	switch(Message) {
		case WM_PAINT:{
			BeginPaint(hwnd, &ps);
			BitBlt(ps.hdc, 0,0, wndSzW,wndSzH, mem.hmdc, 0,0, SRCCOPY);
			DefWindowProc(hwnd, Message, wParam, lParam);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_MOUSEMOVE:{
//			cout<<"Mouse : "<<LOWORD(lParam)<<", "<<HIWORD(lParam)<<endl;
			static COORD b={-1,-1};
			pos={	(LOWORD(lParam)-grid.lt.X+(((grid.size.X)/(grid.num.X-1))>>1))*(grid.num.X-1)/(grid.size.X-1),
					(HIWORD(lParam)-grid.lt.Y+(((grid.size.Y)/(grid.num.Y-1))>>1))*(grid.num.Y-1)/(grid.size.Y-1)};
			if(b.X!=pos.X || b.Y!=pos.Y){
				if(0<=pos.X && pos.X <grid.num.X && 0<=pos.Y && pos.Y <grid.num.Y){
					b=pos;
					SetDCBrushColor(mem.hmdc,0), SetDCPenColor(mem.hmdc, 0);
					Rectangle(3, mem.hmdc, 0,0, wndSzW, wndSzH);
//					Rectangle(0, mem.hmdc, 0,0, wndSzW, wndSzH);
					BitBlt(mem.hmdc, memoryBitmapW*3 + pieceSize.X, pieceSize.Y, grid.size.X,grid.size.Y, mem.hmdc, memoryBitmapW * 1,0, SRCCOPY);
					BitBlt(mem.hmdc, memoryBitmapW*3 + pos.X*(grid.size.X-1)/(grid.num.X-1)-(pieceSize.X>>1) + pieceSize.X, pos.Y*(grid.size.Y-1)/(grid.num.Y-1)-(pieceSize.Y>>1) + pieceSize.Y, pieceSize.X,pieceSize.Y, mem.hmdc, memoryBitmapW * 2,0, SRCCOPY);
					BitBlt(mem.hmdc, memoryBitmapW*0 + grid.lt.X - pieceSize.X, grid.lt.Y - pieceSize.Y, grid.size.X + (pieceSize.X<<1),grid.size.Y + (pieceSize.Y<<1), mem.hmdc, memoryBitmapW * 3,0, SRCCOPY);
					RECT rect={0,0,wndSzW, wndSzH};
					InvalidateRect(hwnd, &rect, false);
				}
				else pos={-1, -1};
			}
			break;
		}
		case WM_LBUTTONDOWN:{
			if(pos.X!=-1){
			}
			cout<<"click : "<<pos.X<<" "<<pos.Y<<endl;
			break;
		}
		case WM_CREATE:{
			//Get Window Size
			GetWindowRect(hwnd,&wndPos.rect);
			wndPos={wndPos.rect.left,
			wndPos.rect.top,
			wndPos.rect.right-wndPos.rect.left,
			wndPos.rect.bottom-wndPos.rect.top};
			cout<<wndPos.ltwh.l<<'\t'<<wndPos.ltwh.t<<'\t'<<wndPos.ltwh.w<<'\t'<<wndPos.ltwh.h<<endl;

			//Setup DrawContext	
			mem.hdc=GetDC(hwnd);
			mem.hmdc=CreateCompatibleDC(mem.hdc);
			mem.hmbmp=CreateCompatibleBitmap(mem.hdc,wndPos.ltwh.w*16,wndPos.ltwh.h);	//Compatible Bitmap
			memoryBitmapW=wndPos.ltwh.w;
			SelectObject(mem.hmdc,mem.hmbmp);

			mem.hOldPen=(HPEN)		SelectObject(mem.hmdc, GetStockObject(DC_PEN));
			mem.hOldBrush=(HBRUSH)	SelectObject(mem.hmdc, GetStockObject(DC_BRUSH));

			//Setup Grid
			grid.num={15,15};
			grid.size={wndSzH*8/10, wndSzH*8/10};
			grid.lt={wndSzH/10,wndSzH/10};

			//Draw Grid
			SetDCBrushColor(mem.hmdc, ARGB(0xffadff));
			SetDCPenColor(mem.hmdc, ARGB(0xffffff));
//			Rectangle(0, mem.hmdc, 200, 200, 250, 250);
			for(int i=grid.num.X-1; i+1;i--){
				MoveToEx(1, mem.hmdc, (grid.size.X-1) * i / (grid.num.X-1), 0, nullptr);
				LineTo(1, mem.hmdc, (grid.size.X-1) * i / (grid.num.X-1), grid.size.Y);
			}
			for(int i=grid.num.Y-1; i+1;i--){
				MoveToEx(1, mem.hmdc, 0, (grid.size.Y-1) * i / (grid.num.Y-1), nullptr);
				LineTo(1, mem.hmdc, grid.size.X, (grid.size.Y-1) * i / (grid.num.Y-1));
			}

			//Draw Pieces
			pieceSize={50,50};
			SetDCPenColor(mem.hmdc, ARGB(0xffffff));
			SetDCBrushColor(mem.hmdc, ARGB(0x000000));
			Ellipse(2,mem.hmdc, 0, 0, pieceSize.X, pieceSize.Y);
			SetDCBrushColor(mem.hmdc, ARGB(0xffffff));
			Ellipse(2,mem.hmdc, pieceSize.X, 0, pieceSize.X<<1, pieceSize.Y);

			BitBlt(mem.hmdc, memoryBitmapW*0 + grid.lt.X, grid.lt.Y, grid.size.X,grid.size.Y, mem.hmdc, memoryBitmapW * 1,0, SRCCOPY);
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
//	FreeConsole();
	cout<<"S : "<<GetSystemMetrics(SM_CXSCREEN)<<", "<<GetSystemMetrics(SM_CYSCREEN)<<endl;
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
		GetSystemMetrics(SM_CXSCREEN)*3/4,
		GetSystemMetrics(SM_CYSCREEN)*3/4,
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL){
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	while(GetMessage(&msg, NULL, 0, 0) > 0){
		mousePointer=msg.pt;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
