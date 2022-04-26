#include "CustLineController.h"
#include "CustLine.h"

HRESULT CustLineController::Initialize(HWND hwnd_cust) {
    if(FAILED(m_frame_creator.SetHWND(hwnd_cust)))
       return E_FAIL;

    return S_OK;
}

HRESULT CustLineController::OnStart() {
    if (m_status == STOP) {

        if (FAILED(SetFilename()))
            return E_FAIL;

        if (FAILED(m_video_creator.Initialize(m_fps, m_frame_creator.GetWidthFrame(), m_frame_creator.GetHeightFrame(), m_filename)))
            return E_FAIL;

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
    if (m_status != STOP) {
        m_status = STOP;
        m_video_creator.StopWrite();
        FileMove();
    }

    return S_OK;
}

const CustLineController::WinCustStatus CustLineController::GetStatus() const {
    return m_status;
}

HRESULT CustLineController::PushFrame() {
    std::vector<BYTE> frame(m_frame_creator.GetWidthFrame() * m_frame_creator.GetWidthFrame() * 4);
    GetBitmapBits(m_frame_creator.GetFrame(), frame.size(), frame.data());
    m_video_creator.AddFrame(frame, true);

    if(m_status == PLAY)
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / m_fps));
            PushFrame();
        }).detach();

    return S_OK;
}

HRESULT CustLineController::FileMove() {
    constexpr DWORD buffer_size = 260ul;
    TCHAR to_video_filename[buffer_size] = {0};

    OPENFILENAME open_filename = {0};
    open_filename.lStructSize = sizeof(OPENFILENAME);
    open_filename.hwndOwner = FindWindow(CustLine::GetCustLineClassName().data(), CustLine::GetCustLineTitle().data());
    open_filename.lpstrFile = &to_video_filename[0];
    open_filename.nMaxFile = buffer_size;
    open_filename.lpstrFilter = _T("Video File(.mp4)\0*.mp4\0");
    open_filename.nFilterIndex = 1;
    open_filename.lpstrTitle = _T("Save video file!");
    open_filename.lpstrFileTitle = NULL;
    open_filename.nMaxFileTitle = 0;
    open_filename.lpstrInitialDir = NULL;
    open_filename.Flags = OFN_OVERWRITEPROMPT | OFN_NOTESTFILECREATE;

    if(!GetSaveFileName(&open_filename))
        return E_FAIL;

    std::wstring to(to_video_filename);
    to += L".mp4";

    if(!MoveFileExW(m_filename.data(), to.data(), MOVEFILE_REPLACE_EXISTING))
        return E_FAIL;

    return S_OK;
}

HRESULT CustLineController::SetFilename() {
    constexpr DWORD buffer_size = 260ul;
    TCHAR from_path_to_video_filename[buffer_size] = {0};

    if (!GetModuleFileNameW(NULL, from_path_to_video_filename, buffer_size))
        return E_FAIL;

    std::wstring from(from_path_to_video_filename);
    from.replace(from.begin() + from.find_last_of(L"\\") + 1, from.end(), L"");
    m_filename = from + std::to_wstring(std::time(NULL) + rand()) + L".mp4";

    return S_OK;
}
