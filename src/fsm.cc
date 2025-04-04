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
 *      s : the state to add to the FSM
 * Returns:
 *      The index where the state has been stored
 */
int Engine::add(State s)
{
    states_.push_back(std::move(s));
    return static_cast<int>(states_.size() - 1);
}

/* add a new event to the FSM
 * Args:
 *      e : the event to add to the FSM
 * Returns:
 *      The index where the event has been stored
 */
int Engine::add(Event e)
{
    events_.push_back(std::move(e));
    return static_cast<int>(events_.size() - 1);
}


/* add a transition to the FSM
 * Args:
 *      t : the transition to add to the FSM
 */
void Engine::add(Transition t)
{
    transitions_[t.begin_state][t.event] = t.end_state;
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

    // inform user we exit from current state
    if (states_[current_].exit != "")
        queue_.push(states_[current_].exit);

    // move to the end state of the transition
    current_ = map.at(event);

    // inform the user we enter to current state
    if (states_[current_].enter != "")
        queue_.push(states_[current_].enter);

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

/* check if the FSM has ended
 * Return:
 *      True if the FSM has ended
 */
bool Engine::hasEnded() const
{
    return has_ended_;
}

/* return the number of states
 * Returns:
 *      The number of states in the FSM
 */
int Engine::states() const
{
    return static_cast<int>(states_.size());
}

/* return the number of events
 * Returns:
 *      The number of events in the FSM
 */
int Engine::events() const
{
    return static_cast<int>(events_.size());
}

/* return the number of transitions
 * Returns:
 *      The number of transitions in the FSM
 */
int Engine::transitions() const
{
    int sum {0};
    for (const auto& map : transitions_) {
        sum = sum + static_cast<int>(map.second.size());
    }
    return sum;
}

/* return the index of particular state
 * Args:
 *      name : the name of the state to look for
 * Returns:
 *      The index of the state or -1 if it cannot be found
 */
int Engine::stateIndex(std::string name) const
{
    for (int i = 0; i < static_cast<int>(states_.size()); ++i) {
        if (states_[i].name == name)
            return i;
    }
    return -1;
}

/* return the index of particular event
 * Args:
 *      name : the name of the event to look for
 * Returns:
 *      The index of the event or -1 if it cannot be found
 */
int Engine::eventIndex(std::string name) const
{
    for (int i = 0; i < static_cast<int>(events_.size()); ++i) {
        if (events_[i].name == name)
            return i;
    }
    return -1;
}


// ----- end namespace
}
