extends Label

const _prefix : String = "FPS: %d"

var _time : float = 0.0

func _process(delta : float) -> void:
	_time += delta
	if _time >= 1.0:
		_time = 0.0
		text = String(_prefix % Engine.get_frames_per_second())
