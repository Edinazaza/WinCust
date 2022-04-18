#include "VideoCreator.h"

VideoCreator::~VideoCreator() {
    StopWrite();
}

HRESULT VideoCreator::Initialize(const unsigned int FPS, const unsigned int width, const unsigned int height, const std::wstring& filename) {
    // disable
    StopWrite();

    // setup members of class  
    m_fps = FPS;
    m_width = width;
    m_height = height;
    m_filename = filename + L".mp4";

    m_frame_duration = 10 * 1000 * 1000 / m_fps;
    m_frame_position = 0u;
    m_bitrate = m_bit_per_pixel * (m_width * m_height * m_fps);

    // setup mf
    if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
        return E_FAIL;

    if (FAILED(MFStartup(MF_VERSION)))
        return E_FAIL;

    // setup writter
    if (FAILED(MFCreateSinkWriterFromURL(m_filename.data(), NULL, NULL, &m_writter)))
        return E_FAIL;

    CComPtr<IMFMediaType> out_media = nullptr;
    CComPtr<IMFMediaType> input_media = nullptr;

    if (FAILED(InitializeOutMedia(out_media)))
        return E_FAIL;

    if (FAILED(InitializeInputMedia(input_media)))
        return E_FAIL;

    if (FAILED(m_writter->AddStream(out_media, &m_stream_index)))
        return E_FAIL;

    if (FAILED(m_writter->SetInputMediaType(m_stream_index, input_media, NULL)))
        return E_FAIL;

    video_status = INITIALIZE_STATUS;
    return S_OK;
}

HRESULT VideoCreator::StartWrite() {
    if (video_status == INITIALIZE_STATUS) {
        video_status = WRITE_STATUS;
        return m_writter->BeginWriting();
    }
    return E_FAIL;
}

HRESULT VideoCreator::AddFrame(std::vector<BYTE>& frame, const bool isUpDown) {
    if (video_status != WRITE_STATUS)
        return E_FAIL;

    const long int buffer_bytes = m_width * 4;
    const unsigned long buffer_size = buffer_bytes * m_height;

    // buffer
    CComPtr<IMFMediaBuffer> buffer = nullptr;
    if (FAILED(MFCreateMemoryBuffer(buffer_size, &buffer)))
        return E_FAIL;

    BYTE* data = nullptr;
    if (FAILED(buffer->Lock(&data, NULL, NULL)))
        return E_FAIL;

    if (isUpDown) {
        if (FAILED(MFCopyImage(data, buffer_bytes, frame.data() + ((m_height - 1) * buffer_bytes), -buffer_bytes, buffer_bytes, m_height)))
            return E_FAIL;
    } else {
        if (FAILED(MFCopyImage(data, buffer_bytes, frame.data(), buffer_bytes, buffer_bytes, m_height)))
            return E_FAIL;
    }

    if (FAILED(buffer->Unlock()))
        return E_FAIL;

    if (FAILED(buffer->SetCurrentLength(buffer_size)))
        return E_FAIL;

    // sample
    CComPtr<IMFSample> sample = nullptr;
    if (FAILED(MFCreateSample(&sample)))
        return E_FAIL;

    if (FAILED(sample->AddBuffer(buffer)))
        return E_FAIL;

    if (FAILED(sample->SetSampleTime(m_frame_position)))
        return E_FAIL;

    if (FAILED(sample->SetSampleDuration(m_frame_duration)))
        return E_FAIL;

    m_frame_position += m_frame_duration;

    // write
    if (FAILED(m_writter->WriteSample(m_stream_index, sample)))
        return E_FAIL;

    return S_OK;
}

HRESULT VideoCreator::StopWrite() {
    video_status = INVALID_STATUS;
    if (m_writter) {
        if (FAILED(m_writter->Finalize()))
            return E_FAIL;
    }

    MFShutdown();
    CoUninitialize();
    return S_OK;
}

HRESULT VideoCreator::InitializeOutMedia(/*out*/ CComPtr<IMFMediaType>& out_media) const {
    if (FAILED(MFCreateMediaType(&out_media)))
        return E_FAIL;

    if (FAILED(out_media->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video)))
        return E_FAIL;

    if (FAILED(out_media->SetGUID(MF_MT_SUBTYPE, m_video_encoding_format)))
        return E_FAIL;

    if (FAILED(out_media->SetUINT32(MF_MT_AVG_BITRATE, m_bitrate)))
        return E_FAIL;

    if (FAILED(out_media->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive)))
        return E_FAIL;

    if (FAILED(MFSetAttributeSize(out_media, MF_MT_FRAME_SIZE, m_width, m_height)))
        return E_FAIL;

    if (FAILED(MFSetAttributeRatio(out_media, MF_MT_FRAME_RATE, m_fps, 1)))
        return E_FAIL;

    if (FAILED(MFSetAttributeRatio(out_media, MF_MT_PIXEL_ASPECT_RATIO, 1, 1)))
        return E_FAIL;

    return S_OK;
}

HRESULT VideoCreator::InitializeInputMedia(/*out*/CComPtr<IMFMediaType>& input_media) const {
    if (FAILED(MFCreateMediaType(&input_media)))
        return E_FAIL;

    if (FAILED(input_media->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video)))
        return E_FAIL;

    if (FAILED(input_media->SetGUID(MF_MT_SUBTYPE, m_input_format)))
        return E_FAIL;

    if (FAILED(input_media->SetUINT32(MF_MT_AVG_BITRATE, m_bitrate)))
        return E_FAIL;

    if (FAILED(input_media->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive)))
        return E_FAIL;

    if (FAILED(MFSetAttributeSize(input_media, MF_MT_FRAME_SIZE, m_width, m_height)))
        return E_FAIL;

    if (FAILED(MFSetAttributeRatio(input_media, MF_MT_FRAME_RATE, m_fps, 1)))
        return E_FAIL;

    if (FAILED(MFSetAttributeRatio(input_media, MF_MT_PIXEL_ASPECT_RATIO, 1, 1)))
        return E_FAIL;

    return S_OK;
}
