#include "gscenemanager.hpp"

namespace godot {

GDREGISTER(GSceneManager)

GSceneManager *GSceneManager::_instance = nullptr;

GSceneManager::~GSceneManager() {}

void GSceneManager::_register_methods() {
	// Properties
	register_property<GSceneManager, String>("Load Animator Path", &GSceneManager::_loadPath, "");
	register_property<GSceneManager, String>("Fade Animator Path", &GSceneManager::_fadePath, "");

	// GDScript
	register_method("FadeIn", &GSceneManager::_fadeIn);
	register_method("FadeOut", &GSceneManager::_fadeOut);
	register_method("Quit", &GSceneManager::_quit);
	register_method("ChangeScene", &GSceneManager::_changeScene);

	// Internal
	register_method("_ready", &GSceneManager::_ready);
	register_method("_actualQuit", &GSceneManager::_actualQuit);
	register_method("_lateReady", &GSceneManager::_lateReady);
	register_method("_fadeDone", &GSceneManager::_fadeDone);
	register_method("_change", &GSceneManager::_change);
	register_method("_notification", &GSceneManager::_notification);

	// Signals
	register_signal<GSceneManager>("fade_in", "callback", GODOT_VARIANT_TYPE_INT);
	register_signal<GSceneManager>("fade_out", "callback", GODOT_VARIANT_TYPE_INT);
	register_signal<GSceneManager>("load_start", "callback", GODOT_VARIANT_TYPE_INT);
	register_signal<GSceneManager>("load_end", "callback", GODOT_VARIANT_TYPE_INT);
	register_signal<GSceneManager>("quit");
}

GSceneManager *GSceneManager::instance() {
	if (!_instance) {
		Godot::print_warning("No GSceneManager exists.  GSceneManager is probably not an AutoLoad",
				"GSceneManager::instance", __FILE__, __LINE__);
	}
	return _instance;
}

void GSceneManager::Quit() {
	instance()->_quit();
}

void GSceneManager::FadeIn(const String &name, const real_t &speed, const int &callback) {
	instance()->_fadeIn(name, -speed, callback);
}

void GSceneManager::FadeOut(const String &name, const real_t &speed, const int &callback) {
	instance()->_fadeOut(name, speed, callback);
}

void GSceneManager::ChangeScene(Resource *res) {
	instance()->_changeScene(res);
}

void GSceneManager::_init() {
	_instance = this;
	_root = nullptr;
	_activeScene = nullptr;
	_fadeAnim = nullptr;
	_loadAnim = nullptr;
	_loadPath = "";
	_fadePath = "";
	_state = Idle;
}

void GSceneManager::_ready() {
	_root = get_node("/root");
	Assert(_root != nullptr, "Cannot get Root Scene.  GSceneManager is probably not an AutoLoad",
			"GSceneManager::ready", __FILE__, __LINE__);
	Assert(_setupChildren(), "Missing Load or Fade", "GSceneManager::ready",
			__FILE__, __LINE__);
	call_deferred("_lateReady");
}

// called at the end of the "ready" cycle
void GSceneManager::_lateReady() {
	// The last scene on root will always be the "startup" scene
	_activeScene = _root->get_child(_root->get_child_count() - 1);
	Assert(_activeScene != nullptr, "Cannot get Active Scene.   GSceneManager is probably not an AutoLoad",
			"GSceneManager::ready", __FILE__, __LINE__);
	get_tree()->set_auto_accept_quit(false);
}

bool GSceneManager::_setupChildren() {
	if (_fadePath != "") {
		_fadeAnim = GetNodeFrom<AnimationPlayer>(this, _fadePath);
		// check to see if needed animation player is found
		if (!_fadeAnim)
			return false;
		_fadeParent = GetNodeFrom<Control>(this, _fadePath.get_base_dir());
		// check to see if parent is found
		if (!_fadeParent)
			return false;
		_fadeParent->set_visible(false);
		_fadeAnim->connect("animation_finished", this, "_fadeDone");
	}
	if (_loadPath != "") {
		_loadAnim = GetNodeFrom<AnimationPlayer>(this, _loadPath);
		// check to see if needed animation player is found
		if (!_loadAnim)
			return false;

		_loadParent = GetNodeFrom<Control>(this, _loadPath.get_base_dir());
		// check to see if parent is found
		if (!_loadParent)
			return false;
		_loadParent->set_visible(false);
	}
	return true;
}

void GSceneManager::_quit() {
	emit_signal("quit");
	call_deferred("_actualQuit");
}

void GSceneManager::_actualQuit() {
	_root->get_tree()->quit();
}

void GSceneManager::_fadeIn(String name, real_t speed, int callback) {
	_callback = callback;
	_state = Fade_In;
	_fade(name, -speed, true);
}

void GSceneManager::_fadeOut(String name, real_t speed, int callback) {
	_callback = callback;
	_state = Fade_Out;
	_fade(name, speed, false);
}

void GSceneManager::_fade(const String &name, const real_t &speed, const bool &back) {
	if (!_fadeAnim) {
		Godot::print_error("Fade called but object doesn't exist.", "GSceneManager::_fade",
				__FILE__, __LINE__);
		_state = Idle;
		_callback = 0;
		return;
	}
	_fadeParent->set_visible(true);
	_fadeAnim->play(name, -1.0f, speed, back);
}

void GSceneManager::_fadeDone(String name) {
	switch (_state) {
		case Fade_In:
			_state = Idle;
			_fadeParent->set_visible(false);
			emit_signal("fade_in", _callback);
			break;
		case Fade_Out:
			_state = Idle;
			emit_signal("fade_out", _callback);
			break;
		case Load_Start:
			_state = Idle;
			emit_signal("load_start", _callback);
			break;
		case Load_End:
			_state = Idle;
			_loadParent->set_visible(false);
			emit_signal("load_end", _callback);
			break;
		default:
			break;
	}
}

void GSceneManager::_notification(int what) {
	if (what == MainLoop::NOTIFICATION_WM_QUIT_REQUEST)
		_quit();
}

void GSceneManager::_changeScene(Resource *res) {
	call_deferred("_change", Resource::cast_to<PackedScene>(res));
}

void GSceneManager::_change(const PackedScene *res) {
	if (!res) {
		Godot::print_error("Node was not a PackedScene", "GSceneManager::_change",
				__FILE__, __LINE__);
		return;
	}
	Node *temp = res->instance();
	if (!temp) {
		Godot::print_error("Unable to instance node from: " + res->get_name(),
				"GSceneManager::_change", __FILE__, __LINE__);
		return;
	}
	Node *cur = _activeScene;
	_root->remove_child(cur);
	cur->queue_free();
	_root->add_child(temp);
	_activeScene = temp;

	// overkill cleanup, just in case
	temp = NULL;
	delete temp;
	cur = NULL;
	delete cur;
}

} // namespace godot