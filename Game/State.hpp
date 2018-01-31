#pragma once

/**
 * @class	State
 *
 * @brief	A game state interface.
 *
 * @author	Julian
 * @date	2018-01-19
 */

class State {
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
	 * @fn	virtual State::~State()
	 *
	 * @brief	Destructor
	 *
	 * @author	Julian
	 * @date	2018-01-22
	 */

	virtual ~State() { }
};