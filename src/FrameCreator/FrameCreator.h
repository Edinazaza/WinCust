#pragma once
#include <windows.h>
#include <future>

class FrameCreator
{
public:
    FrameCreator();
    ~FrameCreator();
    HRESULT SetHWND(HWND hwnd);
    HBITMAP GetFrame();
    HWND GetHWND() const;
    unsigned int GetWidthFrame() const;
    unsigned int GetHeightFrame() const;
private:
    HBITMAP GetUpdateFrame();
    void ClearFrame();

    HWND m_hwnd = {};
    HBITMAP m_frame = nullptr;
    bool m_update_frame = true;
    std::future<HBITMAP> m_frame_updater = {};
};
