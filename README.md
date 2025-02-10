# Rubik's Cube OpenGL Assignment

## Changes
This document outlines the key modifications and improvements made to various files in the project.

### **Makefile**
- **Added `make clean` target** to remove compiled binaries and temporary files, ensuring a clean build environment.

### **main.cpp**
- **Created a RubikCube instance** and set up rendering.
- **Applied shaders** to handle object rendering and color transformations.
- **Modified the infinite rendering loop** to continuously display the cube and handle user interactions.

### **camera.cpp**
- **Added callbacks** for handling user input:
  - **Mouse movement** to control the camera perspective.
  - **Mouse scroll** to zoom in and out.
  - **Mouse press** to select a cube for manipulation.
- **Implemented function calls for RubikCube interactions**:
  - **Rotation functions** to turn different walls of the cube.
  - **Angle adjustments** for smoother animations.
  - **Clockwise/counterclockwise rotations** to provide flexible cube transformations.

### **RubikCube.cpp**
- **Implemented core Rubik's Cube logic**:
  - Defined rotation mechanics for each cube wall.
  - Managed cube transformations using OpenGL matrices.
  - Integrated the picking mechanism to interact with specific cube parts.
- **Enhanced animation and real-time updates** for a smoother user experience.

### **basic.shader**
- **Implemented picking highlight**:
  - If an object is selected, it appears darker than the rest.
- **Added picking mode for special color selection**:
  - Used a unique color mapping technique for object selection.
- **Ensured normal rendering mode works correctly**:
  - Objects render with their correct textures and colors when not in picking mode.

## **How to Build and Run**
1. **Build the project:**
   ```sh
   make
   ```
2. **Run the application:**
   ```sh
   cd bin
   ./main
   ```
3. **Clean up compiled files:**
   ```sh
   make clean
   ```