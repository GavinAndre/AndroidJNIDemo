#include "android_buf.h"

AndroidBuf::AndroidBuf() {
    buffer_[BUFFER_SIZE] = '\0';
    setp(buffer_, buffer_ + BUFFER_SIZE - 1);
}

AndroidBuf::~AndroidBuf() {
    sync();
}

int AndroidBuf::flush_buffer() {
    int len = int(pptr() - pbase());
    if (len <= 0)
        return 0;

    if (len <= BUFFER_SIZE)
        buffer_[len] = '\0';

#ifdef ANDROID
    android_LogPriority t = ANDROID_LOG_INFO;
    __android_log_write(t, "Native", buffer_);
#else
    LOGI("%s", buffer_);
#endif

    pbump(-len);
    return len;
}
