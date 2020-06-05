# CPPND: Enhanced Snake Game 

This is the repo for the Capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

In this code you will find the included features below:
1) Included the a random generator for 50% spawn of bad food. The bad food decreases the score, and size. If size reached 1 again, the snake will die.
2) Start message box.
3) ESC key for pause game.
4) End game message box to display Score ans Snake Size.
5) Sound effects - using the SDL2_mixer the game included 4 sound effects (Eat good food, Eat bad food, Start Game, and Game Over). 
6) Score in the upper corner of the windows, using the SDL2_ttf to generate the texture.
7) Background using SDL2_image to load a png image, optmize to the windows size, and generate to the texture.
You can easily change the background image changing the surface.png in the folder. 

<img src="snake_game.gif"/>

This code can be rebuild to run on Linux, Mac or Windows, just make sure you have the dependencies below.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  * Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. If you preffer to compile the library make sure you install in a folder where the cmake can find it. You can verify the cmake findpackage in the cmake folder.
* SDL2_image >= 2.0
  * All installation instructions can be found [here](https://www.libsdl.org/projects/SDL_image/)
  * Note that for Ubuntu, use sudo apt-get install	libsdl2-image-dev
* SDL2_ttf >= 2.0
  * All installation instructions can be found [here](https://www.libsdl.org/projects/SDL_ttf/)
  * Note that for Ubuntu, use sudo apt-get install	libsdl2-ttf-dev
* SDL2_mixer >= 2.0
  * All installation instructions can be found [here](https://www.libsdl.org/projects/SDL_mixer/)
  * Note that for Ubuntu, use sudo apt-get install	libsdl2-mixer-dev
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.
