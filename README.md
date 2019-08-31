2D Liquid simulator with cellular automaton in Godot Engine (GDNative / C++) - NativeScript 1.1

`$ git clone --recurse-submodules https://github.com/tterrasson/liquid-simulator-godot`

## Build godot-cpp binding
`$ cd liquid-simulator-godot/godot-cpp`
`$ use_custom_api_file=yes custom_api_file=../api.json scons platform=<os> generate_bindings=yes`

## Build plugin
`$ cd ..`
`$ scons platform=<os>`

Replace <os> with 'linux', 'osx' or 'windows'.

Then copy the "bin" directory to your project and follow https://docs.godotengine.org/en/3.1/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module