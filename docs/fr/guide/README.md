---
sidebar: auto
---

<iframe width="100%" height="400" src="https://www.youtube.com/embed/nF7cdUVgvNc" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

# Guide

Ce guide vous apprendra comment compiler et ajouter le plugin à un nouveau projet de **Godot engine**.

::: tip
Essayez une démo du plugin [ici](../game/)
:::

## Conditions préalables.

Pour le développement de ce guide, nous devons avoir les éléments suivants installés dans notre système :

::: tip

- Godot engine 3.x
- Compilateur C++
- Outil de construction de Scons
- Outil de construction Emscripten 2.0.17 (En cas de construction pour le web **"wasm"**).

:::

## Dépôt de clones.

::: warning
Il est probable que vous trouviez des différences entre le référentiel d'origine et ce référentiel, si vous allez utiliser le référentiel **"tterrasson"** d' origine , je vous recommande de suivre les instructions que vous y trouverez, car il peut y avoir des différences dans la façon dont le plugin est installé.
:::

Dépôt d'origine.

```sh
$ git clone --recurse-submodules https://github.com/tterrasson/liquid-simulator-godot
```

Ce référentiel.

```sh
$ git clone --recurse-submodules https://github.com/AJ-Wi/liquid-simulator-godot
```

## Compiler le binaire.

Dans cette section, nous verrons comment compiler à la fois le binaire godot-cpp et notre plugin.

::: tip
Dans les commandes suivantes, nous devons remplacer **"\<os\>"** par la plate-forme sur laquelle nous voulons exécuter notre jeu :

- windows
- linux
- osx
- javascript
- andorid
  :::

### binaire godot-cpp.

Nous devons compiler le binaire godot-cpp en fonction de la plate-forme sur laquelle nous voulons exécuter notre jeu, pour cela nous devons être positionnés dans le dossier godot-cpp de notre terminal.

```sh
$ cd liquid-simulator-godot/godot-cpp
$ scons platform=<os> generate_bindings=yes
```

### binaire du plugin.

Une fois le binaire godot-cpp compilé, nous retournons au dossier racine du projet et compilons le plugin.

```sh
$ cd ..
$ scons platform=<os>
```

## Copiez le plugin dans votre projet.

À ce stade, vous trouverez le plugin compilé dans le dossier **"bin"** du projet, il vous suffit de copier ce dossier dans le dossier **"addons"** de votre projet, et de renommer le dossier du plugin en **"liquidsim"**, vous pouvez également exécuter la commande suivante.

```sh
$ cp -r bin/* /ruta/a/tuproyecto/addons/liquidsim
```

Vous devriez vous retrouver avec la structure de fichiers suivante dans votre projet.

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

## Activez le plugin.

Pour cela, vous accédez à la configuration de votre projet et **"LiquidSim"** apparaîtra dans la section plugin, vous n'aurez plus qu'à l'activer.

![alt img](~@assets2/activate_plugin.png)

## Ajouter un nœud.

Une fois activé, le plugin apparaît déjà dans la liste des nœuds de l'éditeur.

![alt img](~@assets2/add_plugin_to_tilemap.png)

Pour que notre nouveau nœud ou plugin fonctionne, nous devons l'ajouter en tant que nœud enfant d'un **"TileMap"**.

![alt img](~@assets2/added_liquidsim_node.png)

## Paramètre de plug-in.

Ajustez la taille des cellules du plugin en fonction de la taille des cellules de votre **"TileMap"**.

![alt img](~@assets2/adjust_cell_size.png)

## Essayez le plugin.

Attachez un gdscript au **"TileMap"** pour tester le plugin, vous pouvez utiliser l'exemple de code suivant.

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
$LiquidSim.start() : Démarre la simulation (Démarré par défaut)
$LiquidSim.stop() : Arrête la simulation
$LiquidSim.update_simulation() : Force la mise à jour de la simulation
$LiquidSim.cleanup_empty_cells() : Nettoyer toutes les cellules vides (automatique)
$LiquidSim.add_liquid(int x, int y, float amount) : Ajoute du liquide à la cellule.
$LiquidSim.del_liquid(int x, int y) : vide une cellule
$LiquidSim.set_liquid(int x, int y, float amount) : définit une quantité fixe de liquide dans la cellule
$LiquidSim.get_liquid(int x, int y) float : Renvoie la quantité de liquide contenue dans la cellule.
```

## Problèmes de compilation.

Comme mentionné au début, si vous travaillez avec la version ["originale"](https://github.com/tterrasson/liquid-simulator-godot) du plugin, suivez les instructions de ce référentiel.

Si vous continuez à travailler avec la version de ce référentiel, je vous laisserai une astuce ou une solution à un problème que j'ai rencontré en cours de route et mon expérience vous aidera probablement.

::: tip
Au moment de compiler le plugin pour la version web, j'ai trouvé que le script **"SConstruct"** ne contenait pas le fragment de code nécessaire pour cette tâche, j'ai récupéré un script bash des dépôts **"godot-cpp"** pour compiler vers la version html5 , cependant, cette compilation m'a posé de nombreux problèmes, et même avec peu d'informations pour la résoudre, la recommandation de la documentation **"godot"** , quant à la version de **"Emscripten"** est 1.39.9, après tant de tentatives et de recherches j'ai pu pour conclure que pour la compilation de ce plugin la version de **"Emscripten"** que nous devrions utiliser est 2.0.17, et pour plus de commodité, ajoutez l'extrait de code manquant pour cette opération dans le fichier **"SConstruct"** , afin que nous puissions directement exécuter la commande **"scons"** avec en plus que nous devrions utiliser l' attribut **"emscripten"** pour fournir le chemin vers notre outil de compilation, voici un exemple :

```sh
$ scons platform=javascript emscripten=/usr/bin/emscriptem/
```

:::

Pour plus de détails, voir ["Utiliser le module GDNative"](https://docs.godotengine.org/en/3.2/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module)
