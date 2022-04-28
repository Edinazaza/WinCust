#include <windows.h>

#include "CustLine.h"

int APIENTRY wWinMain(HINSTANCE h_instance,
                      HINSTANCE h_prev_instance,
                      LPWSTR    lp_cmd_line,
                      int       n_cmd_show) {
    UNREFERENCED_PARAMETER(h_prev_instance);
    UNREFERENCED_PARAMETER(lp_cmd_line);
    UNREFERENCED_PARAMETER(n_cmd_show);

    std::shared_ptr<CustLine> win_cust = CustLine::GetInstance();
    win_cust->Initialize(h_instance, GetDesktopWindow());
    win_cust->ShowCustLine();

    MSG msg = {};
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
