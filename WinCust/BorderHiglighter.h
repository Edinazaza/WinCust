#pragma once
#include <windows.h>
#include <string>
#include <thread>


class BorderHiglighter final
{
public:
    HRESULT Initialize(HWND hwnd_cust);
    HRESULT StartDrawing();
    HRESULT StopDrawing();
    ~BorderHiglighter();
private:
    HWND m_hwnd_cust;
    void Draw();
    bool is_draw = false;
    const COLORREF m_color = RGB(255, 0, 0);
    std::thread m_draw;
};

