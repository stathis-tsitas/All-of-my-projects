#include <windows.h>
#include <time.h>


HWND hButton; 
COLORREF g_bgColor = RGB(240, 240, 240);
int btnWidth = 180;
int btnHeight = 50;

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            
            srand(time(NULL));
            
            
            hButton = CreateWindow("BUTTON", "Press for a Surprise", 
                         WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
                         0, 0, btnWidth, btnHeight, 
                         hwnd, (HMENU)1, NULL, NULL);
            break;
        }

        case WM_SIZE: {
            
            int windowWidth = LOWORD(lp);
            int windowHeight = HIWORD(lp);

            
            int centerX = (windowWidth - btnWidth) / 2;
            int centerY = (windowHeight - btnHeight) / 2;

            
            MoveWindow(hButton, centerX, centerY, btnWidth, btnHeight, TRUE);
            break;
        }

        case WM_COMMAND: {
            if (LOWORD(wp) == 1) {
                
                g_bgColor = RGB(rand() % 256, rand() % 256, rand() % 256);
                InvalidateRect(hwnd, NULL, TRUE); 
            }
            break;
        }

        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wp;
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH brush = CreateSolidBrush(g_bgColor);
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);
            return 1;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

    WNDCLASS wc = {0};
    wc.lpszClassName = "MyWindowClass";
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = WindowProcessMessages;

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "Surprise App", 
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
                             100, 100, 400, 250, 
                             NULL, NULL, hInst, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}