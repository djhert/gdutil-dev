#include "gdisplay.hpp"

namespace godot {

GDREGISTER(GDisplay)

GDisplay::~GDisplay() {}

void GDisplay::_register_methods() {}

void GDisplay::_init() {
	Godot::print("Hello from GDisplay!");
#ifndef _DEBUG
	queue_free();
#endif
}

} // namespace godot