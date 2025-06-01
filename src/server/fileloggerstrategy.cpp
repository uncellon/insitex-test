#include "fileloggerstrategy.h"

/******************************************************************************
 * Constructors / Destructors
 *****************************************************************************/

FileLoggerStrategy::FileLoggerStrategy(const std::string& path) {
    mLogFile.open(path, std::ios::app);
    if (!mLogFile.is_open()) {
        throw std::runtime_error("Failed to create/open log file");
    }
    mWriteThread = std::thread(&FileLoggerStrategy::writeLoop, this);
}

FileLoggerStrategy::~FileLoggerStrategy() {
    mIsRunning = false;
    mCv.notify_one();
    mWriteThread.join();

    mLogFile.close();
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

void FileLoggerStrategy::writeLoop() {
    std::unique_lock lock(mMessagesToWriteMtx);
    while (mIsRunning) {
        if (mMessagesToWrite.empty()) {
            mCv.wait(lock);
            continue;
        }

        mLogFile << mMessagesToWrite.front();
        mMessagesToWrite.pop();
        mLogFile.flush();
    }

    mLogFile.close();
}