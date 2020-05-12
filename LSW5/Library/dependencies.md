# Dependencies in LSW 5

### Themselves

* Abort
* AllegroInitialization
* Autocaster
* Camera
* Core
* Database
* Display
* Downloader
* Filesystem
* Hash
* Launcher
* ColouredString
* Logger
* Printer
* Shared
* SocketSystem
* Sprite
* SuperMap
* SuperPair
* SuperResource
* SuperThread
* SuperTimer
* Tools


### Dependency configuration

Lib | #include
--- | --------
* Abort | none
* AllegroInitialization | none
* Autocaster | none
* Camera | * SuperMap
 | * Tools
 | * AllegroInitialization
* Core | * Abort
 | * SuperTimer
 | * SuperMap
 | * Logger
 | * Display
 | * Shared
 | * Autocaster
 | * SuperResource
 | * Sprite
* Database | * Abort
 | * SuperMap
 | * Logger
 | * Tools
* Display | * Abort
 | * Database
 | * SuperThread
 | * Logger  
* Downloader | none
* FileSystem | * Tools
* Hash | none
* Launcher | none
* ColouredString | none
* Logger | * Printer
 | * ColouredString
* Printer | * ColouredString
* Shared | none
* SocketSystem | * Abort
* Sprite | * SuperMap
 | * AllegroInitialization
 | * Autocaster
 | * Tools
 | * Camera
 | * Shared
* SuperMap | * SuperPair
* SuperPair | * Autocaster
 | * Abort
* SuperResource | * Abort
 | * AllegroInitialization
* SuperThread | * AllegroInitialization
* SuperTimer | * AllegroInitialization
* Tools | * Abort