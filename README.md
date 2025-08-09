# recurse

![](https://img.itch.zone/aW1nLzIyNTIwMjM5LnBuZw==/315x250%23c/Z65wS%2F.png)

Developed for the [GMTK Game Jam 2025](https://itch.io/jam/gmtk-2025) in 96 hours.

You can play the game [here](https://axavian.itch.io/recurse).

## Game Overview

**recurse** is a minimalist puzzle game about being trapped in a repeating time loop.  
Each loop, your past self returns as an _echo_, replaying your exact previous actions. Use these echoes to solve environmental puzzles — press switches, push objects, open doors, and chain your actions across multiple timelines. Plan carefully: your past selves can be your greatest allies or your worst obstacles.

## Features

-   Designed and built entirely from scratch with a **custom C++ game engine**.

-   Implemented a **full desktop and web build** during the jam using Emscripten.

-   Integrated real-time rendering with OpenGL and a clean, data-driven asset pipeline.

-   Developed a minimal physics and collision system tailored for puzzle interactions.

-   Audio system with streaming background music, sound effects, and volume control via **miniaudio**.

-   Cross-platform: runs natively on Windows and in the browser.

## Technology Stack

**Languages & Tools**

-   C++ (core engine and gameplay code)

-   Premake (build configuration)

-   Emscripten (WebAssembly build for browser play)

**Libraries**

-   OpenGL (via GLAD) – rendering

-   GLFW – window & input management

-   GLM – math library

-   miniaudio – audio playback (music & SFX)

-   stb_image – texture loading

## Build and Run

The project uses **Premake** for build configuration, with batch scripts to simplify setup on Windows.

**Building the Project**

Run the Visual Studio project generator script:

`Build\vs2022.bat` 

This script will generate all project files and a solution (`.sln`) in the root directory. You can change the script filename to generate for a different Visual Studio version (e.g., `vs2019.bat` for Visual Studio 2019).

**Cleaning the Project**

To clean the generated files, run:

`Build\BuildSystemCleanup.bat` 

This removes generated solution and project files to reset the build environment.

**Running the Game**

The build system configures the Game project as the startup project. The generated solution is ready to compile and run with Debug, Release, and Shipping configurations.

## Development Notes

This repository is a self-contained public snapshot of the GMTK 2025 version.
The original development used a private engine repository, which has been included here in a trimmed-down form for public release.

The core engine architecture was partially adapted from a previous personal project, but all gameplay systems, level logic, sound system, rendering effects, and shaders were created specifically for this jam.
Game assets were sourced from free or open-license repositories, while all shaders and engine-side integration were authored from scratch.
The web build required additional work to integrate audio initialization on user input, handle asset loading from the virtual file system, and ensure consistent rendering performance in WebGL.

## License

- **Code:** Licensed under the MIT License — see `LICENSE` in the root folder.
- **Assets and Thirdparty:** Licensed under their respective terms — see the `LICENSE` files inside each subfolder for details.
