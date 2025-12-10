#ifndef STATE_H
#define STATE_H

#include <iostream>

namespace async {

class Handler;

class State {
    std::string name;

public:
    virtual ~State() = default;
    State(const std::string &name) : name(name) {}
    virtual void ProcessLine(const std::string &line, Handler *) = 0;
    virtual void ProcessEnd(Handler *) = 0;
};

class NormalState : public State {
public:
    NormalState() : State("NormalState") {}
    void ProcessLine(const std::string &line, Handler *context) override;
    void ProcessEnd(Handler *) override;
};

class BlockState : public State {
public:
    BlockState() : State("BlockState") {}
    void ProcessLine(const std::string &line, Handler *context) override;
    void ProcessEnd(Handler *) override;
};

}

#endif 