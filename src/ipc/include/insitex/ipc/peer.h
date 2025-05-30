#ifndef INSITEX_IPC_PEER_H
#define INSITEX_IPC_PEER_H

#include "semutils.h"
#include "shmlayout.h"
#include <cstring>

namespace Insitex {
namespace IPC {

template<size_t ChunkSize>
class Peer {
public:
    /**************************************************************************
     * Methods
     *************************************************************************/

    void send(const std::string& message, ShmLayout<ChunkSize>* layout) {
        auto length = message.length();
        auto chunkSize = layout->chunkSize;
        auto chunksToSend = (length / chunkSize) + (length % chunkSize != 0);

        for (auto currentChunk = 0; currentChunk < chunksToSend; ++currentChunk) {
            SemUtils::wait(&layout->writeSem);

            memcpy(layout->chunk, message.c_str() + currentChunk * chunkSize, chunkSize);
            layout->remainingChunks = chunksToSend - currentChunk - 1;

            SemUtils::post(&layout->readSem);
        }

        SemUtils::wait(&layout->dataReadySem);
    }

    std::string receive(ShmLayout<ChunkSize>* layout) {
        std::string result;

        while (true) {
            SemUtils::post(&layout->writeSem); // allow the client to start recording
            SemUtils::wait(&layout->readSem); // wait for the client to finish recording

            result += layout->chunk;

            if (!layout->remainingChunks) {
                break;
            }
        }

        SemUtils::post(&layout->dataReadySem);

        return result;
    }

}; // class Peer

} // namespace IPC
} // namespace Insitex

#endif // INSITEX_IPC_PEER_H