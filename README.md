# Simple 3D renderer

This repository contains a simple renderer written in C using SDL2 library for visuals.

## What's Inside?

### Simple 3D renderer
  
- Objects can be rotated by pressing ```Z```, ```X``` or ```C```.
  
- In scenes with point light, light can be moved around by pressing ```W```, ```A```, ```S``` and ```D```.

- Camera can be moved by pressing &#8592;, &#8593;, &#8594; or &#8595;

- Camera can be rotated by holding ```LEFT CLICK``` and moving the mouse around;

- Scenes can be changed by pressing ```TAB```.
  
- Currently there are 7 scenes:
  - Shark with point light (per pixel) that can be rotated around
  - Shark with point light (per vertex) that can be rotated around
  - Cat with textures and a point light that can be moved around  
  - Cube with point light (per pixel)
  - Sphere with point light (per pixel)
  - Cube with flat shading
  - Textured wavy plane
    
[Screencast from 2024-09-12 00-58-54.webm](https://github.com/user-attachments/assets/e8881996-87de-4f34-99a3-1e1733183036)

### How to Use

1. **Clone the Repository:** Get the code onto your computer by copying this repository.
2. **Open Terminal:** Launch your terminal and navigate to the repository folder (`cd 3d-renderer/`).
3. **Compile and Run:** `make`to compile, then start the game (`./renderer` or `make run`).

### What You Need

- **GCC Compiler:** You'll need a compiler to turn the code into a working program.
- **SDL2 Library:** To see the visual representation, you'll need the SDL2 library.

  - Install on Debian/Ubuntu: `sudo apt install libsdl2-dev`
  
- **SDL2 Image:** For loading iamge files to use as textures.

  - Install on Debian/Ubuntu: `sudo apt install libsdl2-image-dev`
