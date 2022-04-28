#pragma once
#include <Windows.h>
#include <string>
#include <CommCtrl.h>

class CustBar
{
public:
    HRESULT Initialize(const HWND hwnd_parent, const HINSTANCE h_instance, const int id);
    HRESULT Show();
    HRESULT Hide();
    HRESULT SetText(const std::wstring& text);
    HRESULT SetMinHeight(const unsigned int height);
private:
    HWND m_hwnd = NULL;
};

