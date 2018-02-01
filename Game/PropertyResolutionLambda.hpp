#pragma once

#include <functional>
#include <string>
#include <initializer_list>

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
	 * @fn	template <class RESOLUTION_LAMBDA> PropertyResolutionLambda::PropertyResolutionLambda(const std::string& name, const KeyValuePair::Type type, RESOLUTION_LAMBDA func) : name(std::move(name)), type(type), std::function<void(KeyValuePair::Value)>(func)
	 *
	 * @brief	Constructor
	 *
	 * @author	Julian
	 * @date	2018-01-25
	 *
	 * @tparam	RESOLUTION_LAMBDA	Type of the resolution lambda.
	 * @param	name	Name of the key of the key-value pair.
	 * @param	type	Type of the deducted type of the key-value pair.
	 * @param	func	Resolution lambda function for the key-value pair.
	 */

	template <class RESOLUTION_LAMBDA>
	PropertyResolutionLambda(const std::string& name, const KeyValuePair::Type type, RESOLUTION_LAMBDA func) :
		name(std::move(name)),
		type(type),
		std::function<void(KeyValuePair::Value)>(func)
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