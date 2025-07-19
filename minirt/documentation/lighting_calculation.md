# Lighting Calculation in Ray Tracing

## Overview of `compute_lighting` Function

The `compute_lighting` function is a critical component of any ray tracer, responsible for determining the color of a pixel based on how light interacts with object surfaces.

## Function Signature

```c
t_vec3 compute_lighting(t_point3 hit_point, t_vec3 normal, t_vec3 object_color, t_light light);
```

## Input Parameters

1. **`hit_point` (t_point3)**:
   - The 3D coordinates where a ray intersects an object
   - Used to calculate the direction from the hit point to the light source
   - Critical for determining shadows and light falloff

2. **`normal` (t_vec3)**:
   - The surface normal vector at the hit point
   - Perpendicular to the object's surface
   - Essential for calculating how light reflects off the surface

3. **`object_color` (t_vec3)**:
   - The base color of the object at the hit point
   - Typically defined in the scene file
   - Represents the object's material properties in terms of which wavelengths it reflects

4. **`light` (t_light)**:
   - Contains properties of the light source:
     - Position: Where the light is located in 3D space
     - Color: The color of the light being emitted
     - Brightness: The intensity of the light

## Output

- **Color value (t_vec3)**:
   - The final RGB color for the pixel after lighting calculations
   - Combines ambient and diffuse lighting components
   - Values typically range from 0 to 255 for each channel

## Lighting Components Calculated

### 1. Ambient Lighting

```c
double ambient_strength = 0.1;
t_vec3 ambient = vec3_mult(object_color, ambient_strength);
```

- **Purpose**: Simulates indirect lighting or global illumination in a simplified way
- **Why needed**: Prevents objects in shadow from appearing completely black
- **Implementation**: A constant factor applied to the object's color
- **Physical basis**: Approximates light that has bounced multiple times in the environment

### 2. Diffuse Lighting (Lambertian Reflection)

```c
t_vec3 light_dir = vec3_normalize(vec3_sub(light.position, hit_point));
double diff = fmax(vec3_dot(normal, light_dir), 0.0);
t_vec3 diffuse = vec3_mult(vec3_mul_vec(light.color, object_color), diff);
```

- **Purpose**: Simulates the scattering of light on matte surfaces
- **Why needed**: Creates the basic shading that gives objects their 3D appearance
- **Implementation**: Calculated using the dot product between the surface normal and the light direction
- **Physical basis**: Based on Lambert's cosine law - light intensity is proportional to the cosine of the angle between the normal and light direction

## Why Lighting Calculation Is Essential in Ray Tracing

1. **Visual Realism**: Proper lighting is fundamental to creating realistic images
   
2. **Depth Perception**: Lighting cues help the human visual system interpret depth in a 2D image
   
3. **Material Differentiation**: Different lighting responses help distinguish between materials
   
4. **Mood and Atmosphere**: Lighting strongly influences the mood and atmosphere of a rendered scene

## Potential Enhancements

1. **Specular Highlights**: Add specular reflection for shiny surfaces
   
2. **Shadows**: Implement shadow rays to check if a point is in shadow
   
3. **Light Attenuation**: Add distance-based light falloff
   
4. **Multiple Lights**: Support for multiple light sources
   
5. **Global Illumination**: More sophisticated ambient lighting models

## Example Implementation

```c
t_vec3 compute_lighting(t_point3 hit_point, t_vec3 normal, t_vec3 object_color, t_light light)
{
    // Ambient component
    double ambient_strength = 0.1;
    t_vec3 ambient = vec3_mult(object_color, ambient_strength);

    // Diffuse component
    t_vec3 light_dir = vec3_normalize(vec3_sub(light.position, hit_point));
    double diff = fmax(vec3_dot(normal, light_dir), 0.0);
    t_vec3 diffuse = vec3_mult(vec3_mul_vec(light.color, object_color), diff);

    // Combine lighting components
    return vec3_add(ambient, diffuse);
}
```

## Conclusion

The `compute_lighting` function is a fundamental component that translates mathematical intersection data into visually meaningful images. Its implementation directly affects the realism and aesthetic quality of the rendered scene. While a basic ambient and diffuse lighting model provides a good starting point, more sophisticated lighting models can significantly enhance the visual fidelity of the ray tracer.
