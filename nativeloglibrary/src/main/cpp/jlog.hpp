#include <fstream>
#include <iostream>
#include <atomic>
#include <android/log.h>

namespace jlog {
    namespace {
        const static size_t K_bufLen = 2048;
        static char l_buf[K_bufLen];
    }
    static std::ofstream l_ofsLogFile;
    static std::atomic_bool l_logMngLogEn{true};

    static void logOutput(const std::string &s) {
        if (!l_logMngLogEn) return;

        //---------------
        // M-Thread support
        //---------------
#if NO_STD_THREAD
#else
        static std::mutex l_logMutex;
        std::unique_lock<std::mutex> lock(l_logMutex);
#endif
        //------
        std::cout << s << std::endl;
        //---- Set log file
        if (l_ofsLogFile.is_open()) {
            l_ofsLogFile << (s + "\n");
            l_ofsLogFile.flush();
        }
    }

    static void logMsg(const std::string &sTag, const std::string &sMsg) {
        time_t now;
        time(&now);
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&now));
        logOutput(std::string(timeStr) + " [" + sTag + "]: " + sMsg);
    }

    extern void logVerbose(const std::string &s) {
        logMsg("Verbose", s);
    }

    extern void logDebug(const std::string &s) {
        logMsg("Debug", s);
    }

    extern void logInfo(const std::string &s) {
        logMsg("Info", s);
    }

    extern void logError(const std::string &s) {
        logMsg("Error", s);
    }

    extern void logWarn(const std::string &s) {
        logMsg("Warn", s);
    }

    extern void logFatal(const std::string &s) {
        logMsg("Fatal", s);
    }

    extern void logFileErr(const std::string &sFile) {
        logError(std::string("Fail to open file [") + sFile + "]");
    }

    extern bool initLog(const std::string &sFileLog) {
        auto &ofs = l_ofsLogFile;
        auto mode = std::ofstream::out;
        int append = true;
        if (append) mode |= std::ofstream::app;
        ofs.open(sFileLog, mode);
        if (!ofs.is_open()) {
            logFileErr(sFileLog);
            return false;
        }
        return true;
    }
}

#define  LOG_TAG    "jlog"

#define LOG2BUF(...)  \
    { std::snprintf(jlog::l_buf, 2048, __VA_ARGS__); }
#define BUF2FILE(FUNC) \
    {FUNC(std::string(LOG_TAG)+":"+std::string(jlog::l_buf));}

#define _LOGV(...) { LOG2BUF(__VA_ARGS__); BUF2FILE(jlog::logVerbose); }
#define _LOGD(...) { LOG2BUF(__VA_ARGS__); BUF2FILE(jlog::logDebug); }
#define _LOGI(...) { LOG2BUF(__VA_ARGS__); BUF2FILE(jlog::logInfo); }
#define _LOGW(...) { LOG2BUF(__VA_ARGS__); BUF2FILE(jlog::logWarn); }
#define _LOGE(...) { LOG2BUF(__VA_ARGS__); BUF2FILE(jlog::logError); }
#define _LOGF(...) { LOG2BUF(__VA_ARGS__); BUF2FILE(jlog::logFatal); }

#define  LOGV(...)  {__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);_LOGV(__VA_ARGS__);}
#define  LOGD(...)  {__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);_LOGD(__VA_ARGS__);}
#define  LOGI(...)  {__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);_LOGI(__VA_ARGS__);}
#define  LOGW(...)  {__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);_LOGW(__VA_ARGS__);}
#define  LOGE(...)  {__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);_LOGE(__VA_ARGS__);}
#define  LOGF(...)  {__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__);_LOGF(__VA_ARGS__);}

