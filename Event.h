#ifndef _EVENT_H_
#define _EVENT_H_

#include <string>
#include <vector>

namespace FSM {

class Event;

class EventHandler {
public:
    virtual void Handle(Event* e) = 0;
};

class Event {
public:
    void Trigger();

    int GetCode();

    std::string& GetDescription();

    void SetHandler(EventHandler* eh);

protected:
    Event(const std::string des);

    virtual ~Event() {};

private:
    Event(const Event& ee) = delete;

    Event& operator=(const Event& fsm) = delete;

private:
    EventHandler* m_handler;
    std::string m_description;
    int m_code;
};

class ExitEvent : public Event {
public:
    static ExitEvent& Instance();

private:
    ExitEvent() : Event("Exit Event") {}
};

class PlayEvent : public Event {
public:
    static PlayEvent& Instance();

private:
    PlayEvent() : Event("Play Event") {}

};

class BackToMenuEvent : public Event {
public:
    static BackToMenuEvent& Instance();

private:
    BackToMenuEvent() : Event("Exit Event") {}
};

class GoToScoreEvent : public Event {
public:
    static GoToScoreEvent& Instance();

private:
    GoToScoreEvent() : Event("GotoScore Event") {}
};

class DeadEvent : public Event {
public:
    static DeadEvent& Instance();

private:
    DeadEvent() : Event("Dead Event") {}
};

class RestartEvent : public Event {
public:
    static RestartEvent& Instance();

private:
    RestartEvent() : Event("Restart Event") {}
};

}  // namespace FSM

#endif  // !_EVENT_H_
