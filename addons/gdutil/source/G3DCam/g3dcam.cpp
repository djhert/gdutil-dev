#include "g3dcam.hpp"

namespace godot {

GDREGISTER(G3DCam)

G3DCam::~G3DCam() {}

void G3DCam::_register_methods() {
	register_property<G3DCam, real_t>("Movement/Speed", &G3DCam::speed, 10.0);
	register_property<G3DCam, real_t>("Movement/Mouse Sensitivity", &G3DCam::mouse_sensitivity, 10.0f);
	register_property<G3DCam, bool>("Movement/Collision", &G3DCam::collide, true);
	register_property<G3DCam, String>("Conrol/Forward", &G3DCam::_forward, "ui_up");
	register_property<G3DCam, String>("Conrol/Back", &G3DCam::_back, "ui_down");
	register_property<G3DCam, String>("Conrol/Left", &G3DCam::_left, "ui_left");
	register_property<G3DCam, String>("Conrol/Right", &G3DCam::_right, "ui_right");
	register_property<G3DCam, String>("Conrol/Up", &G3DCam::_up, "ui_page_up");
	register_property<G3DCam, String>("Conrol/Down", &G3DCam::_down, "ui_page_down");
	register_property<G3DCam, String>("Conrol/Toggle Collision", &G3DCam::_collide, "ui_accept");

	register_method("_ready", &G3DCam::_ready);
}

void G3DCam::_init() {
	speed = 10.0f;
	mouse_sensitivity = 10.0f;
	collide = true;

	_forward = "ui_up";
	_back = "ui_down";
	_left = "ui_left";
	_right = "ui_right";
	_up = "ui_page_up";
	_down = "ui_page_down";
	_collide = "ui_accept";
}

void G3DCam::_ready() {
	_body = G3DBody::_new();
	_body->_setup(this);
	get_parent()->call_deferred("add_child", _body);
}

} // namespace godot