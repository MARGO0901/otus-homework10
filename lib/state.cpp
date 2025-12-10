#include "state.h"

#include <memory>

#include "handler.h"

namespace async {

void NormalState::ProcessLine(const std::string &line, Handler *handler) {
    if (line == "{") {
        if (handler->getBlockLevel() == 0) {
        handler->printVector();
        }
        handler->incrementBlock();
        handler->setState(std::make_unique<BlockState>());
    } else if (line == "}") {
    } else {
        handler->addToVector(line);
        if (handler->getVectorSize() >= handler->getN()) {
            handler->printVector();
        }
    }
}

void NormalState::ProcessEnd(Handler *handler) {
    handler->printVector();
}

void BlockState::ProcessLine(const std::string &line, Handler *handler) {
    if (line == "{") {
        handler->incrementBlock();
    } else if (line == "}") {
        handler->decrementBlock();
        if (handler->getBlockLevel() == 0) {
        handler->setState(std::make_unique<NormalState>());
        handler->printVector();
        }
    } else {
        handler->addToVector(line);
    }
}

void BlockState::ProcessEnd(Handler *handler) {
    return;
}

}