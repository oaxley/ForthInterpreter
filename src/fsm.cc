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
Engine::Engine(UserQueue_T& queue) :
    queue_{queue}
{
}

// destructor
/*virtual*/ Engine::~Engine()
{
}

/* add a new state to the FSM
 * Args:
 *      state : the state to add to the FSM
 * Returns:
 *      The index where the state has been stored
 */
int Engine::addState(State s)
{
    states_.push_back(std::move(s));
    return states_.size() - 1;
}

/* add a new event to the FSM
 * Args:
 *      event : the event to add to the FSM
 * Returns:
 *      The index where the event has been stored
 */
int Engine::addEvent(Event e)
{
    events_.push_back(std::move(e));
    return events_.size() - 1;
}

/* add a transition to the FSM
 * Args:
 *      begin (int) : the begin state index
 *      event (int) : the event index
 *      end (int)   : the end state index
 */
void Engine::addTransition(int begin, int event, int end)
{
    transitions_[begin][event] = end;
}

/* get the current state of the FSM
 * Returns:
 *      the name of the current state
 */
std::string_view Engine::state() const
{
    return states_[current_].name;
}

// start the FSM
bool Engine::start()
{
    // go through all the states, and find the one marked BEGIN_STATE
    for (int i = 0; i < static_cast<int>(states_.size()); ++i) {
        if (states_[i].type == StateType::BEGIN_STATE) {
            current_ = i;
            has_ended_ = false;
            return true;
        }
    }
    return false;
}

// stop the FSM
bool Engine::stop()
{
    // go through all the states, and find the one marked END_STATE
    for (int i = 0; i < static_cast<int>(states_.size()); ++i) {
        if (states_[i].type == StateType::END_STATE) {
            current_ = i;
            has_ended_ = true;
            return true;
        }
    }
    return false;
}

/* update the FSM with a new event
 * Args:
 *      event : the event to consider for the transition
 * Returns:
 *      True if the update is successful, false otherwise
 */
bool Engine::update(int event)
{
    // nothing to do if the FSM has ended
    if (has_ended_)
        return false;

    // ensure the event exists in our list
    if (event >= static_cast<int>(events_.size()))
        return false;

    // ensure the event exists for the current state
    const auto& map = transitions_.at(current_);
    if (!map.contains(event))
        return false;

    // move to the new state
    queue_.push(states_[current_].exit);         // push the current exit action
    current_ = map.at(event);
    queue_.push(states_[current_].enter);        // push the (new) current enter action

    // check if the state is an end state
    if (states_[current_].type == StateType::END_STATE)
        has_ended_ = true;

    return true;
}

/* check if a transition from the current State, to the one specified is possible
 * Args:
 *      state : the target state to check
 * Returns:
 *      True if the transition is possible, False otherwise
 */
bool Engine::can(int s) const
{
    // check if the state exists in the map for the current state
    for (const auto& map : transitions_.at(current_)) {
        if (map.second == s)
            return true;
    }

    return false;
}

/* check if a transition from the current State, to the one specified is NOT possible
 * Args:
 *      state : the target state to check
 * Returns:
 *      True if the transition is not possible, False otherwise
 */
bool Engine::cannot(int s) const
{
    return !can(s);
}

// ----- end namespace
}
