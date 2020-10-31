extends Control

onready var _3dscene = preload("res://scenes/3DWorld.tscn")
onready var _2dscene = preload("res://scenes/GDExample.tscn")
onready var _setScene = preload("res://scenes/GSettingsScene.tscn")

func _ready():
	SceneManager.FadeIn("Fade", 2.0, 0)
	SceneManager.connect("fade_out", self, "_onFadeOut")

func _onFadeOut(num : int) -> void:
	if num == 0:
		SceneManager.Quit()
	elif num == 1:
		SceneManager.ChangeScene(_3dscene)
	elif num == 2:
		SceneManager.ChangeScene(_setScene)
	else:
		SceneManager.ChangeScene(_2dscene)

func _onButton(num : int) -> void:
	SceneManager.FadeOut("Fade", 2.0, num)
