#include "client.h"
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

namespace Insitex {
namespace IPC {

/******************************************************************************
 * Constructors / Destructors
 *****************************************************************************/

Client::~Client() {
    close(mShmFd);
}

/******************************************************************************
 * Methods
 *****************************************************************************/

void Client::connect(const std::string& shmName) {
    mShmFd = shm_open(
        shmName.c_str(),
        O_RDWR,
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP
    );
    if (mShmFd == -1) {
        throw std::system_error(errno, std::generic_category(), "shm_open(...) failed");
    }

    // Map only "header" of ShmLayout without data to detect server chunk size
    ShmLayout<1>* shmBuffer = reinterpret_cast<ShmLayout<1>*>(mmap(
        nullptr,
        sizeof(ShmLayout<1>),
        PROT_READ,
        MAP_SHARED,
        mShmFd,
        0
    ));
    if (shmBuffer == MAP_FAILED) {
        throw std::system_error(errno, std::generic_category(), "mmap(...) failed");
    }

    mChunkSize = shmBuffer->chunkSize;
    if (!mChunkSize) {
        throw std::runtime_error("Server chunk size is zero!");
    }

    // Re-map with actual chunk size
    if (munmap(shmBuffer, sizeof(ShmLayout<1>)) == -1) {
        throw std::system_error(errno, std::generic_category(), "munmap(...) failed");
    }
    shmBuffer = reinterpret_cast<ShmLayout<1>*>(mmap(
        nullptr,
        sizeof(ShmLayout<1>) + (mChunkSize - 1) * sizeof(char),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        mShmFd,
        0
    ));
    if (shmBuffer == MAP_FAILED) {
        throw std::system_error(errno, std::generic_category(), "mmap(...) failed");
    }

    mShmLayout = shmBuffer;
}

void Client::writeAndWaitReply(const std::string& message) {
    if (!message.length() || !mShmLayout) {
        return;
    }
    this->send(message, mShmLayout);
    auto reply = this->receive(mShmLayout);
    std::cout << "Reply: " << reply << std::endl;
}

} // namespace IPC
} // namespace Insitex