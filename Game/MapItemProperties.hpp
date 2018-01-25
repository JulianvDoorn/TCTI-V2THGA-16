#pragma once

#include <vector>

#include "CurlyBracketList.hpp"
#include "KeyValuePair.hpp"
#include "PropertyResolutionLambda.hpp"

/**
 * @class	MapItemProperties
 *
 * @brief	Map item properties list.
 * 	
 * @detailed MapItemProperties allows checking and applying a list of PropertyResolutionLambdas to every KeyValuePair it contains accordingly.
 * 			 It inherits from CurlyBracketList<KeyValuePair> in favor of std::vector<KeyValuePair> so it can be read from an input stream implicitly.
 *
 * @author	Julian
 * @date	2018-01-25
 */

class MapItemProperties : public CurlyBracketList<KeyValuePair> {
public:

	/**
	 * @fn	void MapItemProperties::read(const std::vector<PropertyResolutionLambda>& definitions) const
	 *
	 * @brief	Applies the right PropertyResolutionLambda to the right key value pair.
	 * 
	 * @detailed It matches the name and type of the PropertyResolutionLambda with the name and type of the KeyValuePair. 
	 * 			 When the name and type matches, fire the PropertyResolutionLambda(KeyValuePair::Value) with arguments pair.value;
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param	definitions	The PropertyResolutionLambdas that are defined for any type and name combination.
	 */

	void read(const std::vector<PropertyResolutionLambda>& definitions) const {
		for (const KeyValuePair& pair : *this) {
			auto it = std::find(definitions.begin(), definitions.end(), pair.key);

			if (it != definitions.end()) {
				if (it->type == pair.type) {
					(*it)(pair.value);
				}
			}
		}
	}
};