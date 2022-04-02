#include <windows.h>
#include <tchar.h>
#include <gdiplus.h>
#include <FrameCreator/FrameCreator.h>

constexpr int FPS = 33;
FrameCreator vr;

void OnPaint(HWND hWnd, HBITMAP hBMP) {
    PAINTSTRUCT ps;
    HDC hDC = BeginPaint(hWnd, &ps);
    Gdiplus::Graphics graphics(hDC);
    Gdiplus::Bitmap bitmap(hBMP, NULL);
    RECT rc;
    GetClientRect(hWnd, &rc);
    graphics.DrawImage(&bitmap, (INT)rc.left, (INT)rc.top, (INT)(rc.right - rc.left), (INT)(rc.bottom - rc.top));
    EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam) {
    HBITMAP bitmap;
    switch (message) {
    case WM_TIMER:
        SetTimer(hWnd, 1, FPS, (TIMERPROC)NULL);
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        bitmap = vr.GetFrame();
        OnPaint(hWnd, bitmap);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASS wc;
    wc.hInstance = hInstance;
    wc.lpszClassName = _T("MainWindow");
    wc.lpfnWndProc = WndProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = 0;
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(_T("MainWindow"), _T("MainWindow"), WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        HWND_DESKTOP, NULL, hInstance, NULL);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    SetTimer(hwnd,
            1,
            FPS,
            (TIMERPROC)NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}