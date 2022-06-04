# 2D Liquid simulator with cellular automaton in Godot Engine (GDNative / C++) - NativeScript 1.1

**Demo:** https://www.youtube.com/watch?v=nF7cdUVgvNc

## prerequisites
> Godot 3.x executable
> 
> C++ compiler
> 
> SCons as a build tool

## clone repository

```
$ git clone --recurse-submodules https://github.com/tterrasson/liquid-simulator-godot
```

## Build godot-cpp binding
```
$ cd liquid-simulator-godot/godot-cpp
$ scons platform=<os> generate_bindings=yes
```

## Build plugin
```
$ cd ..
$ scons platform=<os>
```

*Replace `<os>` with linux, osx, windows or android.*

## Copy the plugin to your project
```
$ cp -r bin /path/to/yourproject/liquidsim
$ cp assets/* /path/to/yourproject/liquidsim/
```

**You must use the same folder name 'liquidsim'**

## Usage

### Add a new Node2D to your TileMap and call it `LiquidSim`

![alt text](doc/media/1.png?raw=true)

### Attach the native script to the LiquidSim node

![alt text](doc/media/2.png?raw=true)

![alt text](doc/media/3.png?raw=true)

### Attach a gdscript to the TileMap to test the plugin

Example:

```
extends TileMap

func _ready():
	pass # Replace with function body.

func _input(event):
	if event is InputEventMouseButton:
        # Mouse in viewport coordinates
		var wpos = world_to_map(get_global_mouse_position())
		$LiquidSim.add_liquid(wpos.x, wpos.y, 10.0)
```

For more details, see https://docs.godotengine.org/en/3.2/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module

## API

`$LiquidSim.start()` : Start the simulation (started by default)

`$LiquidSim.stop()` : Stop the simulation

`$LiquidSim.update_simulation()` : Force an update

`$LiquidSim.cleanup_empty_cells()` : Cleanup all empty cell (automatic)

`$LiquidSim.add_liquid(int x, int y, float amount)` : Add some liquid to the cell

`$LiquidSim.del_liquid(int x, int y)` : Empty a cell

`$LiquidSim.set_liquid(int x, int y, float amount)` : Set a fixed amount of liquid to the cell

`$LiquidSim.get_liquid(int x, int y) float` : Return the amount of liquid for the cell
