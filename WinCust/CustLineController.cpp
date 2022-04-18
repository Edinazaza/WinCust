#include "CustLineController.h"

HRESULT CustLineController::Initialize(HWND hwnd) {
    if(FAILED(m_frame_creator.SetHWND(hwnd)))
       return E_FAIL;

    const std::wstring random_filename = std::to_wstring(std::time(NULL) + rand());
    if(FAILED(m_video_creator.Initialize(m_fps, m_frame_creator.GetWidthFrame(), m_frame_creator.GetHeightFrame(), random_filename)))
        return E_FAIL;
    return S_OK;
}

HRESULT CustLineController::OnStart() {
    if (m_status == STOP) {
        m_video_creator.StartWrite();
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000/m_fps));
            PushFrame();
        }).detach();
    } else if (m_status == PAUSE)
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
            PushFrame();
        }).detach();

    m_status = PLAY;

    return S_OK;
}

HRESULT CustLineController::OnPause() {
    if(m_status == PLAY)
        m_status = PAUSE;

    return S_OK;
}

HRESULT CustLineController::OnStop() {
    if (m_status == PLAY || m_status == PAUSE) {
        m_status = STOP;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
        m_video_creator.StopWrite();
    }

    return S_OK;
}

HRESULT CustLineController::OnSave() {
    return E_NOTIMPL;
}

HRESULT CustLineController::PushFrame() {
    std::vector<BYTE> frame(m_frame_creator.GetWidthFrame() * m_frame_creator.GetWidthFrame() * 4);
    GetBitmapBits(m_frame_creator.GetFrame(), 1920 * 1080 * 4, frame.data());
    m_video_creator.AddFrame(frame, true);

    if(m_status == PLAY)
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
            PushFrame();
        }).detach();

    return S_OK;
}
