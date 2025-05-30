#ifndef REVERSE_DATA_PROCESS_STRATEGY_H
#define REVERSE_DATA_PROCESS_STRATEGY_H

#include "dataprocessstrategy.h"
#include <algorithm>
#include <string>

class ReverseDataProcessStrategy : public Insitex::IPC::DataProcessStrategy {
public:
    /**************************************************************************
     * Methods
     *************************************************************************/

    std::string process(std::string_view data) const override {
        std::string result{ data };
        std::reverse(result.begin(), result.end());
        return result;
    }

}; // class ReverseDataProcessStrategy

#endif // REVERSE_DATA_PROCESS_STRATEGY_H