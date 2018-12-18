#include <windows.h>
#include <stdio.h>

const char g_szClassName[] = "myWindowClass";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{HDC hdc;
static int sx, sy;
    switch(msg)
    {
		case WM_LBUTTONDOWN:{
		POINT cor;
	  WORD nSize;
      TCHAR szBuf[80];
   
 
      cor.x = LOWORD(lParam);
      cor.y = HIWORD(lParam);
 
      hdc = GetDC(hwnd);
      nSize = wsprintf(szBuf, TEXT("%d, %d"), cor.x, cor.y);
      TextOut(hdc, cor.x, cor.y, szBuf, nSize);
      
      ReleaseDC(hwnd, hdc);
	  
	  break;
		}
		case WM_CHAR: {
			// if ((int)wParam == 32){
				// TextOut(hdc, 0, 0, "key: d", 6);
			// }
			hdc = GetDC(hwnd);
			TCHAR szBuf[80];
			WORD nSize;
			
			nSize = wsprintf(szBuf, TEXT("key: %c \n"), (char)wParam);
			TextOut(hdc, 0, 0, szBuf, nSize);
			return 0;
		 }
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
		
		case WM_SIZE: {
			hdc = GetDC(hwnd);
			POINT cor;
		    TCHAR szBuf[80];
			WORD nSize;
            cor.x = LOWORD(lParam); 
            cor.y = HIWORD(lParam);
			nSize = wsprintf(szBuf, TEXT("size: %d %d"), (int)cor.x, (int)cor.y);
			TextOut(hdc, 0, 30, szBuf, nSize);
			
        return 0;
		}
		
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        // MessageBox(NULL, "Window Registration Failed!", "Error!",
            // MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Lab5",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        // MessageBox(NULL, "Window Creation Failed!", "Error!",
            // MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}