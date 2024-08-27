# Simple 3D renderer

This repository contains a simple renderer written in C using SDL2 library for visuals.

## What's Inside?

### Simple 3D renderer

- Only rendering a cube for now, but will be able to load object files in the future.
  
- Cube can be rotated by pressing ```X```, ```Y``` or ```Z```.

- Cube can be moved further away or closer by pressing &#8593; or &#8595;

- Scenes can be changed by pressing ```TAB```.
  
- Currently there are 3 scenes:
  - Textured cube
  - Skinned cube with texture wrapped around the cube
  - Two skinned cubes rotating in oposite directions where one can be moved
    

[Screencast from 2024-08-27 14-54-32.webm](https://github.com/user-attachments/assets/f5faf72c-4683-4e53-b47d-01622db48490)


### How to Use

1. **Clone the Repository:** Get the code onto your computer by copying this repository.
2. **Open Terminal:** Launch your terminal and navigate to the repository folder (`cd 3d-renderer/`).
3. **Compile and Run:** `make`to compile, then start the game (`./renderer` or `make run`).

### What You Need

- **GCC Compiler:** You'll need a compiler to turn the code into a working program.
- **SDL2 Library:** To see the visual representation, you'll need the SDL2 library.


