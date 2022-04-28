#pragma once
#include <Windows.h>
#include <CommCtrl.h>

#include <string>

class CustBar
{
public:
    HRESULT Initialize(const HWND hwnd_parent, const HINSTANCE h_instance, const int id);
    HRESULT Show() const;
    HRESULT Hide() const;
    HRESULT SetText(const std::wstring& text);
    HRESULT SetMinHeight(const unsigned int height);
private:
    HWND m_hwnd = NULL;
};

