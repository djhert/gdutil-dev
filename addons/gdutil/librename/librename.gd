extends Control

# Set the Variable
export var _newLibPath : String

const _prefix = "res://addons/gdutil/nativescript/"

var _scripts = [
	_prefix + "GDisplay.gdns",
	_prefix + "SceneManager.gdns"
]

onready var _wid = $Box/VBox/WhatIDo
onready var _usg = $Box/VBox/Using
onready var _exp = $Box/VBox/Example

func _ready():
	if _newLibPath.empty():
		print("New Lib Path is empty!  Check the Control node and please set a path to your '.gdnlib' file")
		assert(false)

	var directory = Directory.new();
	if !directory.file_exists(_newLibPath):
		print("The file '%s' doesn't exist!  Check the Control node and please set a path to your '.gdnlib' file" % _newLibPath)
		assert(false)

	for i in _scripts:
		_loadFile(i)
	
	_wid.text = "All completed!  Updated to: " + _newLibPath
	_usg.text = "You can now close the window"
	_exp.text = "Also, did you know you should delete the 'librename' folder in this addon directory?  Don't Forget!!"

func _loadFile(path):
	var file = File.new()
	var pos : int
	file.open(path, file.READ_WRITE)
	while !file.eof_reached():
		pos = file.get_position()
		var _s = file.get_line()
		if _s.match("[ext_resource*"):
			file.seek(pos)
			file.store_line("[ext_resource path=\"res://" + _newLibPath + "\" type=\"GDNativeLibrary\" id=1]")
			print("Updated: " + path)
			break
	file.close()
