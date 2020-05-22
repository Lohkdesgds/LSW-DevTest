# Dependencies in LSW 5

<hr>

#### Abort
*Abort is the main "error" or "give up". It is used to get information why something went wrong, from where, and what is needed to do (yes, it does support variable functions to "clean up" if you want to).*
<hr>

#### AllegroInitialization
*AllegroInitialization is basically the main initialization base to easily initialize the app.*
<hr>

#### Autocaster
*Autocaster simplifies the casting of things around the code. No need to static_cast<>() or (type)var;. Use this to make it more readable and probably easier to expand the code.*
<hr>

#### Camera
*Camera is the main vision of the game. Set up the camera and you can control how coordinates work within the game!*
<hr>

#### Core
*Core starts, works and ends with the game. This is the CORE, this does everything.*
<hr>

#### Database
*Database is the base of everything, from the resolution to on-the-fly variables that you can really set on a online environment.*
<hr>

#### Display
*This is your display, or one of them. Want a display? Create it with this and you're good to go!*
<hr>

#### Downloader
*Quick download from a link? This can download from many places many stuff. Put the link and wait!*
<hr>

#### Filesystem
*Sometimes we do things in multiple ways. Why not use only one of them?*
<hr>

#### Hash
*Generate SHA256 here with this powerful tool.*
<hr>

#### Launcher
*You have an addon and wants it to run aside your app? Why don't you just LAUNCH it? This allows Discord extension and some pretty cool stuff to happen!*
<hr>

#### ColouredString
*This is the main coloured string you might see in the game.*
<hr>

#### Logger
*Logging stuff is pretty useful, why not make it the EASIEST thing to do? Take a look on Logger, the main logging system you'll keep everywhere you go!*
<hr>

#### Printer
*How to print coloured stuff? How to print always one way or another? This is it.*
<hr>

#### Shared
*Want some stuff shared around the code?*
<hr>

#### SocketSystem
*So you want multiplayer communication? Just use that then!*
<hr>

#### Sprite
*A base for everything in the game? This is so easy!*
<hr>

#### SuperMap
*An extended version of std::map with unique features!*
<hr>

#### SuperPair
*An extended version of std::pair used in SuperMaps!*
<hr>

#### SuperResource
*Want some resource around your app no brainer? Just create your data and you're good to go!*
<hr>

#### SuperThread
*You want things being done at the same time with full control? Maybe you want to check this out then!*
<hr>

#### SuperTimer
*If you need multiple timers so you won't forget all your tasks, try this!*
<hr>

#### Tools
*Useful tools to use around the app!*
<hr>

## Dependency configuration

Lib | #include
--- | --------
Abort | *none*
AllegroInitialization | *none*
Autocaster | *none*
Camera | SuperMap<br>Tools<br>AllegroInitialization<br>SuperResource
Core | Abort<br>SuperTimer<br>SuperMap<br>Logger<br>Display<br>Shared<br>Autocaster<br>SuperResource<br>Sprite
Database | Abort<br>SuperMap<br>Logger<br>Tools
Display | Abort<br>Database<br>SuperThread<br>Logger
Downloader | *none*
FileSystem | Tools
Hash | *none*
Launcher | *none*
ColouredString | *none*
Logger | Printer<br>ColouredString
Printer | ColouredString
Shared | *none*
SocketSystem | Abort
Sprite | SuperMap<br>AllegroInitialization<br>Autocaster<br>Tools<br>Camera<br>Shared
SuperMap | SuperPair
SuperPair | Autocaster<br>Abort
SuperResource | Abort<br>AllegroInitialization
SuperThread | AllegroInitialization
SuperTimer | AllegroInitialization
Tools | Abort
