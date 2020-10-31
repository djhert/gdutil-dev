#ifndef _GDUTIL_H_
#define _GDUTIL_H_

#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>

#include <Godot.hpp>
#include <Node.hpp>
#include <NodePath.hpp>

namespace godot {
/* Assert is missing from the main GDNative, this reimplements it how Godot does it.
If compile flag "_DEBUG" is not passed, abort will not be called but the error message
is printed by Godot.
*/
void Assert(const bool &test, const String &message,
		const String &function, const String &file,
		const int &line);

/* GetNodeFrom is a helper function to get a child node from the passed Node.
*/
template <class T>
static T *GetNodeFrom(const Node *from, const String &name) {
	const NodePath &path = NodePath(name);
	return Node::cast_to<T>(from->get_node_or_null(path));
}

/* deep_copy is a helper function to perform a deep copy on a Dictionary or Array
*/
template <class T>
static T deep_copy(const Variant &var) {
	Variant out;
	if (var.get_type() == Variant::DICTIONARY) {
		out = Dictionary::make();
		const Array &arc = var.operator godot::Dictionary().keys();
		for (int i = 0; i < arc.size(); i++) {
			const Variant &key = arc[i];
			const Variant &value = var.operator godot::Dictionary()[key];
			if (value.get_type() == Variant::DICTIONARY) {
				const Dictionary &temp = deep_copy<Dictionary>(value);
				out.operator godot::Dictionary()[key] = temp;
			} else {
				out.operator godot::Dictionary()[key] = value;
			}
		}
	} else if (var.get_type() == Variant::ARRAY) {
		out = Array::make();
		const Array &arc = var.operator godot::Array();
		for (int i = 0; i < arc.size(); i++) {
			const Variant &value = var.operator godot::Array()[i];
			if (value.get_type() == Variant::DICTIONARY) {
				const Dictionary &temp = deep_copy<Dictionary>(value);
				out.operator godot::Array().append(temp);
			} else if (value.get_type() == Variant::ARRAY) {
				const Array &temp = deep_copy<Array>(value);
				out.operator godot::Array().append(temp);
			} else {
				out.operator godot::Array().append(value);
			}
		}
	} else {
		out = var;
	}
	return (T)out;
}

/* clamp is missing from GDNative, reimplements.
*/
real_t clamp(const real_t &n, const real_t &l, const real_t &h);
/* deg2rad is missing from GDNative, reimplements.
*/
real_t deg2rad(const real_t &v);

void Reparent(Node *node, Node *newparent);

/* ZERO2 is a const at 0
UP2 is a const pointing up
RIGHT2 is a const pointing right
*/
const Vector2 ZERO2 = Vector2(0, 0);
const Vector2 UP2 = Vector2(0, 1);
const Vector2 RIGHT2 = Vector2(1, 0);

/* ZERO3 is a const at 0
RIGHT3 is a const pointing right
UP3 is a const pointing up
FORWARD3 is a const pointing right
*/
const Vector3 ZERO3 = Vector3(0, 0, 0);
const Vector3 RIGHT3 = Vector3(1, 0, 0);
const Vector3 UP3 = Vector3(0, 1, 0);
const Vector3 FORWARD3 = Vector3(0, 0, 1);

} // namespace godot

#endif
