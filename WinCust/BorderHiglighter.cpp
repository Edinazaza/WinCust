#include "BorderHiglighter.h"
#include <thread>

HRESULT BorderHiglighter::Initialize(const HWND hwnd_cust) {
    m_hwnd_cust = hwnd_cust;

    if (!m_hwnd_cust)
        return E_FAIL;

    return S_OK;
}

HRESULT BorderHiglighter::StartDrawing() {
    if (!is_draw) {
        is_draw = true;
        m_draw = std::thread(&BorderHiglighter::Draw, this);
    }
    return S_OK;
}

HRESULT BorderHiglighter::StopDrawing() {
    if (is_draw) {
        is_draw = false;
        if (m_draw.joinable())
            m_draw.join();
    }
    return S_OK;
}

BorderHiglighter::~BorderHiglighter() {
    StopDrawing();
}

void BorderHiglighter::Draw() const {
    const HBRUSH brush = CreateSolidBrush(m_color);
    RECT rc = {};
    HDC hdc = {};
    while (is_draw) {
        hdc = GetDC(m_hwnd_cust);
        GetClientRect(m_hwnd_cust, &rc);
        FrameRect(hdc, &rc, brush);
        ReleaseDC(m_hwnd_cust, hdc);
    }
    DeleteObject(brush);
    return;
}
