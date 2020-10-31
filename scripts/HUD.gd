extends Control

func _ready():
	visible = false
	SceneManager.connect("fade_in", self, "_onFadeIn")
	SceneManager.connect("fade_out", self, "_onFadeOut")
	

func _onFadeIn(num : int) -> void:
	get_parent().get_parent().connect("focus", self, "_onFocus")

func _onFocus(f : bool) -> void:
	if f:
		visible = true
	else:
		visible = false

func _onFadeOut(num : int) -> void:
	if num == 0:
		SceneManager.Quit()
	else:
		var menu = load("res://scenes/MainMenu.tscn")
		SceneManager.ChangeScene(menu)

func _onButton(num : int) -> void:
	SceneManager.FadeOut("Fade", 2.0, num)
