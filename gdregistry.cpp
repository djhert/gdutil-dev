#include "gdregistry.hpp"

namespace godot {

// Create an empty instance on init
GDRegistry *GDRegistry::_instance = nullptr;

// Destructor that clears the std::vector
GDRegistry::~GDRegistry() {
	_gens.clear();
}

// instance() will return the static instance of the Registry
GDRegistry *GDRegistry::instance() {
	if (!_instance)
		_instance = new GDRegistry;

	return _instance;
}

// Run will run all of the functions in the Registry
void GDRegistry::Run() {
	// Make sure that an instance exists
	instance();
	for (auto &&x : _instance->_gens) {
		// Call each register_class(TYPE)
		x();
	}
}

} // namespace godot