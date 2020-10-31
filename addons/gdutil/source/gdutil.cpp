#include "gdutil.hpp"

namespace godot {
void Assert(const bool &test, const String &message,
		const String &function, const String &file,
		const int &line) {
	if (test) {
		return;
	}
	Godot::print_error(message, function, file, line);
#ifdef _DEBUG
	std::abort();
#endif
}

real_t clamp(const real_t &n, const real_t &l, const real_t &h) {
	return std::max(l, std::min(n, h));
}

real_t deg2rad(const real_t &v) {
	return real_t((v * M_PI) / 180.0f);
}

void Reparent(Node *node, Node *newparent) {
	Node *old = node->get_parent();
	old->remove_child(node);
	newparent->add_child(node);
	node->set_owner(newparent);
	old = nullptr;
	delete old;
}
} // namespace godot