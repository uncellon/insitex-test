#ifndef INSITEX_IPC_LOGGER_STRATEGY_H
#define INSITEX_IPC_LOGGER_STRATEGY_H

#include <chrono>
#include <ctime>
#include <string_view>

namespace Insitex {
namespace IPC {

class LoggerStrategy {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    virtual ~LoggerStrategy() = default;

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual void write(
        std::chrono::microseconds processingTime,
        std::string_view message
    ) = 0;

}; // class LoggerStrategy

} // namespace IPC
} // namespace Insitex

#endif // INSITEX_IPC_LOGGER_STRATEGY_H