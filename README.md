# Cpp3Dgraphics

## What?
A simple rendering pipeline from vertices to 3D graphics.

## Why?
Learn by building!

## How?
GLFW for platform specifics. 
GLAD2 for OpenGL function pointers.
stb\_image.h for loading image data.

Wrapped up in C++23.

## Build
If needed, generate GLAD 2 and drop the files in the include folder.

Install GLFW using your favorite packagemanager.
If not in a path know to CMake, pass CMAKE\_MODULE\_PATH=<your path> (iirc).

<code>
<pre>
mkdir build
cmake -B build -S .
cmake --build
</pre>
</code>

If using Ninja: <code>cmake -G "Ninja" -B build -S .</code>
