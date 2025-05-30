#ifndef INSITEX_IPC_SERVER_H
#define INSITEX_IPC_SERVER_H

#include "defaultprocesstrategy.h"
#include "loggerstrategy.h"
#include "peer.h"
#include <chrono>
#include <fcntl.h>           /* For O_* constants */
#include <memory>
#include <sys/mman.h>

namespace Insitex {
namespace IPC {

template<size_t ChunkSize>
class Server : Peer<ChunkSize> {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Server(std::unique_ptr<DataProcessStrategy> strategy) : mStrategy(std::move(strategy)) {
        if (!mStrategy) {
            mStrategy = std::make_unique<DefaultDataProcessStrategy>();
        }
    }
    Server(const Server& other) = delete;
    Server(Server&& other) = delete;
    ~Server() {
        shm_unlink(mShmName.c_str());
    }

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    Server<ChunkSize>& operator=(const Server<ChunkSize>& other) = delete;
    Server<ChunkSize>& operator=(Server<ChunkSize>&& other) = delete;

    /**************************************************************************
     * Methods
     *************************************************************************/

    void listen(const std::string& shmName) {
        mShmFd = shm_open(
            shmName.c_str(),
            O_CREAT | O_EXCL | O_RDWR,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
        );
        if (mShmFd == -1) {
            throw std::system_error(errno, std::generic_category(), "shm_open(...) failed");
        }
        mShmName = shmName;

        if (ftruncate(mShmFd, sizeof(ShmLayout<ChunkSize>)) == -1) {
            throw std::system_error(errno, std::generic_category(), "ftruncate(...) failed");
        }

        mShmLayout = reinterpret_cast<ShmLayout<ChunkSize>*>(
            mmap(nullptr, sizeof(ShmLayout<ChunkSize>), PROT_READ | PROT_WRITE, MAP_SHARED, mShmFd, 0)
        );
        if (mShmLayout == MAP_FAILED) {
            throw std::system_error(errno, std::generic_category(), "mmap(...) failed");
        }

        // Initialize layout data
        if (sem_init(&mShmLayout->readSem, 1, 0) == -1) {
            throw std::system_error(errno, std::generic_category(), "sem_init(sendSem, ...) failed");
        }
        if (sem_init(&mShmLayout->writeSem, 1, 0) == -1) {
            throw std::system_error(errno, std::generic_category(), "sem_init(recvSem, ...) failed");
        }
        if (sem_init(&mShmLayout->dataReadySem, 1, 0) == -1) {
            throw std::system_error(errno, std::generic_category(), "sem_init(dataReadySem, ...) failed");
        }
        mShmLayout->chunkSize = ChunkSize;

        // Start logging thread? dependency injection?

        std::string buffer;
        mIsRunning = true;
        while (mIsRunning) {
            try {
                buffer = this->receive(mShmLayout);
            } catch (const std::system_error& e) {
                // Ctrl+C pressed
                if (e.code() == std::error_code(EINTR, std::generic_category())) {
                    break;
                }
                throw e;
            }

            // Temp
            if (buffer == "halt") {
                mIsRunning = false;
                continue;
            }

            // Process data
            // std::cout << "Received: " << buffer << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            this->send(mStrategy->process(buffer), mShmLayout);
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            duration.count();

            if (mLogger) {
                mLogger->write(
                    std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime),
                    "Received: " + buffer
                );
            }
        }
    }

    inline void requireStop() {
        mIsRunning = false;
    }

    /**************************************************************************
     * Accessors / Mutators
     *************************************************************************/

    inline void setLogger(std::unique_ptr<LoggerStrategy> logger) { mLogger = std::move(logger); }

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    int mShmFd = -2;
    bool mIsRunning = false;
    std::string mShmName;
    ShmLayout<ChunkSize>* mShmLayout;
    std::string mBuffer;
    std::unique_ptr<DataProcessStrategy> mStrategy = nullptr;
    std::unique_ptr<LoggerStrategy> mLogger = nullptr;

}; // class Server

} // namspace IPC
} // namespace Insitex

#endif // INSITEX_IPC_SERVER_H