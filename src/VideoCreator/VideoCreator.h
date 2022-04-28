#pragma once
#include <vector>
#include <string>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#include <atlbase.h>

class VideoCreator
{
public:
    VideoCreator() = default;
    ~VideoCreator();
    HRESULT Initialize(const unsigned int FPS, const unsigned int width, const unsigned int height, const std::wstring& filename);
    HRESULT StartWrite();
    HRESULT AddFrame(std::vector<BYTE>& frame, const bool isUpDown = false);
    HRESULT StopWrite();
private:
    enum StatusOfVideo
    {
        INVALID_STATUS = 0,
        INITIALIZE_STATUS = 1, 
        WRITE_STATUS = 2,
    };

    HRESULT InitializeOutMedia(CComPtr<IMFMediaType>& out_media) const;
    HRESULT InitializeInputMedia(CComPtr<IMFMediaType>& input_media) const;

    const GUID m_video_encoding_format = MFVideoFormat_H264;
    const GUID m_input_format = MFVideoFormat_RGB32;
    const double m_bit_per_pixel = 0.1;

    CComPtr<IMFSinkWriter> m_writter = nullptr;
    unsigned long m_stream_index = 0ul;

    unsigned int m_fps = 0u;
    unsigned int m_frame_duration = 0u;
    unsigned long long int m_frame_position = 0ull;
    unsigned long int m_bitrate = 0ul;
    unsigned int m_width = 0u;
    unsigned int m_height = 0u;
    std::wstring m_filename = {};

    StatusOfVideo video_status = INVALID_STATUS;
};
