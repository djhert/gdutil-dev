#ifndef _GSCENEMANAGER_H_
#define _GSCENEMANAGER_H_

#include <gdregistry/gdregistry.hpp>

#include <gdutil.hpp>

#include <AnimationPlayer.hpp>
#include <CanvasLayer.hpp>
#include <Control.hpp>
#include <MainLoop.hpp>
#include <PackedScene.hpp>
#include <SceneTree.hpp>

namespace godot {

class GSceneManager : public CanvasLayer {
	GDCLASS(GSceneManager, CanvasLayer);

	// Class
public:
	template <class T>
	static T *GetNode(const String &name) {
		return GetNodeFrom<T>(instance()->_activeScene, name);
	}

	void _ready();
	void _lateReady();

	// Statics
	static GSceneManager *instance();
	static void Quit();
	static void FadeIn(const String &name, const real_t &speed, const int &callback = 0);
	static void FadeOut(const String &name, const real_t &speed, const int &callback = 0);
	static void ChangeScene(Resource *res);

protected:
	Node *_root;
	Node *_activeScene;

	Control *_fadeParent;
	AnimationPlayer *_fadeAnim;
	Control *_loadParent;
	AnimationPlayer *_loadAnim;

	String _loadPath;
	String _fadePath;

	/* _state is what operation is occurring
	_callback holds the value to send with finish signal */
	enum State {
		Idle,
		Fade_In,
		Fade_Out,
		Load_Start,
		Load_End
	};

	State _state;
	int _callback;

	bool _setupChildren();
	void _quit();
	void _actualQuit();
	void _fade(const String &name, const real_t &speed, const bool &back = false);
	void _fadeIn(String name, real_t speed, int callback);
	void _fadeOut(String name, real_t speed, int callback);
	void _fadeDone(String name);
	void _notification(int what);

	void _changeScene(Resource *res);
	void _change(const PackedScene *res);

private:
	static GSceneManager *_instance;
};

} // namespace godot
#endif