#include <windows.h>
void main()
{
    HINSTANCE hInstance;
    STARTUPINFO si;
    int nCmdShow;
    HWND hwnd;
    MSG msg;
    WNDCLASS wc;
    LRESULT WINAPI WinProc(HWND hwnd, UINT tmsg, WPARAM wParam, LPARAM lParam)
    {
        HDC hdc;
        switch(tmsg)
        {
            case WM_LBUTTONDOWN:
            {
                POINT pos;
                char num[6] = "";
                pos.x = LOWORD(lParam);
                pos.y = HIWORD(lParam);
                hdc = GetDC(hwnd);
                TextOut(hdc, pos.x, pos.y,"Pressed left botton  X: ", 24);
                itoa(pos.x/4, num, 6);
                TextOut(hdc, pos.x+142, pos.y,num, 6);
                TextOut(hdc, pos.x+184, pos.y, " Y: ", 5);
                itoa(pos.y/4, num, 6);
                TextOut(hdc, pos.x + 202, pos.y,num, 6);
                ReleaseDC(hwnd, hdc);
                return 0;
            }

            case WM_KEYDOWN: 
            {
                hdc = GetDC(hwnd); 
                char key[1]; 
                key[0] = (char)wParam;
                TextOut(hdc, 10, 10, "key: ", 5); 
                TextOut(hdc, 64, 10, key, 1); 
                return 0; 
            } 

            case WM_SIZE: {
                hdc = GetDC(hwnd);
                POINT pos;
                TCHAR buf[80];
                WORD size;
                pos.x = LOWORD(lParam); 
                pos.y= HIWORD(lParam);
                size = wsprintf(buf, TEXT("size: (x)%d  (y)%d"), pos.x, pos.y);
                TextOut(hdc, 0, 30, buf, size);
                return 0;
            }

            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
        }
        return DefWindowProc(hwnd, tmsg, wParam, lParam);
    }

    GetStartupInfo(&si);
    if(si.dwFlags & STARTF_USESHOWWINDOW)
    {
        nCmdShow = si.wShowWindow;
    }else{
        nCmdShow = SW_SHOWDEFAULT;
    }
    hInstance = GetModuleHandle(NULL);

    memset(&wc, 0, sizeof(wc));
    wc.lpszClassName = "MyClass";
    wc.lpfnWndProc = WinProc;
    wc.hCursor=LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
    wc.hInstance = hInstance;
    if(!RegisterClass(&wc)) return;

    hwnd = CreateWindow("MyClass", "OurWindow", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, NULL);
    if(!hwnd) return;
    ShowWindow(hwnd, nCmdShow);
    while(GetMessage(&msg, 0, 0, 0))
    {
        DispatchMessage(&msg);
    } 
}