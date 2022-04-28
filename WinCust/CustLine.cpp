#include "CustLineController.h"
#include "CustLine.h"
#include <sstream>
#include "resource.h"

namespace
{
    std::wstring seconds_to_string_hh_mm_ss(unsigned long int time) {
        unsigned int hh = time / 3600;
        time = time % 3600;
        unsigned int mm = time / 60;
        time = time % 60;
        unsigned int ss = time;
        std::wstringstream ws = {};
        ws.width(2);
        ws.fill('0');
        ws << hh << L':';
        ws.width(2);
        ws.fill('0');
        ws << mm << L':';
        ws.width(2);
        ws.fill('0');
        ws << std::right << ss;
        return ws.str();
    }
}

CustLineController CustLine::m_controller = {};
BorderHiglighter CustLine::m_higlighter = {};
CustBar CustLine::m_statusbar = {};

HWND CustLine::m_hwnd = NULL;
std::array<HWND, 3> CustLine::m_buttons = {};
const std::wstring CustLine::m_class_name = L"CustLine";
const std::wstring CustLine::m_title_window = L"WinCust";

const unsigned int CustLine::m_width = 340u;
const unsigned int CustLine::m_height = 80u;
const unsigned int CustLine::m_statusbar_height = 20u;

std::shared_ptr<CustLine> CustLine::m_instance = nullptr;


std::shared_ptr<CustLine> CustLine::GetInstance() {
    if (m_instance == nullptr) {
        m_instance.reset(new CustLine());
    }
    return m_instance;
}

HRESULT CustLine::Initialize(const HINSTANCE h_instance, const HWND hwnd_cust) {
    if (!RegisterCustLine(h_instance))
        return E_FAIL;

    if (FAILED(CreateCustLine(h_instance)))
        return E_FAIL;

    m_controller.Initialize(hwnd_cust);
    m_higlighter.Initialize(hwnd_cust);
    return S_OK;
}

HRESULT CustLine::ShowCustLine() const {
    ShowWindow(m_hwnd, SW_SHOWNORMAL);
    UpdateWindow(m_hwnd);
    return S_OK;
}

const std::wstring CustLine::GetCustLineClassName() {
    return m_class_name;
}

const std::wstring CustLine::GetCustLineTitle() {
    return m_title_window;
}

LRESULT CustLine::CustLineProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    static unsigned long int seconds_of_record = 0ul;
    switch (msg) {
    case WM_TIMER:
        if (w_param != StatusBar)
            break;

        if (m_controller.GetStatus() == CustLineController::WinCustStatus::PLAY) {
            SetTimer(hwnd, StatusBar, 1000, (TIMERPROC)CustLineProc);
            m_statusbar.SetText(seconds_to_string_hh_mm_ss(seconds_of_record));
            ++seconds_of_record;
        } else if (m_controller.GetStatus() == CustLineController::WinCustStatus::STOP)
            seconds_of_record = 0ul;
        break;
    case WM_COMMAND:
        OnPush(static_cast<Controllers>(w_param));
        if (w_param == StartButton) {
            m_higlighter.StartDrawing();
            m_controller.OnStart();
            ShowStatusBar();
            SetTimer(hwnd, StatusBar, 1000, (TIMERPROC)CustLineProc);
        }
        else if (w_param == PauseButton) {
            m_higlighter.StopDrawing();
            m_controller.OnPause();
        }
        else if (w_param == StopButton) {
            m_higlighter.StopDrawing();
            m_controller.OnStop();
            HideStatusbar();
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hwnd, msg, w_param, l_param);
    }
    return 0;
}

ATOM CustLine::RegisterCustLine(const HINSTANCE h_instance) const {
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = CustLineProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = h_instance;
    wcex.hIcon = LoadIcon(h_instance, MAKEINTRESOURCE(IDI_ICON_WIN_CUST));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = m_class_name.data();
    wcex.hIconSm = NULL;

    return RegisterClassExW(&wcex);
}

HRESULT CustLine::CreateCustLine(const HINSTANCE h_instance) {
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

    if (FAILED(m_statusbar.Initialize(m_hwnd, h_instance, StatusBar)))
        return E_FAIL;

    if (FAILED(m_statusbar.SetMinHeight(m_statusbar_height)))
        return E_FAIL;

    return S_OK;
}

HRESULT CustLine::CreateCustLineButtons() {
    constexpr DWORD style_button = WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE | WS_BORDER;
    const int width_button  = m_width / m_buttons.size() - 4; // width_button = |equal part of window space for each button| - |space for button borders|
    const int height_button = m_height / 2;
    const HINSTANCE instance_button = (HINSTANCE)GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE);

    m_buttons[StartButton] = CreateWindow(L"BUTTON", L"Start", style_button, width_button * StartButton, 0, width_button, height_button, m_hwnd, (HMENU)StartButton, instance_button, NULL);
    m_buttons[PauseButton] = CreateWindow(L"BUTTON", L"Pause", style_button, width_button * PauseButton, 0, width_button, height_button, m_hwnd, (HMENU)PauseButton, instance_button, NULL);
    m_buttons[StopButton]  = CreateWindow(L"BUTTON", L"Stop",  style_button, width_button * StopButton,  0, width_button, height_button, m_hwnd, (HMENU)StopButton,  instance_button, NULL);

    for (const HWND& buttons : m_buttons)
        if (!buttons)
            return E_FAIL;

    return S_OK;
}

HRESULT CustLine::OnPush(const Controllers button_push) {
    for (const HWND& button : m_buttons)
        SendMessageW(button, BM_SETCHECK, 0, 0);
    
    if (button_push != StartButton && m_controller.GetStatus() == CustLineController::WinCustStatus::STOP)
        return S_OK;

    return SendMessageW(m_buttons[button_push], BM_SETCHECK, 1, 0);
}

HRESULT CustLine::ShowStatusBar() {
    SetWindowPos(m_hwnd, 0, 0, 0, m_width, m_height + m_statusbar_height, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER);

    if (FAILED(m_statusbar.Show()))
        return E_FAIL;

    return S_OK;
}

HRESULT CustLine::HideStatusbar() {
    SetWindowPos(m_hwnd, 0, 0, 0, m_width, m_height, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER);

    if (FAILED(m_statusbar.Hide()))
        return E_FAIL;

    return S_OK;
}
