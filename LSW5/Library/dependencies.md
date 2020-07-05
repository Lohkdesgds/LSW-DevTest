# Dependencies in LSW 5
*Updated 2020/07/04 approx 21:14 GMT-3*

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

#### Entities
*Blocks, Texts, stuff shown on screen mostly based on Sprite_Base*
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

#### SuperMutex
*An extended version of std::mutex more like a semaphore!*
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

#### Voice
*It's the sound device itself (hardware or software).*
<hr>

#### Mixer
*It's an interface for handling samples/tracks/other mixers.*
<hr>

#### Sample
*It's the music file handler itself. If you want to load a music, you have to firstly create a Sample of it.*
<hr>

#### Track
*It's the music "player" (instance). You can load a Sample and handle it with this.*
<hr>

## Dependency configuration
*Some dependencies may be chained from other headers.*


Lib | #include
--- | --------
Abort | *none*
AllegroInitialization | *none*
Autocaster | *none*
Camera | SuperMap<br>Tools<br>AllegroInitialization<br>SuperResource<br>Database
Core | Abort<br>SuperTimer<br>SuperMap<br>Logger<br>Display<br>Shared<br>Autocaster<br>Database<br>SuperResource<br>Sprite<br>Entities<br>Tracks
Database | Abort<br>SuperMap<br>Logger<br>Tools<br>SuperMutex
Display | Abort<br>SuperThread<br>AllegroInitialization<br>SuperMutex<br>Tools<br>SuperMap
Downloader | *none*
Entities | Sprite<br>SuperResource<br>Database<br>Tools<br>Shared
FileSystem | Tools<br>AllegroInitialization
Hash | *none*
Launcher | *none*
ColouredString | *none*
Logger | Printer<br>ColouredString<br>Abort<br>Tools<br>Shared
Printer | ColouredString
Shared | *none*
SocketSystem | Abort<br>Tools<br>FileSystem<br>SuperMutex
Sprite | Abort<br>SuperMap<br>AllegroInitialization<br>Autocaster<br>Tools<br>Camera<br>Shared<br>Database
SuperMap | SuperPair
SuperPair | Autocaster<br>Abort
SuperMutex | *none*
SuperResource | Abort<br>AllegroInitialization<br>SuperMutex
SuperThread | AllegroInitialization
SuperTimer | AllegroInitialization
Tools | Abort
Voice | SuperResource<br>AllegroInitialization<br>Mixer<br>Tools
Mixer | SuperResource<br>AllegroInitialization<br>Tracks
Track | Abort<br>SuperResource<br>AllegroInitialization<br>Sample<br>SuperMap<br>Tools
Sample | SuperResource<br>AllegroInitialization<br>Tools