#include<windows.h>

typedef struct LTWH{SHORT l,t, w,h;}LTWH;//Left Top Width Height
typedef union RECT$LTWH{
	LTWH ltwh;
	RECT rect;
}RECT$LTWH;
RECT$LTWH wndPos;
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam){
	static struct{
		HDC hdc, hmdc;
		HBITMAP hmbmp;
	} mem;
	switch(Message) {
		case WM_PAINT:{

			break;
		}
		case WM_CREATE:{
			//Get Window Size
			GetWindowRect(hwnd,&wndPos.rect);
			wndPos={wndPos.rect.left,
			wndPos.rect.top,
			wndPos.rect.bottom-wndPos.rect.top,
			wndPos.rect.right-wndPos.rect.left};

			mem.hdc=GetDC(hwnd);
			mem.hmdc=CreateCompatibleDC(mem.hdc);
			mem.hmbmp=CreateCompatibleBitmap(mem.hdc,wndPos.ltwh.w,wndPos.ltwh.h);	//Compatible Bitmap
			SelectObject(mem.hmdc,mem.hmbmp);

			break;
		}
		case WM_DESTROY: {
			
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
	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)){
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Yee",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		0,
		0,
		1280,
		720,
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
