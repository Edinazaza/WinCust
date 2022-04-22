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
    HRESULT Initialize(HWND hwnd);
    HRESULT OnStart();
    HRESULT OnPause();
    HRESULT OnStop();

    const WinCustStatus GetStatus() const;

private:
    HRESULT PushFrame();
    HRESULT FileMove();
    HRESULT SetFilename();

    const unsigned int m_fps = 25;

    FrameCreator m_frame_creator;
    VideoCreator m_video_creator;

    WinCustStatus m_status = STOP;

    std::wstring m_filename = L"";
};

