---
sidebar: auto
---

<iframe width="100%" height="400" src="https://www.youtube.com/embed/nF7cdUVgvNc" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

# Guía

Esta guía, te enseñará cómo compilar y agregar el plugin a un nuevo proyecto de **Godot engine**.

::: tip
Prueba una demostración del plugin [aqui](../game/)
:::

## Prerrequisitos.

Para el desenvolvimiento de esta guía, debemos tener instalado en nuestro sistema lo siguiente:

::: tip

- Godot engine 3.x
- Compilador C++
- Herramienta de compilación Scons
- Herramienta de compilación Emscripten 2.0.17 (En caso de compilar para web **"wasm"**).

:::

## Clonar Repositorio.

::: warning
Es probable que encuentres diferencias entre el repositorio original y este repositorio, si vas a usar el repositorio original de **"tterrasson"**, te recomiendo que sigas las instrucciones que encontraras en él, ya que puede haber diferencias en como se instala el plugin.
:::

Repositorio Original.

```sh
$ git clone --recurse-submodules https://github.com/tterrasson/liquid-simulator-godot
```

Este Repositorio.

```sh
$ git clone --recurse-submodules https://github.com/AJ-Wi/liquid-simulator-godot
```

## Compilar binario.

En esta sección veremos como compilar, tanto el binario de godot-cpp como nuestro plugin.

::: tip
En los siguientes comandos debemos sustituir **"\<os\>"** por la plataforma en la que queremos ejecutar nuestro juego:

- windows
- linux
- osx
- javascript
- andorid
  :::

### binario godot-cpp.

Necesitamos compilar el binario de godot-cpp acorde a la plataforma en la que queremos ejecutar nuestro juego, para ello debemos estar posicionados dentro de la carpeta godot-cpp en nuestro terminal.

```sh
$ cd liquid-simulator-godot/godot-cpp
$ scons platform=<os> generate_bindings=yes
```

### binario plugin.

Una vez compilado el binario de godot-cpp volvemos a la carpeta raíz del proyecto y compilamos el plugin.

```sh
$ cd ..
$ scons platform=<os>
```

## Copia el plugin a tu proyecto.

En este punto encontrarás el plugin compilado en la carpeta **"bin"** del proyecto, solo debes copiar esta carpeta a la carpeta **"addons"** de tu proyecto, y renombrar la carpeta del plugin como **"liquidsim"**, también puedes ejecutar el siguiente comando.

```sh
$ cp -r bin/* /ruta/a/tuproyecto/addons/liquidsim
```

Deberías quedar con la siguiente estructura de archivos en tu proyecto.

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
    |        |   └── `libliquidsim.so` (segun el que hayas compilado.)
    |        ├── LiquidSim.tscn
    |        ├── liquidsim_plugin.gd
    |        ├── liquidsim.gdnlib
    |        ├── liquuidsim.gdns
    |        └── plugin.cfg
    ├── default_env.tres
    ├── icon.png
    ├── project.godot
```

## Activa el plugin.

Para ello accedes a la configuración de tu proyecto y en la sección de plugin te aparecerá **"LiquidSim"**, solo debes activarlo.

![alt img](~@assets2/activate_plugin.png)

## Agregar Nodo.

Una vez activado, ya nos aparece el plugin en la lista de nodos del editor.

![alt img](~@assets2/add_plugin_to_tilemap.png)

Para que funcione nuestro nuevo nodo o plugin, debemos agregarlo como nodo hijo de un **"TileMap"**.

![alt img](~@assets2/added_liquidsim_node.png)

## Ajuste del plugin.

Ajusta el tamaño de las celdas del plugin según el tamaño de celdas de tu **"TileMap"**.

![alt img](~@assets2/adjust_cell_size.png)

## Probar el plugin.

Adjunte un gdscript al **"TileMap"** para probar el complemento, puede usar el siguiente código de muestra.

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
$LiquidSim.start() : Inicia la simulación (Iniciado por defecto)
$LiquidSim.stop() : Detiene la simulación
$LiquidSim.update_simulation() : Fuerza la actualización de la simulación
$LiquidSim.cleanup_empty_cells() : Limpiar todas las celdas vacías (automático)
$LiquidSim.add_liquid(int x, int y, float amount) : Añade un poco de líquido a la celda.
$LiquidSim.del_liquid(int x, int y) : vaciar una celda
$LiquidSim.set_liquid(int x, int y, float amount) : Establecer una cantidad fija de líquido a la celda
$LiquidSim.get_liquid(int x, int y) float : Retorna la cantidad de líquido que tiene la celda.
```

## Problemas al compilar.

Como se mencionó al comienzo, si estás trabajando con la versión ["original"](https://github.com/tterrasson/liquid-simulator-godot) del plugin, sigue las instrucciones de ese repositorio.

En caso de continuar trabajando con la versión de este repositorio, te dejo un tip o solución a un problema que yo me encontré en el camino y probablemente a ti te sirva mi experiencia.

::: tip
Al momento de compilar el plugin para la versión web, me encontré con que el script **"SConstruct"** no contenía el fragmento de código necesario para dicha tarea, conseguí en los repositorios de **"godot-cpp"** un script de bash para compilar a la versión html5, sin embargo, dicha compilación me produjo muchos problemas, e incluso con poca información para poder solucionarlo, la recomendación de la documentación de **"godot"**, en cuanto a la versión de **"Emscripten"** es la 1.39.9, después de tantos intentos e investigar pude concluir que para la compilación de este plugin la versión de **"Emscripten"** que debemos usar es la 2.0.17, y para mayor comodidad agregue el fragmento de código faltante para esta operación en el archivo **"SConstruct"**, de manera que podemos ejecutar directamente el comando **"scons"** con el agregado que debemos usar el atributo **"emscripten"** para proporcionar la ruta de nuestra herramienta de compilación, aquí les dejo un ejemplo:

```sh
$ scons platform=javascript emscripten=/usr/bin/emscriptem/
```

:::

Para obtener más detalles, consulte ["Uso de módulo GDNative"](https://docs.godotengine.org/en/3.2/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module)
