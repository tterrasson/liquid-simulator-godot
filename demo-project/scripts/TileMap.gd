extends TileMap


# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	$LiquidSim.set_liquid(15, -1, 5.0)


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _input(event):
	# Mouse in viewport coordinates
	if event is InputEventMouseButton:
		var wpos = world_to_map(get_global_mouse_position())		
		$LiquidSim.add_liquid(wpos.x, wpos.y, 5.0)
