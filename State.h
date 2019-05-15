#ifndef _STATE_H_
#define _STATE_H_

#include <string>
#include <vector>
#include "UniqueIntGenerator.h"

namespace FSM {

class State {
public:
    State(const std::string des) : m_description(des), m_code(UniqueIntGenerator::NextInt()) {}

    virtual void Enter() = 0;

    virtual void Run() = 0;

    virtual void Stop() = 0;

    virtual void Exit() = 0;

    int GetCode() {
        return m_code;
    }

    std::string& GetDescription() {
        return m_description;
    }

    virtual ~State(){};

private:
    std::string m_description;
    int m_code;
};

}  // namespace FSM

#endif  // !_STATE_H_
