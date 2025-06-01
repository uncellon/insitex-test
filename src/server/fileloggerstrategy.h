#ifndef FILE_LOGGER_STRATEGY_H
#define FILE_LOGGER_STRATEGY_H

#include "loggerstrategy.h"
#include <condition_variable>
#include <fstream>
#include <queue>

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

    void writeLoop();

    /**************************************************************************
     * Members
     *************************************************************************/

    std::condition_variable mCv;
    std::mutex mMessagesToWriteMtx;
    std::queue<std::string> mMessagesToWrite;
    std::thread mWriteThread;
    std::ofstream mLogFile;
    bool mIsRunning = true;

}; // class FileLoggerStrategy

#endif // FILE_LOGGER_STRATEGY_H