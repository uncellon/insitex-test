#ifndef INSITEX_IPC_SHM_LAYOUT_H
#define INSITEX_IPC_SHM_LAYOUT_H

#include <semaphore.h>

namespace Insitex{
namespace IPC {

template<size_t ChunkSize>
    requires (ChunkSize > 0)
struct ShmLayout {
    sem_t readSem;
    sem_t writeSem;
    sem_t dataReadySem;
    size_t chunkSize = ChunkSize;
    size_t remainingChunks = 0;
    char chunk[ChunkSize];
}; // struct ShmLayout

} // namespace IPC
} // namespace Insitex

#endif // INSITEX_IPC_SHM_LAYOUT_H