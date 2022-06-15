---
sidebar: auto
---

<iframe width="100%" height="400" src="https://www.youtube.com/embed/nF7cdUVgvNc" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

# Guide

This guide will teach you how to compile and add the plugin to a new **Godot engine** project .

::: tip
Try a demo of the plugin [here](../game/)
:::

## Prerequisites.

For the development of this guide, we must have the following installed in our system:

::: tip

- Godot engine 3.x
- Compilador C++
- Scones build tool
- Emscripten 2.0.17 build tool (In case of building for web **"wasm"**).

:::

## Clone Repository.

::: warning
It is likely that you will find differences between the original repository and this repository, if you are going to use the original **"tterrasson"** repository , I recommend that you follow the instructions you will find there, since there may be differences in how the plugin is installed.
:::

Original Repository.

```sh
$ git clone --recurse-submodules https://github.com/tterrasson/liquid-simulator-godot
```

This repository.

```sh
$ git clone --recurse-submodules https://github.com/AJ-Wi/liquid-simulator-godot
```

## Compile binary.

In this section we will see how to compile both the godot-cpp binary and our plugin.

::: tip
In the following commands we must replace **"\<os\>"** with the platform on which we want to run our game:

- windows
- linux
- osx
- javascript
- andorid
  :::

### godot-cpp binary.

We need to compile the godot-cpp binary according to the platform on which we want to run our game, for this we must be positioned inside the godot-cpp folder in our terminal.

```sh
$ cd liquid-simulator-godot/godot-cpp
$ scons platform=<os> generate_bindings=yes
```

### plugin binary.

Once the godot-cpp binary is compiled, we return to the root folder of the project and compile the plugin.

```sh
$ cd ..
$ scons platform=<os>
```

## Copy the plugin to your project.

At this point you will find the compiled plugin in the **"bin"** folder of the project, you just need to copy this folder to the **"addons"** folder of your project, and rename the plugin folder as **"liquidsim"** , you can also execute the following command.

```sh
$ cp -r bin/* /ruta/a/tuproyecto/addons/liquidsim
```

You should be left with the following file structure in your project.

```
.
tuproyecto
    ├── addons
    |    └── liquidsim
    |        ├── assets
    |        |   ├── icon.png
    |        |   ├── lava.png
    |        |   └── water.png
    |        ├── lib
    |        |   └── `libliquidsim.so` (according to the one you have compiled.)
    |        ├── LiquidSim.tscn
    |        ├── liquidsim_plugin.gd
    |        ├── liquidsim.gdnlib
    |        ├── liquuidsim.gdns
    |        └── plugin.cfg
    ├── default_env.tres
    ├── icon.png
    ├── project.godot
```

## Activate the plugin.

To do this, you access the configuration of your project and **"LiquidSim"** will appear in the plugin section, you just have to activate it.

![alt img](~@assets/activate_plugin.png)

## Add Node.

Once activated, the plugin already appears in the list of nodes in the editor.

![alt img](~@assets/add_plugin_to_tilemap.png)

For our new node or plugin to work, we must add it as a child node of a **"TileMap"**.

![alt img](~@assets/added_liquidsim_node.png)

## Plugin setting.

Adjust the size of the plugin cells according to the cell size of your **"TileMap"**.

![alt img](~@assets/adjust_cell_size.png)

## Try the plugin.

Attach a gdscript to the **"TileMap"** to test the plugin, you can use the following sample code.

```py
extends TileMap

func _ready():
	pass # Replace with function body.

func _input(event):
	if event is InputEventMouseButton:
        # Mouse in viewport coordinates
		var wpos = world_to_map(get_global_mouse_position())
		$LiquidSim.add_liquid(wpos.x, wpos.y, 10.0)
```

## API.

```py
$LiquidSim.start() : Starts the simulation (Started by default)
$LiquidSim.stop() : Stops the simulation
$LiquidSim.update_simulation() : Force simulation update
$LiquidSim.cleanup_empty_cells() : Clean all empty cells (automatic)
$LiquidSim.add_liquid(int x, int y, float amount) : Adds some liquid to the cell.
$LiquidSim.del_liquid(int x, int y) : empty a cell
$LiquidSim.set_liquid(int x, int y, float amount) : Set a fixed amount of liquid to the cell
$LiquidSim.get_liquid(int x, int y) float : Returns the amount of liquid the cell has.
```

## Compiling problems.

As mentioned at the beginning, if you are working with the ["original"](https://github.com/tterrasson/liquid-simulator-godot) version of the plugin, follow the instructions in that repository.

In case you continue working with the version of this repository, I will leave you with a tip or solution to a problem that I encountered along the way and my experience will probably help you.

::: tip
At the time of compiling the plugin for the web version, I found that the **"SConstruct"** script did not contain the necessary code fragment for this task, I got a bash script from the **"godot-cpp"** repositories to compile to the version html5, however, this compilation gave me many problems, and even with little information to solve it, the recommendation of the **"godot"** documentation , as for the version of **"Emscripten"** is 1.39.9, after so many attempts and research I was able to conclude that for the compilation of this plugin the version of **"Emscripten"** that we should use is 2.0.17, and for convenience add the missing code snippet for this operation in the **"SConstruct"** file , so that we can directly execute the **"scons"** command with the addition that we should use the **"emscripten"** attribute to provide the path to our build tool, here is an example:

```sh
$ scons platform=javascript emscripten=/usr/bin/emscriptem/
```

:::

For more details, see ["Using GDNative Module"](https://docs.godotengine.org/en/3.2/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module)
