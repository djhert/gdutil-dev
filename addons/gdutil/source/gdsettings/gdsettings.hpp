#ifndef _GDSETTINGS_H_
#define _GDSETTINGS_H_

#include <ConfigFile.hpp>
#include <Godot.hpp>
#include <Node.hpp>
#include <OS.hpp>

#include <iostream>

#include <gdregistry/gdregistry.hpp>
#include <gdutil/gdutil.hpp>

namespace godot {
class GDSettings : public Node {
	GDCLASS(GDSettings, Node);

public:
	void _ready();

	static GDSettings *instance();
	static void Save();
	static Variant Get(const String &section, const String &key, Variant dfault = Variant());
	static void Set(String section, String key, Variant value);

protected:
	ConfigFile *_file;

	String _fileName;
	String _fullPath;

	Dictionary _defaultSettings;
	Dictionary _activeSettings;

	int _test;

	Error _load();
	void _save();

	Variant _get(String section, String key, Variant dfault = Variant());
	void _set(String section, String key, Variant value);

	Variant _getFilename();

private:
	static GDSettings *_instance;
};
} // namespace godot

#endif