# ChessLights

## Introduction
This project is created as a part of the university Computer Graphics course.
The purpose of this program is to present some basic and more advanced graphics concepts.
Application was written in C++ with GLSL graphics shaders. It uses OpenGL with GLEW and GLFW librararies. I used [Assimp](https://github.com/assimp/assimp) package for object loading and [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) for image loading.

## Functionalities
Currently ChessLights project has the following functionalities:
- Visualisation of several 3D chess objects on the board.
- 4 different camera modes: static, tracking, POV and free.
- 3 types of light sources: directional, point and spotlight, along with system of power decreasing with distance.
- Moving, rotating and vibrating object with responsive spotlights on it.
- Day and night cycle.
- Smoothly changing fog effect.
- 3 shading options: flat, Gourad and Phong shading.
- Controlling interface for several functionalities.

## Manual
When camera mode allows that, you can move with WSAD and mouse. When lights on, you can change the direction of spotligh with arrows. Buttons:
- 1 - Turn on/off fog
- 2 - Change camera mode (Static, POV, Tracking, Free)
- 3 - Object shaking on/off
- 4 - Lamps on/off
- 5 - Time start/stop
- 6 - Shading mode (flat, Gourad, Phong)

# Description
## Shading models
### Flat
This model uses 1 color for every pixel in the triangle. Board has very few polygons that is why it looks poor. We can notice edges in the round figures.
<p align="center">
<img width="800" alt="flat" src="https://github.com/Pitchiu/ChessLights/assets/69166155/4c897e98-5a39-412f-8dc3-e60d28c6937b">
</p>

### Gourad
In this model, colors are interpolated depending on position in a triangle. Result is better, but defects are still visible.

  <p align="center">
<img width="800" alt="gourad" src="https://github.com/Pitchiu/ChessLights/assets/69166155/61c30155-0908-4921-819f-89e028b23e1f">
</p>

### Phong
In this model, normal vectors and colors are interpolated. Best result.

  <p align="center">
<img width="800" alt="phong" src="https://github.com/Pitchiu/ChessLights/assets/69166155/01cf9199-0bab-4213-aa3e-0a29bbc9b804">
</p>

## Fog
Fog density is calculated with function: $e^{-(distance \cdot density)^{gradient}}$
  <p align="center">
<img width="800" alt="Zrzut ekranu 2023-09-06 013342" src="https://github.com/Pitchiu/ChessLights/assets/69166155/8916802a-ac95-404f-a1db-94935f712d38">
</p>

## Moving
White king is constantly moving and rotating.


https://github.com/Pitchiu/ChessLights/assets/69166155/714c5926-0d29-4ab0-b03d-ee1f2f588796

