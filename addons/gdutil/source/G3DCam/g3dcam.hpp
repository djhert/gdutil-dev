#ifndef __GD3DCAM_HPP__
#define __GD3DCAM_HPP__

#include <Camera.hpp>
#include <Godot.hpp>

#include <gdregistry/gdregistry.hpp>
#include <gdutil.hpp>

#include "g3dbody.hpp"

namespace godot {
class G3DBody;
class G3DCam : public Camera {
	GDCLASS(G3DCam, Camera)

public:
	real_t getSpeed() { return speed; };
	real_t getMouseSensitivity() { return mouse_sensitivity; };
	bool getCollide() { return collide; };

	String getForward() { return _forward; };
	String getBack() { return _back; };
	String getLeft() { return _left; };
	String getRight() { return _right; };
	String getUp() { return _up; };
	String getDown() { return _down; };
	String getCollideToggle() { return _collide; };

protected:
	real_t speed;
	real_t mouse_sensitivity;
	bool collide;

	// controls
	String _forward;
	String _back;
	String _left;
	String _right;
	String _up;
	String _down;
	String _collide;

	void _ready();

	G3DBody *_body;
};
} // namespace godot

#endif