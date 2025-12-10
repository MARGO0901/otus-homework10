#include "handler.h"
#include "worker.h"
#include <memory>

namespace async {

std::atomic<int> Handler::nextHandlerId{1};

Handler::Handler(std::size_t bulk) : handlerId(nextHandlerId++), bulkSize(bulk) {
//    std::cout << "Create handler, id = " << handlerId << std::endl;

    //запуск общих потоков
    static bool workerStarted = false;
    if(!workerStarted) {
        GlobalWorker::start();
        workerStarted = true;
    }  

    currentState = std::make_unique<NormalState>();
}

Handler::~Handler() {
//    std::cout << "Destroy handler, id = " << handlerId << std::endl;
    printVector();
}

void Handler::receive(const char *data, std::size_t size) {
    if (!data || size <= 0) return;

    std::string input(data, size);

    std::istringstream stream(input);
    std::string line;

    while(std::getline(stream, line)) {
        if (!line.empty()) processLine(line);
    }
}  

int Handler::getBlockLevel() {
    return blockLevel;
}

void Handler::incrementBlock() {
    blockLevel++;
}

void Handler::decrementBlock() {
    blockLevel++;
}

void Handler::setState(std::unique_ptr<State> newState) {
    currentState = std::move(newState);
}

void Handler::addToVector(const std::string &line) {
    vec.push_back(line);
}

void Handler::processLine(const std::string &line) {
    if(currentState) {
        currentState->ProcessLine(line, this);
    }
}

void Handler::printVector() {
    if (!vec.empty()) {
        std::stringstream stream;
        stream << "bulk: ";
        for (const auto &v : vec) {
            stream << v;
        }
        //std::cout << "stream = " << stream.str() << std::endl;

        GlobalWorker::logToConsole(handlerId, stream.str());
        GlobalWorker::logToFile(handlerId, stream.str());
    }
    vec.clear();
}

int Handler::getVectorSize() const { 
    return vec.size(); 
}

int Handler::getN() const {
    return bulkSize;
}

}