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
        m_status = PLAY;

        if (m_push_frame_process.joinable())
            m_push_frame_process.join();

        m_push_frame_process = std::thread(&CustLineController::PushFrame, this);
    }

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

CustLineController::~CustLineController() {
    m_status = STOP;
    if (m_push_frame_process.joinable())
        m_push_frame_process.join();
}

const CustLineController::WinCustStatus CustLineController::GetStatus() const {
    return m_status;
}

HWND CustLineController::GetCustHWND() const {
    return m_frame_creator.GetHWND();
}

HRESULT CustLineController::PushFrame() {
    const unsigned int size_of_frame_data = m_frame_creator.GetWidthFrame() * m_frame_creator.GetWidthFrame() * 4;
    std::vector<BYTE> frame(size_of_frame_data);
    auto next_frame = std::chrono::system_clock::now();
    auto last_frame = next_frame - std::chrono::duration<int64_t, std::ratio<1, m_fps>>{1};
    while (true) {
        do {
            std::this_thread::sleep_until(next_frame);
        } while (m_status == PAUSE);

        if (m_status == STOP)
            break;

        GetBitmapBits(m_frame_creator.GetFrame(), frame.size(), frame.data());
        std::thread([this, &frame]() {
            m_video_creator.AddFrame(frame, true);
        }).detach();

        last_frame = next_frame;
        next_frame += std::chrono::duration<int64_t, std::ratio<1, m_fps>>{1};
    }

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

    if(!GetSaveFileName(&open_filename)) {
        DeleteFileW(m_filename.data());
        return E_FAIL;
    }

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
    m_filename = from + std::to_wstring(std::time(NULL)) + L".mp4";

    return S_OK;
}
