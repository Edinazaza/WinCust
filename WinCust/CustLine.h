#pragma once
#include <Windows.h>

#include <sstream>
#include <string>
#include <array>
#include <memory>

#include "CustLineController.h"
#include "BorderHiglighter.h"
#include "CustBar.h"
#include "resource.h"

class CustLine final
{
public:
    CustLine(CustLine& other) = delete;
    void operator=(const CustLine&) = delete;

    static std::shared_ptr<CustLine> GetInstance();

    HRESULT Initialize(const HINSTANCE h_instance, const HWND hwnd_cust);
    HRESULT ShowCustLine() const;
    static const std::wstring GetCustLineClassName();
    static const std::wstring GetCustLineTitle();
private:
    CustLine() = default;

    enum Controllers
    {
        StartButton = 0,
        PauseButton = 1,
        StopButton  = 2,
        StatusBar   = 4,
    };

    static LRESULT CALLBACK CustLineProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);
    ATOM RegisterCustLine(const HINSTANCE h_instance) const;
    HRESULT CreateCustLine(const HINSTANCE h_instance);
    HRESULT CreateCustLineButtons();

    static HRESULT OnPush(const Controllers button_push);
    static HRESULT ShowStatusBar();
    static HRESULT HideStatusbar();

    static HWND m_hwnd;
    static std::array<HWND, 3> m_buttons;
    static const std::wstring m_class_name;
    static const std::wstring m_title_window;
    static const unsigned int m_width;
    static const unsigned int m_height;
    static const unsigned int m_statusbar_height;

    static CustLineController m_controller;
    static BorderHiglighter m_higlighter;
    static CustBar m_statusbar;
    static std::shared_ptr<CustLine> m_instance;
};
