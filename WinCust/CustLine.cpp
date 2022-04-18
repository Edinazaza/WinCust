#include "CustLine.h"

HRESULT CustLine::Initialize(HINSTANCE h_instance) {
    if (!RegisterCustLine(h_instance))
        return E_FAIL;

    if (FAILED(CreateCustLine(h_instance)))
        return E_FAIL;

    return S_OK;
}

HRESULT CustLine::ShowCustLine() {
    ShowWindow(m_hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_hwnd);
    return S_OK;
}

LRESULT CustLine::CustLineProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, msg, w_param, l_param);
    }
    return 0;
}

ATOM CustLine::RegisterCustLine(HINSTANCE h_instance) {
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = CustLineProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = h_instance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_class_name.data();
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

HRESULT CustLine::CreateCustLine(HINSTANCE h_instance) {
    m_hwnd = CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST,
                             m_class_name.data(), m_title_window.data(),
                             WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER,
                             CW_USEDEFAULT, 0,
                             m_width, m_height,
                             NULL,
                             NULL,
                             h_instance,
                             NULL);

    if (!m_hwnd)
        return E_FAIL;

    if (FAILED(CreateCustLineButtons()))
        return E_FAIL;

    return S_OK;
}

HRESULT CustLine::CreateCustLineButtons() {
    constexpr DWORD style_button = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | WS_BORDER;
    const int width_button  = m_width / m_buttons.size() - m_buttons.size(); // width_button = |equal part of window space for each button| - |space for button borders|
    const int height_button = m_height / 2;
    const HINSTANCE instance_button = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);

    m_buttons[StartButton] = CreateWindow(L"BUTTON", L"Start", style_button, width_button * StartButton, 0, width_button, height_button, m_hwnd, (HMENU)StartButton, instance_button, NULL);
    m_buttons[PauseButton] = CreateWindow(L"BUTTON", L"Pause", style_button, width_button * PauseButton, 0, width_button, height_button, m_hwnd, (HMENU)PauseButton, instance_button, NULL);
    m_buttons[StopButton]  = CreateWindow(L"BUTTON", L"Stop",  style_button, width_button * StopButton,  0, width_button, height_button, m_hwnd, (HMENU)StopButton, instance_button, NULL);
    m_buttons[SaveButton]  = CreateWindow(L"BUTTON", L"Save",  style_button, width_button * SaveButton,  0, width_button, height_button, m_hwnd, (HMENU)SaveButton, instance_button, NULL);

    for (const HWND& buttons : m_buttons)
        if (!buttons)
            return E_FAIL;

    return S_OK;
}
