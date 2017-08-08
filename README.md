# RBMemViewer
A simple hex memory viewer designed entirely for Gen I Pokémon games - or more precisely, Pokémon Red/Blue. Most notably, the ASCII view is using the game's proprietary character/tile set, and defaults to a 20-column hex display (20 tiles is the width of a Gameboy screen, making memory contents look a lot more "aligned").

![RBMemViewer in action](https://cdn.pbrd.co/images/GEKcKp1.png)

The application works by directly reading the emulator's memory. Currently supported are BGB (fully) and all versions of VisualBoyAdvance (partially - the application might disconnect/show wrong information a lot).

# Installation
Precompiled binaries for Windows are available in the `bin` directory.

Or, if you feel adventurous, you can compile it all by yourself! After setting up MSYS/Cygwin/Gnuwin32 and `g++`, get the sources, run that `make` and hope for the best.

# Getting started
While the application is running, start up your preferred emulator, load up your preferred Gen I Pokémon game and view the Trainer Card. This will allow the application to synchronize with the emulator. Once it's done, you can do whatever you want in the game.

# Controls
Left/right arrow keys: Move current address by -1/+1
Up/down arrow keys: Go up/down a single row
PgUp/PgDn: Go up/down an entire screen
W/S: Change number of rows (default 18, maximum 24, minimum 2)
A/D: Change number of columns (default 20, maximum 30, minimum 4)

# Shameless todo list
- Support for different tilesets (or better yet - read the tileset data in real time from VRAM)
- Allow editing of memory
- Make navigation easier ('go to' command, provide some mouse controls, allow for loading of SYM files)
- Support for Pokémon Yellow and other GBC games under VisualBoyAdvance, and better support for VisualBoyAdvance in general (VBA's coding makes it difficult to consistently read the game's RAM from process memory)
- Support for Gen II Pokémon games. Or potentially extend this to ALL the games, making the project into a "Gameboy game hex viewer".
- Currently Win32 is the only supported platform - show some love for Linux maybe?