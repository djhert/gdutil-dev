#ifndef _GDISPLAY_H_
#define _GDISPLAY_H_

#include <gdregistry/gdregistry.hpp>

#include <CanvasLayer.hpp>

namespace godot {

/* Godot CanvasLayer Object that deletes itself on _init() if _DEBUG is not defined */
class GDisplay : public CanvasLayer {
	GDCLASS(GDisplay, CanvasLayer);
};
} // namespace godot
#endif
