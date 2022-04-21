#pragma once
#include <windows.h>
#include <string>
#include <array>

#include "CustLineController.h"

class CustLine final
{
public:
    CustLine() = default;
    HRESULT Initialize(HINSTANCE h_instance, HWND hwnd_cust);
    HRESULT ShowCustLine();
    static const std::wstring GetCustLineClassName();
    static const std::wstring GetCustLineTitle();
private:
    enum Buttons
    {
        StartButton = 0,
        PauseButton = 1,
        StopButton  = 2,
    };

    static LRESULT CALLBACK CustLineProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
    ATOM RegisterCustLine(HINSTANCE h_instance);
    HRESULT CreateCustLine(HINSTANCE h_instance);
    HRESULT CreateCustLineButtons();

    HWND m_hwnd = NULL;
    std::array<HWND, 3> m_buttons = {};
    static const std::wstring m_class_name;
    static const std::wstring m_title_window;
    const unsigned int m_width = 340u;
    const unsigned int m_height = 80u;

    static CustLineController m_controller;
};
