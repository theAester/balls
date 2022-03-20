# balls
bouncy balls.(v1.0)

They bounce.

They make sound.

following the video that became popular on instagram a while ago.

## requirements
the entire SDL library:

for debian linux read https://gist.github.com/aaangeletakis/3187339a99f7786c25075d4d9c80fad5

## compiling
use the command bellow
``` bash
g++ proj.cpp main.cpp -o balls `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf
./balls
```
## Code manipulation
Youll find useful comments inside the main.cpp file on how to customize the code. I list the parameters here:
* const int N : number of balls
* const int rad : radius of each ball
* const int starrt : starting bpm (the bpm of each ball is one greater than the previous)
* const double angle : angle between the two lines
* doube jmps[] : the differential sequence of the scales available

## TODO:
Create UI
bug fixes
