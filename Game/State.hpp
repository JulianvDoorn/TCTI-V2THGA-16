#pragma once

#include <memory>

/**
 * @class	State
 *
 * @brief	A game state interface.
 *
 * @author	Julian
 * @date	2018-01-19
 */

class State {
	/** @brief	Name of the State, used for doing transitions towards this state */
	const std::string name;
public:

	/**
	 * @fn	virtual void State::update(const float elapsedTime) = 0;
	 *
	 * @brief	Runs an update cycle for this state
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param	elapsedTime	The elapsed time.
	 */

	virtual void update(const float elapsedTime) = 0;

	/**
	 * @fn	virtual void State::entry()
	 *
	 * @brief	Entry is called once the state has been entered.
	 * 			The update function is only called after this function has returned.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	virtual void entry() { };

	/**
	 * @fn	virtual void State::exit()
	 *
	 * @brief	Exit is called once the state has been left.
	 * 			The update function is only called before this function is invoked.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 */

	virtual void exit() { };

	/**
	 * @fn	friend bool State::operator==(std::unique_ptr<State>& state, const std::string& name)
	 *
	 * @brief	Equality operator.
	 *
	 * @author	Julian
	 * @date	2018-01-19
	 *
	 * @param [in,out]	state	If non-null, the first instance to compare.
	 * @param 		  	name 	The second instance to compare.
	 *
	 * @return	state->name == name
	 */

	friend bool operator==(std::unique_ptr<State>& state, const std::string& name) {
		return state->name == name;
	}

	virtual ~State() { }
};