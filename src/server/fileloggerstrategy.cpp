#include "fileloggerstrategy.h"
#include <chrono>
#include <format>
#include <fstream>
#include <mutex>

/******************************************************************************
 * Constructors / Destructors
 *****************************************************************************/

FileLoggerStrategy::FileLoggerStrategy(const std::string& path) {
    mWriteThread = std::thread(&FileLoggerStrategy::writeLoop, this, path);
}

FileLoggerStrategy::~FileLoggerStrategy() {
    mIsRunning = false;
    mCv.notify_one();
    mWriteThread.join();
}

/******************************************************************************
 * Methods
 *****************************************************************************/

void FileLoggerStrategy::write(
    std::chrono::microseconds microseconds,
    std::string_view message)
{
    auto currentTime = std::chrono::system_clock::now();
    std::string currentTimeStr = std::format("{:%Y-%m-%d %H:%M:%S}", currentTime);


    std::unique_lock lock(mMessagesToWriteMtx);
    mMessagesToWrite.emplace(
        std::format("[{}] took {}, message: {}\n", currentTimeStr, microseconds, message)
    );
    mCv.notify_one();
}

/******************************************************************************
 * Methods (Protected)
 *****************************************************************************/

void FileLoggerStrategy::writeLoop(const std::string& logPath) {
    std::ofstream logFile(logPath, std::ios::app);

    std::unique_lock lock(mMessagesToWriteMtx);
    while (mIsRunning) {
        if (mMessagesToWrite.empty()) {
            mCv.wait(lock);
            continue;
        }

        logFile << mMessagesToWrite.front();
        mMessagesToWrite.pop();
        logFile.flush();
    }
}