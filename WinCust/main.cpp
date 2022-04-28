#include <windows.h>

#include "CustLine.h"

int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR    lpCmdLine,
                      int       nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    std::shared_ptr<CustLine> win_cust = CustLine::GetInstance();
    win_cust->Initialize(hInstance, GetDesktopWindow());
    win_cust->ShowCustLine();

    MSG msg = {};
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
