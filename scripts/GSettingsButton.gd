extends Button

onready var lbl = $Label

onready var toplvl = get_parent().get_parent().get_parent().get_parent().get_parent()

func _ready():
	_onUpdate()
	connect("pressed", toplvl, "_onButton", [name])

func _onUpdate():
	print(name)
	lbl.text = name + "\n" + OS.get_scancode_string(GSettings.GetKey(name.to_lower()))
