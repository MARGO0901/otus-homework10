#pragma once

#include <iostream>
#include <sstream>
#include <future>

#include "worker.h"
#include "state.h"

namespace async {

class Handler{
public:
    Handler(std::size_t bulk);
    ~Handler();
    void receive(const char *data, std::size_t size);

    int getBlockLevel();
    void incrementBlock();
    void decrementBlock();

    void setState(std::unique_ptr<State> newState);
    void addToVector(const std::string &line);
    void printVector();
    int getVectorSize() const;
    int getN() const;

    
private:
    void processLine(const std::string &line);

    std::unique_ptr<State> currentState;
    const std::size_t bulkSize;
    int blockLevel;
    static std::atomic<int> nextHandlerId;
    const int handlerId;
    std::vector<std::string> vec;

    std::future<void> consoleFuture;
    std::future<void> file1Future;
    std::future<void> file2Future;

};

}