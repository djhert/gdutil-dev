#include "g3dbody.hpp"

using namespace godot;

GDREGISTER(G3DBody)

G3DBody::~G3DBody() {
}

void G3DBody::_register_methods() {
	register_method("_ready", &G3DBody::_ready);
	register_method("_process", &G3DBody::_process);
	register_method("_input", &G3DBody::_input);
	register_method("_physics_process", &G3DBody::_physics_process);

	register_signal<G3DBody>("focus", "focused", GODOT_VARIANT_TYPE_BOOL);
}

void G3DBody::_init() {
}

void G3DBody::_ready() {
	set_name("g3dbody");
	_escaped = false;

	Reparent(_camera, this);
	_camera->set_translation(1.5f * UP3);

	_shape = CollisionShape::_new();
	_cap = CapsuleShape::_new();
	_shape->set_name("g3dcollide");
	add_child(_shape);
	_shape->set_shape(_cap);
	_shape->set_rotation_degrees(Vector3(90, 0, 0));
	_shape->set_translation(UP3);
	_shape->set_disabled(!_camera->getCollide());

	Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
}

void G3DBody::_setup(G3DCam *cam) {
	_camera = cam;
	set_global_transform(_camera->get_global_transform());
}

void G3DBody::_process(real_t delta) {
	if (Input::get_singleton()->is_action_just_pressed("ui_cancel")) {
		_escaped = !_escaped;
		if (_escaped)
			Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_VISIBLE);
		else
			Input::get_singleton()->set_mouse_mode(Input::MOUSE_MODE_CAPTURED);
		emit_signal("focus", _escaped);
	}
	if (Input::get_singleton()->is_action_just_pressed(_camera->getCollideToggle()))
		_shape->set_disabled(!_shape->is_disabled());

	Vector3 temp = _camera->get_rotation_degrees();
	temp.x -= _mouseDelta.y * _camera->getMouseSensitivity() * delta;
	temp.x = clamp(temp.x, -90.0f, 90.0f);
	_camera->set_rotation_degrees(temp);

	temp = get_rotation_degrees();
	temp.y -= _mouseDelta.x * _camera->getMouseSensitivity() * delta;

	set_rotation_degrees(temp);

	_mouseDelta = ZERO2;
}

void G3DBody::_physics_process(double delta) {
	// Clean out _velocity
	_velocity = ZERO3;
	_moveDir = ZERO2;
	if (_escaped)
		return;

	if (Input::get_singleton()->is_action_pressed(_camera->getForward()))
		_moveDir.y -= 1;
	if (Input::get_singleton()->is_action_pressed(_camera->getBack()))
		_moveDir.y += 1;

	if (Input::get_singleton()->is_action_pressed(_camera->getLeft()))
		_moveDir.x -= 1;
	if (Input::get_singleton()->is_action_pressed(_camera->getRight()))
		_moveDir.x += 1;

	_moveDir.normalize();

	Vector3 forward = get_global_transform().basis.z;
	Vector3 right = get_global_transform().basis.x;

	Vector3 relativeDir = ((forward * _moveDir.y) + (right * _moveDir.x));

	_velocity.x = relativeDir.x * _camera->getSpeed();
	_velocity.z = relativeDir.z * _camera->getSpeed();

	if (Input::get_singleton()->is_action_pressed(_camera->getUp()))
		_velocity.y += _camera->getSpeed();
	if (Input::get_singleton()->is_action_pressed(_camera->getDown()))
		_velocity.y -= _camera->getSpeed();

	_velocity = move_and_slide(_velocity, UP3);
}

void G3DBody::_input(Variant event) {
	InputEvent *e = InputEvent::cast_to<InputEvent>(event);
	if (e->is_class("InputEventMouseMotion")) {
		InputEventMouseMotion *mouse = InputEvent::cast_to<InputEventMouseMotion>(event);
		if (!_escaped)
			_mouseDelta = mouse->get_relative();
	}
}
