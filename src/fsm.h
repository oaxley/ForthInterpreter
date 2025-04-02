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

// main FSM class
class Engine
{
public:
    // constructor and destructor
    Engine();
    virtual ~Engine();

    // no copy or move semantics
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(const Engine&&) = delete;
    Engine& operator=(Engine&&) = delete;

    // setup the FSM
    void setup();

    // add transitions to the engine
    void add(std::list<Transition>);
    void add(Transition);

    // get the current state of the FSM
    std::string_view state();

    // start and stop the FSM
    void start();
    void stop();

    // update the FSM according to the new event
    void update(const Event&);

    // check if a transition to a new state is possible (can) or not (cannot)
    bool can(const State&);
    bool cannot(const State&) const;

private:
    bool has_ended_;
    State current_;
    std::unordered_map<std::string, std::unordered_map<std::string, State>> states_;
};


// ----- end namespace
}

#endif // FORTH_FSM_H_
