#ifndef __GD3DBODY_HPP__
#define __GD3DBODY_HPP__

#include <Camera.hpp>
#include <CapsuleShape.hpp>
#include <CollisionShape.hpp>
#include <Godot.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouseMotion.hpp>
#include <KinematicBody.hpp>
#include <NodePath.hpp>
#include <SceneTree.hpp>
#include <Spatial.hpp>
#include <Viewport.hpp>

#include <gdregistry/gdregistry.hpp>
#include <gdutil.hpp>

#include "g3dcam.hpp"

namespace godot {
class G3DCam;
class G3DBody : public KinematicBody {
	GDCLASS(G3DBody, KinematicBody)

public:
	void _ready();
	void _process(real_t delta);
	void _input(Variant);
	void _physics_process(double);

	void _setup(G3DCam *cam);

protected:
	CollisionShape *_shape;
	CapsuleShape *_cap;
	G3DCam *_camera;

	Vector3 _velocity;
	Vector2 _moveDir;
	Vector2 _mouseDelta;

	bool _escaped;
};
} // namespace godot

#endif