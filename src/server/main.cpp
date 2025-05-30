#include "fileloggerstrategy.h"
#include "reversedataprocessstrategy.h"
#include <csignal>
#include <insitex/ipc/server.h>
#include <iostream>
#include <memory>

// Insitex::IPC::Server<1024> server(nullptr); // default strategy
Insitex::IPC::Server<1024> server(std::make_unique<ReverseDataProcessStrategy>());

void sigHandler(int) {
    server.requireStop();
}

int main(int argc, char* argv[]) {
    struct sigaction sa{};
    sa.sa_handler = sigHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, nullptr) == -1) {
        std::cerr << "sigaction(...) failed\n";
    }

    server.setLogger(std::make_unique<FileLoggerStrategy>("log.txt"));

    std::cout << "Press Ctrl+C to exit...\n";

    try {
        server.listen("insitex-ipc-server");
    } catch (const std::system_error &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}