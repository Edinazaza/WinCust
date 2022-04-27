#include "CustBar.h"
#include <CommCtrl.h>

HRESULT CustBar::Initialize(const HWND hwnd_parent, const HINSTANCE h_instance, const int id) {
    InitCommonControls();
    constexpr DWORD style_of_statusbar = WS_CHILD | CCS_NODIVIDER;
    m_hwnd = CreateWindowExW(WS_EX_RIGHT, STATUSCLASSNAME, (PCTSTR)NULL, style_of_statusbar, 0, 0, 0, 0, hwnd_parent, (HMENU)id, h_instance, NULL);

    if (!m_hwnd)
        return E_FAIL;

    return S_OK;
}

HRESULT CustBar::Show() {
    SendMessageW(m_hwnd, WM_SIZE, 0, 0);
    ShowWindow(m_hwnd, SW_SHOW);
    return S_OK;
}

HRESULT CustBar::Hide() {
    ShowWindow(m_hwnd, SW_HIDE);
    return S_OK;
}

HRESULT CustBar::SetText(const std::wstring& text) {
    SendMessageW(m_hwnd, SB_SETTEXT, 0, (LONG)text.data());
    return S_OK;
}

HRESULT CustBar::SetMinHeight(const unsigned int height) {
    SendMessageW(m_hwnd, SB_SETMINHEIGHT, height, 0);
    return S_OK;
}