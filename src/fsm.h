/*
 * @file    fsm.h
 * @author  Sebastien LEGRAND
 *
 * @brief   Interface / Finite State Machine
 */

// ----- header guards
#ifndef FORTH_FSM_H_
#define FORTH_FSM_H_

// ----- includes
#include <list>
#include <queue>
#include <string>
#include <unordered_map>

// ----- begin namespace
namespace FSM {

// define the different types for a State
enum class StateType {
    UNKNOWN_STATE,
    BEGIN_STATE,
    NORMAL_STATE,
    END_STATE
};

// define a State in the FSM
struct State
{
    std::string name {};
    StateType type { StateType::UNKNOWN_STATE };
    std::string enter {};
    std::string exit {};
};

// define an Event in the FSM
struct Event
{
    std::string name {};
};

// define a Transition in the FSM
struct Transition
{
    Event event;
    State begin;
    State end;
};

// define user queue type
using UserQueue_T = std::queue<std::string>;

// main FSM class
class Engine
{
public:
    // constructor and destructor
    Engine(UserQueue_T& queue);
    virtual ~Engine();

    // no copy or move semantics
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(const Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    // add transitions to the engine
    void add(std::list<Transition>);
    void add(const Transition&);

    // get the current state of the FSM
    std::string_view state() const;

    // start and stop the FSM
    void start();
    void stop();

    // update the FSM according to the new event
    bool update(const Event&);

    // check if a transition to a new state is possible (can) or not (cannot)
    bool can(const State&) const;
    bool cannot(const State&) const;

private:
    bool has_ended_ {true};
    State current_;

    using EventMap_T = std::unordered_map<std::string, State>;
    using StateMap_T = std::unordered_map<std::string, EventMap_T>;
    StateMap_T states_;

    UserQueue_T& queue_;
};


// ----- end namespace
}

#endif // FORTH_FSM_H_
