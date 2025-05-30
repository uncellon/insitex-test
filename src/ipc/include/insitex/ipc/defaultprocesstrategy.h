#ifndef INSITEX_IPC_DEFAULT_DATA_PROCESS_STRATEGY_H
#define INSITEX_IPC_DEFAULT_DATA_PROCESS_STRATEGY_H

#include "dataprocessstrategy.h"
#include <string>

namespace Insitex {
namespace IPC {

class DefaultDataProcessStrategy : public DataProcessStrategy {
    /**************************************************************************
     * Methods
     *************************************************************************/

    std::string process(std::string_view data) const override {
        return std::string(data);
    }

}; // class DefaultDataProcessStrategy

} // namespace IPC
} // namespace Insitex

#endif // INSITEX_IPC_DEFAULT_DATA_PROCESS_STRATEGY_H