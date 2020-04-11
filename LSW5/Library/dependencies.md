# Dependencies in LSW 5

### Themselves

* Abort
* AllegroInitialization
* Autocaster
* Filesystem
* ColouredString
* Logger
* Printer
* SocketSystem
* SuperMap
* SuperPair
* SuperResource
* SuperTimer
* Tools


### Dependency configuration

* Abort

* AllegroInitialization

* Autocaster

* Tools

* FileSystem
  * Tools

* Logger
  * Printer
    * ColouredString

* SocketSystem
  * Abort

* SuperMap
  * SuperPair
    * Abort
    * Autocaster

* SuperResource
  * Abort
  * AllegroInitialization

* SuperTimer
  * AllegroInitialization
