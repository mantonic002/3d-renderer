# Simple 3D renderer

This repository contains a simple renderer written in C using SDL2 library for visuals.

## What's Inside?

### Simple 3D renderer

- Only rendering a cube for now, but will be able to load object files in the future.
  
- Cube can be rotated by pressing ```X```, ```Y``` or ```Z```.

- Cube can be moved further away or closer by pressing &#8593; or &#8595;

- Scenes can be changed by pressing ```TAB```.
  
- Currently there are 6 scenes:
  - Cat with textures and shading  
  - Textured cube
  - Skinned cube with texture wrapped around the cube
  - Two skinned cubes rotating in oposite directions where one can be moved
  - Wavy plane
  - Shaded white cube
    
[Screencast from 2024-08-31 20-17-00.webm](https://github.com/user-attachments/assets/27ab52ef-97a2-4f8b-8eac-3dbbf2376348)

### How to Use

1. **Clone the Repository:** Get the code onto your computer by copying this repository.
2. **Open Terminal:** Launch your terminal and navigate to the repository folder (`cd 3d-renderer/`).
3. **Compile and Run:** `make`to compile, then start the game (`./renderer` or `make run`).

### What You Need

- **GCC Compiler:** You'll need a compiler to turn the code into a working program.
- **SDL2 Library:** To see the visual representation, you'll need the SDL2 library.


