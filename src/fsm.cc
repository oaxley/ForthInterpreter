/*
 * @file    fsm.h
 * @author  Sebastien LEGRAND
 *
 * @brief   Interface / Finite State Machine
 */

// ----- includes
#include "fsm.h"

// ----- begin namespace
namespace FSM {

// constructor
Engine::Engine()
{
}

// destructor
/*virtual*/ Engine::~Engine()
{
}

// setup the FSM
void Engine::setup()
{
}

/* add a transition to the FSM
 * Args:
 *      transition : the transition to add to the FSM
 */
void Engine::add(const Transition& transition)
{
    // begin state lookup | record state object
    if (!states_.contains(transition.begin.name)) {
        states_[transition.begin.name]["__object"] = transition.begin;
    }

    // end state lookup | record state object
    if (!states_.contains(transition.end.name)) {
        states_[transition.end.name]["__object"] = transition.end;
    }

    // associate the begin state with the event and end state
    states_[transition.begin.name][transition.event.name] = transition.end;
}

/* add a list of transitions to the FSM
 * Args:
 *      transitions : the list of transitions to add
 */
void Engine::add(std::list<Transition> transitions)
{
    for (auto t : transitions) {
        add(t);
    }
}

/* get the current state of the FSM
 * Returns:
 *      the name of the current state
 */
std::string_view Engine::state() const
{
    return current_.name;
}

// start the FSM
void Engine::start()
{
    // lookup for the starting state
    for (const auto& state : states_) {
        for (const auto& event : state.second) {
            if (event.second.type == StateType::BEGIN_STATE) {
                current_ = event.second;
            }
        }
    }
    has_ended_ = false;
}

// stop the FSM
void Engine::stop()
{
    // lookup for the ending state
    for (const auto& state : states_) {
        for (const auto& event : state.second) {
            if (event.second.type == StateType::END_STATE) {
                current_ = event.second;
            }
        }
    }
    has_ended_ = true;
}

// update the FSM with a new event
void Engine::update(const Event& event)
{

}

/* check if a transition from the current State, to the one specified is possible
 * Args:
 *      state : the target state to check
 * Returns:
 *      True if the transition is possible, False otherwise
 */
bool Engine::can(const State& state) const
{
    // retrieve the map for the current state
    const auto& map = states_.at(current_.name);

    // check if the state exists in the map
    return map.contains(state.name);
}

/* check if a transition from the current State, to the one specified is NOT possible
 * Args:
 *      state : the target state to check
 * Returns:
 *      True if the transition is not possible, False otherwise
 */
bool Engine::cannot(const State& state) const
{
    return !can(state);
}

// ----- end namespace
}
