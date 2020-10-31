extends Control

onready var _setLabel = $Panel/Hbox/Label
onready var _menu = $Panel/Hbox/Vbox/MenuButton
onready var _inputPanel = $InputPanel
onready var _inputLabel = $InputPanel/NinePatchRect/Label

const _inText = "Press a key to set "
var _editKey : String

func _ready():
	_inputPanel.visible = false
	set_process_input(false)
	SceneManager.FadeIn("Fade", 2.0, 0)
	_setLabel.text = JSONBeautifier.beautify_json(JSON.print(GSettings.GetSettings()), 4)
	var _popup = _menu.get_popup()
	_menu.text = "Keymap: " + GSettings.GetCurrentKeymap()["name"]
	var _kmaps = GSettings.GetKeyMapNames()
	for i in _kmaps:
		_popup.add_item(i)
	_popup.connect("id_pressed", self, "_onMenu")
	SceneManager.connect("fade_out", self, "_onFadeOut")

func _onFadeOut(_cb : int):
	SceneManager.ChangeScene(load("res://scenes/MainMenu.tscn"))

func _onMenu(id):
	var _selected = _menu.get_popup().get_item_text(id)
	GSettings.SetKeymap(_selected)
	_menu.text = "Keymap: " + GSettings.GetCurrentKeymap()["name"]
	get_tree().call_group("SettingsButtons", "_onUpdate")

func _process(_delta):
	if !_inputPanel.visible:
		if Input.is_action_just_pressed("ui_cancel"):
			SceneManager.FadeOut("Fade", 2.0, 1)

func _onSL(opt : bool):
	if opt:
		GSettings.Save()
	else:
		GSettings.Load()
		_setLabel.text = JSONBeautifier.beautify_json(JSON.print(GSettings.GetSettings()), 4)
		get_tree().call_group("SettingsButtons", "_onUpdate")

func _input(event):
	if not event.is_pressed():
		return
	_updateKey(event.scancode)

func _updateKey(scan):
	set_process_input(false)
	_inputPanel.visible = false
	GSettings.SetKey(_editKey, scan)
	get_tree().call_group("SettingsButtons", "_onUpdate")

func _onButton(name : String):
	set_process_input(true)
	_inputLabel.text = _inText + name
	_inputPanel.visible = true
	_editKey = name.to_lower()
