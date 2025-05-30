#ifndef INSITEX_IPC_CLIENT_H
#define INSITEX_IPC_CLIENT_H

#include "peer.h"

namespace Insitex {
namespace IPC {

class Client : public Peer<1> {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    Client() = default;
    Client(const Client& other) = delete;
    Client(Client&& other) = delete;
    ~Client();

    /**************************************************************************
     * Overloaded operators
     *************************************************************************/

    Client& operator=(const Client& other) = delete;
    Client& operator=(Client&& other) = delete;

    /**************************************************************************
     * Methods
     *************************************************************************/

    void connect(const std::string& shmName);
    void writeAndWaitReply(const std::string& message);

protected:
    /**************************************************************************
     * Members
     *************************************************************************/

    ShmLayout<1>* mShmLayout = nullptr;
    size_t mChunkSize = 0;
    int mShmFd = -2;

}; // class Client

} // namespace IPC
} // namespace Insitex

#endif // INSITEX_IPC_CLIENT_H