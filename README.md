# Simple 3D renderer

This repository contains a simple renderer written in C using SDL2 library for visuals.

## What's Inside?

### Simple 3D renderer
  
- Objects can be rotated by pressing ```Z```, ```X``` or ```C```.
  
- In scenes with point light, light can be moved around by pressing ```W```, ```A```, ```S``` and ```D```.

- Objects can be moved further away or closer by pressing &#8593; or &#8595;

- Scenes can be changed by pressing ```TAB```.
  
- Currently there are 8 scenes:
  - Shark with point light that can be moved around
  - Cat with textures and a point light that can be moved around  
  - Sphere with flat shading
  - Cube with flat shading
  - Wavy plane
  - Two skinned cubes rotating in oposite directions where one can be moved (demonstrating Z-buffering)
  - Cube with same texture on 4 sides
  - Skinned cube with texture wrapped around the cube
    
[Screencast from 2024-09-04 18-02-43.webm](https://github.com/user-attachments/assets/19ef7bdf-1cfd-4c97-9978-1b538ce70e21)

### How to Use

1. **Clone the Repository:** Get the code onto your computer by copying this repository.
2. **Open Terminal:** Launch your terminal and navigate to the repository folder (`cd 3d-renderer/`).
3. **Compile and Run:** `make`to compile, then start the game (`./renderer` or `make run`).

### What You Need

- **GCC Compiler:** You'll need a compiler to turn the code into a working program.
- **SDL2 Library:** To see the visual representation, you'll need the SDL2 library.


