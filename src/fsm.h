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
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <optional>

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
    int begin_state;
    int event;
    int end_state;
};

// define user types
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


    // add a new state, event and transition
    int add(State);
    int add(Event);
    void add(Transition);

    // get the current state of the FSM
    std::string_view state() const;

    // start and stop the FSM
    bool start();
    bool stop();

    // update the FSM according to the new event
    bool update(int);
    bool update(std::string);

    // check if a transition to a new state is possible (can) or not (cannot)
    bool can(int) const;
    bool cannot(int) const;

    // check if the FSM is in END state
    bool hasEnded() const;

    // return the number of states / events / transitions
    int states() const;
    int events() const;
    int transitions() const;

    int stateIndex(std::string) const;
    std::optional<std::reference_wrapper<const State>> state(int) const;
    std::optional<std::reference_wrapper<const State>> state(std::string) const;

    int eventIndex(std::string) const;
    std::optional<std::reference_wrapper<const Event>> event(int) const;
    std::optional<std::reference_wrapper<const Event>> event(std::string) const;


private:
    bool has_ended_ {true};
    int current_ {-1};

    std::vector<State> states_;
    std::vector<Event> events_;

    // event_map        => event ID, end state ID
    // transition_map   => begin state ID, event_map
    using EventMap_T = std::unordered_map<int, int>;
    using TransitionMap_T = std::unordered_map<int, EventMap_T>;
    TransitionMap_T transitions_ {};

    UserQueue_T& queue_;
};


// ----- end namespace
}

#endif // FORTH_FSM_H_
