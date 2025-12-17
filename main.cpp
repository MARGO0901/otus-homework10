#include <cstring>
#include <iostream>
#include <string>

#include "lib/async.h"
#include "lib/worker.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cout << "Not enouth arguments" << std::endl;
        return 1;
    }

    if (argv[1] == nullptr || strlen(argv[1]) == 0) {
        std::cout << "Empty argument" << std::endl;
        return 1;
    }

    std::size_t bulk;
    std::string programName = "bulk";

    try {
        bulk = std::stoi(argv[1]);
    } catch(const std::invalid_argument& e) {
        std::cout << "Invalid argument" << std::endl;
    } catch(const std::out_of_range& e) {
        std::cout << "Too laught number" << std::endl;
    }

    std::string line;

    auto h = async::connect(bulk);
    
    while(std::getline(std::cin, line)) {
        async::receive(h, line.c_str(), line.size());
    }
    async::disconnect(h);
    async::GlobalWorker::stop();

    /*
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);
    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);
    async::disconnect(h);
    async::disconnect(h2);*/
    
    return 0;
}
