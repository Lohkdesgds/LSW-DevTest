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
* SuperMap
* SuperPair
* SuperResource
* SuperThread
* SuperTimer
* Tools


### Dependency configuration

* Abort

* AllegroInitialization

* Autocaster
 
* Camera
  * SuperMap
    * SuperPair
      * Abort
      * Autocaster
  * Tools
    * Abort
  * AllegroInitialization

* Core
  * Abort
  * SuperTimer
    * AllegroInitialization
  * SuperMap
    * SuperPair
      * Abort
      * Autocaster
  * Logger
    * Printer
      * ColouredString
  * Display
  * Shared
  * Autocaster
  
* Database
  * Abort
  * SuperMap
    * SuperPair
      * Abort
      * Autocaster
  * Logger
    * Printer
      * ColouredString
  * Tools
    * Abort

* Display
  * Logger
    * Printer
      * ColouredString
  * Database
    * Abort
    * SuperMap
      * SuperPair
        * Abort
        * Autocaster
    * Logger
      * Printer
        * ColouredString
    * Tools
      * Abort
  * SuperThread
    * AllegroInitialization
  * Logger
    * Printer
      * ColouredString
  
* Downloader

* FileSystem
  * Tools
  
* Hash

* Launcher

* ColouredString

* Logger
  * Printer
    * ColouredString
    
* Printer
  * ColouredString

* Shared

* SocketSystem
  * Abort

* SuperMap
  * SuperPair
    * Abort
    * Autocaster

* SuperResource
  * Abort
  * AllegroInitialization
  
* SuperThread
  * AllegroInitialization

* SuperTimer
  * AllegroInitialization
  
* Tools
  * Abort