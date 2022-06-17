
tool
extends EditorPlugin

func _enter_tree():
	add_custom_type("LiquidSim", "Node", preload("res://addons/liquidsim/liquidsim.gdns"), preload("res://addons/liquidsim/assets/icon.png"))

func _exit_tree():
	remove_custom_type("LiquidSim")
