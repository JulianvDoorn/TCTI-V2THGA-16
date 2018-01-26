#pragma once

#include <functional>
#include <string>

#include "KeyValuePair.hpp"

/**
 * @class	PropertyResolutionLambda
 *
 * @brief	PropertyResolutionLambda is used by MapFactory to define the semantics behind key-value pairs.
 * 
 * @detailed Preferred way of defining a PropertyResolutionLambda is using the MapItemProperties
 * 			 @code
 * 			 	  MapItemProperties properties = SomeDummyFunctionReturningMapItemProperties();
 * 			 	  
 * 			 	  properties.read({
 * 			 	       { "Id", Type::String, [&](Value value) { std::cout << *value.stringValue << std::endl; },
 * 			 	       { "Location", Type::String, [&](Value value) { std::cout << *value.stringValue << std::endl; } 
 * 			 	  });
 * 			 @endcode
 *
 * @author	Julian
 * @date	2018-01-25
 */

class PropertyResolutionLambda : public std::function<void(KeyValuePair::Value)> {
public:
	const std::string name;
	const KeyValuePair::Type type;

	/**
	 * @fn	PropertyResolutionLambda::PropertyResolutionLambda(const std::string& name, const KeyValuePair::Type type, const std::function<void(KeyValuePair::Value)>& func) : name(std::move(name)), type(type), std::function<void(KeyValuePair::Value)>(std::move(func))
	 *
	 * @brief	Constructor
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param	name	Name of the key of the key-value pair.
	 * @param	type	Type of the deducted type of the key-value pair.
	 * @param	func	Resolution lambda function for the key-value pair.
	 */

	PropertyResolutionLambda(const std::string& name, const KeyValuePair::Type type, const std::function<void(KeyValuePair::Value)>& func) :
		name(std::move(name)),
		type(type),
		std::function<void(KeyValuePair::Value)>(std::move(func))
	{ }

	/**
	 * @fn	bool PropertyResolutionLambda::operator== (const std::string& rhs) const
	 *
	 * @brief	Equality operator
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @param	rhs	The right hand side.
	 *
	 * @return	True the rhs const std::string& matches name
	 */

	bool operator== (const std::string& rhs) const {
		return name == rhs;
	}
};