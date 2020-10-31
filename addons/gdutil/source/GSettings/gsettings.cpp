#include "gsettings.hpp"

namespace godot {
GDREGISTER(GSettings);

GSettings *GSettings::_instance = nullptr;

GSettings *GSettings::instance() {
	if (!_instance) {
		Godot::print_warning("No GSettings exists.  GSettings is probably not an AutoLoad",
				"GSettings::instance", __FILE__, __LINE__);
	}
	return _instance;
}

GSettings::~GSettings() {
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
	const Array &keySec = _keymaps.keys();
	for (int i = 0; i < keySec.size(); i++) {
		_keymaps[keySec[i]].operator godot::Dictionary().clear();
	}
	_keymaps.clear();
}

void GSettings::_register_methods() {
	register_method("_ready", &GSettings::_ready);
	register_property<GSettings, String>("Filename", &GSettings::_fileName, "");

	register_method("Get", &GSettings::get);
	register_method("Set", &GSettings::set);
	register_method("Save", &GSettings::save);
	register_method("Load", &GSettings::load);
	register_method("Filename", &GSettings::getFilename);
	register_method("SetDefaultSettings", &GSettings::setDefault);
	register_method("GetSettings", &GSettings::getSettings);
	register_method("SetDefaultKeymap", &GSettings::setDefaultKeymap);
	register_method("SetKeymap", &GSettings::setKeymap);
	register_method("SetKey", &GSettings::setKey);
	register_method("GetKey", &GSettings::getKey);
	register_method("GetKeyMapNames", &GSettings::getKeyMapNames);
	register_method("GetKeymap", &GSettings::getKeymap);
	register_method("GetCurrentKeymap", &GSettings::getCurrentKeymap);
}

void GSettings::_init() {
	_file = ConfigFile::_new();
	_defaultSettings = Dictionary::make();
	_keymaps = Dictionary::make();
	_fileName = "";
}

void GSettings::_ready() {
	Assert(!_fileName.empty(), "A filename is required for the configuration file.  See the \"GSettings\" Node",
			"GSettings::_ready", __FILE__, __LINE__);
	_instance = this;
	_fullPath = OS::get_singleton()->get_user_data_dir() + "/" + _fileName;
}

void GSettings::setDefault(Dictionary dict) {
	_defaultSettings = deep_copy<Dictionary>(dict);
	_activeSettings = deep_copy<Dictionary>(_defaultSettings);
	load();
	save();
}

void GSettings::setDefaultKeymap(Dictionary dict) {
	if (dict.keys().size() == 0)
		return;
	_keymaps = deep_copy<Dictionary>(dict);
	if (!_activeSettings.has(_sKey))
		setKeymap(_keymaps.keys()[0]);
}

void GSettings::setKeymap(String name) {
	if (_activeSettings.has(_sKey))
		_activeSettings[_sKey].operator godot::Dictionary().clear();
	if (_keymaps.has(name)) {
		_activeSettings[_sKey] = deep_copy<Dictionary>(_keymaps[name]);
		_activeSettings[_sKey].operator godot::Dictionary()["name"] = name;
	}
	const Array &actions = _activeSettings[_sKey].operator godot::Dictionary().keys();
	for (int i = 0; i < actions.size(); i++) {
		if (actions[i] == String("name"))
			continue;
		_setInput(actions[i], _activeSettings[_sKey].operator godot::Dictionary()[actions[i]]);
	}
}

void GSettings::_setInput(String action, int key) {
	if (InputMap::get_singleton()->has_action(action))
		InputMap::get_singleton()->action_erase_events(action);
	else
		InputMap::get_singleton()->add_action(action);

	InputEventKey *e = InputEventKey::_new();
	e->set_scancode(key);
	InputMap::get_singleton()->action_add_event(action, e);
}

void GSettings::setKey(String name, Variant value) {
	set(_sKey, name, value);
	_setInput(name, value.operator signed int());
}

Variant GSettings::getKey(String name) const {
	return get(_sKey, name);
}

Dictionary GSettings::getKeymap(String name) const {
	return _keymaps[name].operator godot::Dictionary();
}

Dictionary GSettings::getCurrentKeymap() const {
	return _activeSettings[_sKey].operator godot::Dictionary();
}

bool GSettings::load() {
	Error err = _file->load(_fullPath);
	if (err != Error::OK) {
		Godot::print("!! Unable to load settings at: " + _fullPath);
		return false;
	}
	const Array &sections = _file->get_sections();
	for (int i = 0; i < sections.size(); i++) {
		const String &section = sections[i].operator godot::String();
		if (section.empty())
			continue;

		const Array &keys = _file->get_section_keys(section);
		for (int j = 0; j < keys.size(); j++) {
			const String &key = keys[j].operator godot::String();
			if (key.empty())
				continue;
			const Variant &_t = _file->get_value(section, key, "gdnull");
			if (_t.operator godot::String() != "gdnull")
				set(section, key, _t);
		}
	}

	return true;
}

void GSettings::save() {
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

Variant GSettings::get(String section, String key, Variant dfault) const {
	if (_activeSettings.has(section)) {
		const Dictionary &_t = _activeSettings[section].operator godot::Dictionary();
		if (_t.has(key)) {
			return _t[key];
		}
	}
	return dfault;
}

void GSettings::set(String section, String key, Variant value) {
	if (!_activeSettings.has(section))
		_activeSettings[section] = Dictionary::make();

	_activeSettings[section].operator godot::Dictionary()[key] = value;
}

Variant GSettings::getFilename() const {
	return _fileName;
}

Dictionary GSettings::getSettings() const {
	return _activeSettings;
}

Array GSettings::getKeyMapNames() const {
	return _keymaps.keys();
}

} // namespace godot