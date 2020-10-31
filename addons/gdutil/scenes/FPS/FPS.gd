extends Label

var _time : float = 0.0

func _process(delta : float) -> void:
	_time += delta
	if _time >= 1.0:
		_time = 0.0
		text = "FPS: " + String(Engine.get_frames_per_second())
