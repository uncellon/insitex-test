#ifndef INSITEX_IPC_SEM_UTILS_H
#define INSITEX_IPC_SEM_UTILS_H

#include <semaphore.h>
#include <system_error>

namespace Insitex {
namespace IPC {

class SemUtils {
public:
    /**************************************************************************
     * Methods
     *************************************************************************/

    inline static void wait(sem_t* semaphore) {
        if (sem_wait(semaphore) == -1) {
            throw std::system_error(errno, std::generic_category(), "sem_wait(...) failed");
        }
    }

    inline static void post(sem_t* semaphore) {
        if (sem_post(semaphore) == -1) {
            throw std::system_error(errno, std::generic_category(), "sem_post(...) failed");
        }
    }

}; // class SemUtils

} // namespace IPC
} // namespace Insitex

#endif // INSITEX_IPC_SEM_UTILS_H