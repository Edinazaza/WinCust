#pragma once
#include "FrameCreator/FrameCreator.h"
#include "VideoCreator/VideoCreator.h"

class CustLineController final
{
public:
    CustLineController() = default;
    HRESULT Initialize(HWND hwnd);
    HRESULT OnStart();
    HRESULT OnPause();
    HRESULT OnStop();
    HRESULT OnSave();
private:
    enum WinCustStatus
    {
        PLAY,
        PAUSE,
        STOP,
    };

    HRESULT PushFrame();

    const unsigned int m_fps = 25;

    FrameCreator m_frame_creator;
    VideoCreator m_video_creator;

    WinCustStatus m_status = STOP;
};

