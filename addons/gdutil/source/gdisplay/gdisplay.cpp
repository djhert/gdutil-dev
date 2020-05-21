#include "gdisplay.hpp"

namespace godot {

GDREGISTER(GDisplay)

GDisplay::~GDisplay() {}

void GDisplay::_register_methods() {}

void GDisplay::_init() {
#ifndef _DEBUG
	queue_free();
#endif
}

} // namespace godot