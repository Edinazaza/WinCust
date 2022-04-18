#pragma once
#include <windows.h>
#include <string>
#include <array>

class CustLine final
{
public:
    CustLine() = default;
    HRESULT Initialize(HINSTANCE h_instance);
    HRESULT ShowCustLine();
private:
    enum Buttons
    {
        StartButton = 0,
        PauseButton = 1,
        StopButton  = 2,
        SaveButton  = 3,
    };


    static LRESULT CALLBACK CustLineProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
    ATOM RegisterCustLine(HINSTANCE h_instance);
    HRESULT CreateCustLine(HINSTANCE h_instance);
    HRESULT CreateCustLineButtons();

    HWND m_hwnd = NULL;
    std::array<HWND, 4> m_buttons = {};
    const std::wstring m_class_name = L"CustLine";
    const std::wstring m_title_window = L"WinCust";
    const unsigned int m_width = 340u;
    const unsigned int m_height = 80u;

};
