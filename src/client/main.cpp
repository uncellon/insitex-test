#include "insitex/ipc/client.h"
#include <iostream>
#include <string>
#include <system_error>

int main(int argc, char* argv[]) {
    Insitex::IPC::Client client;
    try {
        client.connect("insitex-ipc-server");
    } catch (const std::system_error& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    std::cout << "Welcome to Insitex test application #1!\n";
    std::cout << "Enter messages or type 'quit' to exit\n";

    std::string input;

    while (true) {
        std::getline(std::cin, input, '\n');

        if (input == "quit") {
            break;
        }

        client.writeAndWaitReply(input);
    }

    // for (int i = 0; i < 10000; ++i) {
    //     client.writeAndWaitReply("Message #" + std::to_string(i));
    // }

    return 0;
}