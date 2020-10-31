extends Node

func _ready():
	SceneManager.FadeIn("Fade", 2.0, 256)
	SceneManager.connect("fade_in", self, "_onFadeIn")
	SceneManager.connect("fade_out", self, "_onFadeOut")

func _onFadeIn(var cb : int):
	print("Callback: %d " % cb )

func _onFadeOut(_cb : int):
	var scn = load("res://scenes/MainMenu.tscn")
	SceneManager.ChangeScene(scn)

func _process(_delta):
	if Input.is_action_just_pressed("ui_cancel"):
		SceneManager.FadeOut("Fade", 2.0, 1)
