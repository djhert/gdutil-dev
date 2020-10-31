extends Node

var _keymaps = {
	"default": {
		"forward": KEY_UP,
		"back": KEY_DOWN,
		"left": KEY_LEFT,
		"right": KEY_RIGHT,
		"up": KEY_PAGEUP,
		"down": KEY_PAGEDOWN,
		"collision": KEY_ENTER,
		"escape": KEY_ESCAPE
	},
	"alternate": {
		"forward": KEY_W,
		"back": KEY_S,
		"left": KEY_A,
		"right": KEY_D,
		"up": KEY_SPACE,
		"down": KEY_SHIFT,
		"collision": KEY_E,
		"escape": KEY_ESCAPE
	}
}

# Settings variable to hold all of the configurable settings
var _settings := {
		"display": {
			"window_width": 1920,
			"window_height": 1080
		},
		"music": {
			"volume": 25,
			"enabled": true
		}
}

func _ready():
	get_parent().call_deferred("SetDefaultSettings", _settings)
	get_parent().call_deferred("SetDefaultKeymap", _keymaps)
