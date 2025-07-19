# Ray Tracing Input/Output Documentation

## Introduction

This document explains the inputs and outputs used in the MiniRT ray tracing program, their purpose, and why they are essential for the ray tracing process.

## Inputs

### Scene Description File (.rt)

The primary input to MiniRT is a scene description file with the `.rt` extension. This file defines the entire 3D environment that will be rendered.

**Format:** Text file containing object definitions and scene parameters
**Purpose:** To provide a structured way to describe the 3D scene

**Elements defined in scene files:**

1. **Ambient Lighting**
   - Ratio: Controls the intensity of ambient lighting (0.0 to 1.0)
   - Color: RGB values for ambient light color
   - **Why needed:** Provides baseline illumination for areas not directly lit by light sources

2. **Camera**
   - Origin: 3D coordinates of camera position
   - Target/Direction: Where the camera is looking
   - FOV (Field of View): Angular extent of the scene visible (in degrees)
   - **Why needed:** Defines the viewpoint from which the scene is rendered

3. **Light Source**
   - Position: 3D coordinates of the light source
   - Brightness: Intensity of the light (0.0 to 1.0)
   - Color: RGB values for light color
   - **Why needed:** Provides illumination for the scene, creating shadows and highlights

4. **Objects**
   - **Spheres:**
     - Center: 3D coordinates of sphere center
     - Radius: Size of the sphere
     - Color: RGB values for sphere color
   
   - **Planes:**
     - Point: A point on the plane
     - Normal: Vector perpendicular to the plane
     - Color: RGB values for plane color
   
   - **Cylinders:**
     - Center: 3D coordinates of cylinder base center
     - Axis: Direction vector of the cylinder
     - Diameter: Width of the cylinder
     - Height: Length of the cylinder
     - Color: RGB values for cylinder color
   
   - **Why needed:** These objects constitute the 3D world that will be rendered

## Processing

### Ray Generation

**Inputs:**
- Camera parameters (position, direction, FOV)
- Pixel coordinates (x, y)
- Image dimensions (width, height)

**Output:**
- Ray (origin, direction)

**Why needed:**
- Each pixel in the final image requires a ray to be cast from the camera through that pixel
- The ray's direction determines what part of the scene will be visible at that pixel

### Ray-Object Intersection

**Inputs:**
- Ray (origin, direction)
- Object parameters (position, size, orientation)

**Outputs:**
- Hit information (hit point, normal, t-value, color)
- Boolean indicating whether a hit occurred

**Why needed:**
- Determines which objects are visible from the camera
- Provides essential information for lighting calculations
- The closest intersection point is what will be visible in the final image

### Lighting Calculation

**Inputs:**
- Hit point
- Surface normal at hit point
- Object color
- Light source parameters
- Ambient light parameters

**Output:**
- Final color for the pixel

**Why needed:**
- Calculates how light interacts with surfaces
- Combines ambient and diffuse lighting
- Creates the visual appearance of depth and realism in the scene

## Outputs

### Image Rendering

**Inputs:**
- Array of calculated pixel colors

**Output:**
- Visual display of the rendered scene

**Why needed:**
- Converts the mathematical results of ray tracing into a visual representation
- Displays the final rendered image to the user

### Debug Information

**Inputs:**
- Scene data
- Object data

**Output:**
- Console output of scene parameters and objects

**Why needed:**
- Helps verify that the scene was loaded correctly
- Useful for debugging and development

## Technical Implementation

### Ray-Sphere Intersection

Ray-sphere intersection involves solving a quadratic equation:

```
(d·d)t² + 2(o-c)·dt + (o-c)·(o-c) - r² = 0
```

Where:
- d is the ray direction
- o is the ray origin
- c is the sphere center
- r is the sphere radius
- t is the distance along the ray

### Ray-Plane Intersection

Ray-plane intersection involves solving:

```
t = ((p - o)·n) / (d·n)
```

Where:
- p is a point on the plane
- n is the plane normal
- o is the ray origin
- d is the ray direction
- t is the distance along the ray

## Conclusion

Ray tracing is fundamentally an input/output system where the inputs (scene description) are processed through mathematical algorithms to produce outputs (rendered image). Each stage requires specific inputs and generates outputs that feed into the next stage of the rendering pipeline.

The quality and accuracy of the inputs directly affect the realism and correctness of the output image. Properly structured inputs make the ray tracing process more efficient and produce more visually pleasing results.
