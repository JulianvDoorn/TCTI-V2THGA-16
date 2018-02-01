#pragma once

#include <map>
#include <functional>
#include <memory>

/**
 * @class	FactoryException
 *
 * @brief	Exception for signalling factory errors.
 *
 * @author	Julian
 * @date	2018-01-22
 */

class FactoryException : public std::exception {
	/** @brief	The error */
	std::string err;

protected:

	/**
	 * @fn	FactoryException::FactoryException(const std::string& err) : err(std::move(err))
	 *
	 * @brief	Constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @param	err	The error.
	 */

	FactoryException(const std::string& err) : err(std::move(err)) { }

public:

	/**
	 * @fn	virtual const char* FactoryException::what() const noexcept override
	 *
	 * @brief	Gets the what
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 *
	 * @return	Null if it fails, else a pointer to a const char.
	 */

	virtual const char* what() const noexcept override {
		return err.c_str();
	}
};

/**
 * @class	FactoryInvalidId
 *
 * @brief	Thrown when BaseFactory::create(T, Args...) has been called with an invalid identifier.
 *
 * @author	Julian
 * @date	2018-01-22
 */

class FactoryInvalidId : FactoryException {
public:

	/**
	 * @fn	FactoryInvalidId::FactoryInvalidId()
	 *
	 * @brief	Default constructor
	 *
	 * @author	Jeffrey
	 * @date	1/31/2018
	 */

	FactoryInvalidId() : FactoryException("Given creator method Id is not registered") { }
};

/**
 * @class	BaseFactory
 *
 * @brief	A base factory.
 *
 * @author	Julian
 * @date	2018-01-22
 *
 * @tparam	V   	Type parameter for creating types of the factory.
 * @tparam	T   	Type parameter for the createMethod identifier.
 * @tparam	Args	Variable type parameters for the FactoryCreateMethod to require.
 */

template<class V, class T, class... Args>

/**
 * @class	BaseFactory
 *
 * @brief	A base factory.
 *
 * @author	Jeffrey
 * @date	1/31/2018
 */

class BaseFactory {
public:
	/** @brief	FactoryCreateMethod definition */
	using FactoryCreateMethod = std::function<V(Args...)>;

private:
	/** @brief	Map for registered FactoryCreateMethods with T as identifiers */
	std::map<T, FactoryCreateMethod> createMethods;

public:
	BaseFactory() {	}

	/**
	 * @fn	V BaseFactory::create(T id, Args... args)
	 *
	 * @brief	Creates a new V*
	 *
	 * @author	Julian
	 * @date	2018-01-22
	 *
	 * @exception	FactoryInvalidId	Thrown when the given id is doesn't have a registered create method
	 *  occurs.
	 *
	 * @param	id  	Identifier for resolving the right FactoryCreateMethod to use.
	 * @param	args	Arguments that the FactoryCreateMethod may require.
	 *
	 * @return	Pointer to newly created V
	 */

	V create(T id, Args... args) {
		auto it = createMethods.find(id);

		if (it != createMethods.end()) {
			return it->second(args...);
		} else {
			throw FactoryInvalidId();
		}
	}

	/**
	 * @fn	void BaseFactory::registerCreateMethod(T id, FactoryCreateMethod factoryCreateMethod)
	 *
	 * @brief	Registers a FactoryCreateMethod bound to an identifier
	 *
	 * @author	Julian
	 * @date	2018-01-22
	 *
	 * @param	id				   	The identifier.
	 * @param	factoryCreateMethod	The factory create method.
	 */

	void registerCreateMethod(T id, FactoryCreateMethod factoryCreateMethod) {
		createMethods.insert_or_assign(id, factoryCreateMethod);
	}
};