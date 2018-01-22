#pragma once

#include <map>
#include <functional>
#include <memory>

class FactoryException : public std::exception {
	std::string err;

protected:
	FactoryException(const std::string& err) : err(std::move(err)) { }

public:
	virtual const char* what() const override {
		return err.c_str();
	}
};

class FactoryInvalidId : FactoryException {
public:
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
class BaseFactory {
public:
	using FactoryCreateMethod = std::function<V*(Args...)>;

private:
	std::map<T, FactoryCreateMethod> createMethods;

public:
	BaseFactory() {	}

	/**
	 * @fn	V BaseFactory::create(T id, Args... args)
	 *
	 * @brief	Creates a new V
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
	 * @return	Newly created std::shared_ptr<V>
	 */

	V* create(T id, Args... args) {
		auto it = createMethods.find(id);

		if (it != createMethods.end()) {
			return it->second(args...);
		} else {
			throw FactoryInvalidId();
		}
	}

	void registerCreateMethod(T id, FactoryCreateMethod factoryCreateMethod) {
		createMethods.insert_or_assign(id, factoryCreateMethod);
	}
};