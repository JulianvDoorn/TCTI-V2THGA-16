#pragma once

/**
 * @class	InteractionGroup
 *
 * @brief	An interaction group.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class InteractionGroup {
public:

	/**
	 * @fn	virtual void InteractionGroup::resolve() = 0;
	 *
	 * @brief	Resolves this object
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	virtual void resolve() = 0;
};