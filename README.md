# Field generator++
My [Field Generator](https://github.com/DDunda/Field-generator), but written with C++ instead. Unlike the other project, however, attractors are embedded through code for easier use and faster runtimes. This program is also exclusively for generating gravity-like fields unlike the original which can, technically, extend to other types of forces.

![A collage of examples](https://media.discordapp.net/attachments/364021339486421004/770947253862006784/Collage.png)

## Okay, but what does it actually do?
Well, it makes fields. Basically there are "planets" which have gravitational forces pulling inwards, but the direction the forces point toward changes depending on which pixel you pick on the screen. To show the direction the forces point, different colours are used. There are two different modes for generating these colours,:

* XY mode: is fast, but not entirely pleasing to look at. The red channel represents the strength in the X direction, and the green represents the strength in the y direction.
* Hue mode: the direction is represented by the colour wheel, or hue. This is a very colourful, but also quite a slow.

![Different modes](https://media.discordapp.net/attachments/364021339486421004/770948286265032725/Modes.png)

## How to compile Synthboard
Fields++ is written as standard C++ Visual Studio project that can be compiled and ran. Like most of my C++ projects, however, the SDL2 library is required for compilation. You can get SDL2 from the following link, and then simply copy it into the project directory to the folder called SDL2. Make sure to download the **development library**.

* SDL2: https://www.libsdl.org/download-2.0.php
