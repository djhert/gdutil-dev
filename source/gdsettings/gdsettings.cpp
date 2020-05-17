#include "gdsettings.hpp"

namespace godot {
GDREGISTER(GDSettings);

GDSettings *GDSettings::_instance = nullptr;

GDSettings *GDSettings::instance() {
	if (!_instance) {
		Godot::print_warning("No GDSettings exists.  GDSettings is probably not an AutoLoad",
				"GDSettings::instance", __FILE__, __LINE__);
	}
	return _instance;
}

GDSettings::~GDSettings() {
	_file->free();
	const Array &acSec = _activeSettings.keys();
	for (int i = 0; i < acSec.size(); i++) {
		_activeSettings[acSec[i]].operator godot::Dictionary().clear();
	}
	_activeSettings.clear();
	const Array &dfSec = _defaultSettings.keys();
	for (int i = 0; i < dfSec.size(); i++) {
		_defaultSettings[dfSec[i]].operator godot::Dictionary().clear();
	}
	_defaultSettings.clear();
}

void GDSettings::_register_methods() {
	register_method("_ready", &GDSettings::_ready);
	register_property<GDSettings, String>("Filename", &GDSettings::_fileName, "");
	register_property<GDSettings, Dictionary>("Default Settings", &GDSettings::_defaultSettings, Dictionary::make());

	register_method("Get", &GDSettings::_get);
	register_method("Set", &GDSettings::_set);
	register_method("Save", &GDSettings::_save);
	register_method("Filename", &GDSettings::_getFilename);
}

void GDSettings::_init() {
	_file = ConfigFile::_new();
	_defaultSettings = Dictionary::make();
}

void GDSettings::_ready() {
	Assert(!_fileName.empty(), "A filename is required for the configuration file.  See the \"GDSettings\" Node",
			"GDSettings::_ready", __FILE__, __LINE__);
	_fullPath = OS::get_singleton()->get_user_data_dir() + "/" + _fileName;

	_activeSettings = deep_copy<Dictionary>(_defaultSettings);
	_load();
	_save();

	Godot::print((Variant)_activeSettings);
	_instance = this;
}

Error GDSettings::_load() {
	Error err = _file->load(_fullPath);
	if (err != Error::OK) {
		Godot::print("!! Unable to load settings at: " + _fullPath);
		return err;
	}
	const Array &sections = _activeSettings.keys();
	for (int i = 0; i < sections.size(); i++) {
		const String &section = sections[i].operator godot::String();
		if (section.empty())
			continue;

		const Dictionary &dic = _activeSettings[section].operator godot::Dictionary();
		if (dic.empty())
			continue;

		const Array &keys = dic.keys();
		for (int j = 0; j < keys.size(); j++) {
			const String &key = keys[j].operator godot::String();
			if (key.empty())
				continue;
			const Variant &_t = _file->get_value(section, key, "gdnull");
			if (_t.operator godot::String() != "gdnull")
				_set(section, key, _t);
		}
	}
	return Error::OK;
}

void GDSettings::Save() {
}

void GDSettings::_save() {
	const Array &sections = _activeSettings.keys();
	for (int i = 0; i < sections.size(); i++) {
		const String &section = sections[i].operator godot::String();
		if (section.empty())
			continue;

		const Dictionary &dic = _activeSettings[section].operator godot::Dictionary();
		if (dic.empty())
			continue;

		const Array &keys = dic.keys();
		for (int j = 0; j < keys.size(); j++) {
			const String &key = keys[j].operator godot::String();
			if (key.empty())
				continue;
			const Variant &_t = dic[key];
			_file->set_value(section, key, _t);
		}
	}
	if (_file->save(_fullPath) != Error::OK)
		Godot::print("!! Unable to save settings at: " + _fullPath);
}

Variant GDSettings::Get(const String &section, const String &key, Variant dfault) {
	return dfault;
}

void GDSettings::Set(String section, String key, Variant value) {
}

Variant GDSettings::_get(String section, String key, Variant dfault) {
	if (_activeSettings.has(section)) {
		const Dictionary &_t = _activeSettings[section].operator godot::Dictionary();
		if (_t.has(key)) {
			return _t[key];
		}
	}
	return dfault;
}

void GDSettings::_set(String section, String key, Variant value) {
	if (_activeSettings.has(section)) {
		Dictionary _t = _activeSettings[section];
		if (_t.has(key)) {
			_t[key] = value;
			_activeSettings[section] = _t;
		}
	}
}

Variant GDSettings::_getFilename() {
	return _fileName;
}

} // namespace godot