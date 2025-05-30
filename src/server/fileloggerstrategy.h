#ifndef FILE_LOGGER_STRATEGY_H
#define FILE_LOGGER_STRATEGY_H

#include "loggerstrategy.h"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

class FileLoggerStrategy : public Insitex::IPC::LoggerStrategy {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    FileLoggerStrategy(const std::string& path);
    ~FileLoggerStrategy();

    /**************************************************************************
     * Methods
     *************************************************************************/

    void write(
        std::chrono::microseconds microseconds,
        std::string_view message
    ) override;

protected:
    /**************************************************************************
     * Methods (Protected)
     *************************************************************************/

    void writeLoop(const std::string& logPath);

    /**************************************************************************
     * Members
     *************************************************************************/

    std::condition_variable mCv;
    std::mutex mMessagesToWriteMtx;
    std::queue<std::string> mMessagesToWrite;
    std::thread mWriteThread;
    bool mIsRunning = true;

}; // class FileLoggerStrategy

#endif // FILE_LOGGER_STRATEGY_H