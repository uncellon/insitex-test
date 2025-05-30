#ifndef INSITEX_IPC_DATA_PROCESS_STRATEGY_H
#define INSITEX_IPC_DATA_PROCESS_STRATEGY_H

#include <string_view>

namespace Insitex {
namespace IPC {

class DataProcessStrategy {
public:
    /**************************************************************************
     * Constructors / Destructors
     *************************************************************************/

    virtual ~DataProcessStrategy() = default;

    /**************************************************************************
     * Methods
     *************************************************************************/

    virtual std::string process(std::string_view data) const = 0;

}; // class DataProcessStrategy

} // namespace IPC
} // namespace Insitex

#endif // INSITEX_IPC_DATA_PROCESS_STRATEGY_H