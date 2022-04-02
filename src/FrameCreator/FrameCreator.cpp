#include "FrameCreator.h"

FrameCreator::FrameCreator() {
    m_hwnd = GetDesktopWindow();
}

FrameCreator::~FrameCreator() {
    ClearFrame();
}

HRESULT FrameCreator::SetHWND(HWND hwnd) {
    if (hwnd == nullptr) {
        return E_FAIL;
    }

    ClearFrame();
    m_hwnd = hwnd;

    return S_OK;
}

HBITMAP FrameCreator::GetFrame() {
    if (m_hwnd == nullptr)
        return nullptr;

    if (m_update_frame) {
        m_frame_updater = std::async(std::launch::async, &FrameCreator::GetUpdateFrame, this);
    } else {
        DeleteObject(m_frame);
        m_frame = m_frame_updater.get();
        m_update_frame = true;
    }

    return m_frame;
}

HBITMAP FrameCreator::GetUpdateFrame() {
    HDC hdc = GetDC(m_hwnd);
    if (hdc == nullptr) {
        return nullptr;
    }

    HDC hdc_compatible = CreateCompatibleDC(hdc);

    RECT client_rect = {};
    GetClientRect(m_hwnd, &client_rect);
    const int width = client_rect.right - client_rect.left;
    const int height = client_rect.bottom - client_rect.top;

    HBITMAP bitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP old_bitmap = (HBITMAP)SelectObject(hdc_compatible, bitmap);

    BitBlt(hdc_compatible, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
    ReleaseDC(m_hwnd, hdc);
    SelectObject(hdc_compatible, old_bitmap);
    DeleteDC(hdc_compatible);
    DeleteObject(old_bitmap);
    m_update_frame = false;

    return bitmap;
}

void FrameCreator::ClearFrame() {
    if (m_frame_updater.valid()) {
        m_frame_updater.wait();
        DeleteObject(m_frame_updater.get());
    }

    DeleteObject(m_frame);
    m_update_frame = true;
}
