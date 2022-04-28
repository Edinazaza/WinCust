#pragma once
#include "FrameCreator/FrameCreator.h"
#include "VideoCreator/VideoCreator.h"

class CustLineController final
{
public:
    enum WinCustStatus
    {
        PLAY,
        PAUSE,
        STOP,
    };

    CustLineController() = default;
    HRESULT Initialize(HWND hwnd_cust);
    HRESULT OnStart();
    HRESULT OnPause();
    HRESULT OnStop();
    ~CustLineController();

    const WinCustStatus GetStatus() const;
    HWND GetCustHWND() const;
private:
    HRESULT PushFrame();
    HRESULT FileMove();
    HRESULT SetFilename();

    static const unsigned int m_fps = 25;

    FrameCreator m_frame_creator = {};
    VideoCreator m_video_creator = {};

    WinCustStatus m_status = STOP;

    std::thread m_push_frame_process = {};

    std::wstring m_filename = L"";

    static const std::wstring m_dir_video;
};

