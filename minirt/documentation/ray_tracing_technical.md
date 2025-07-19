# MiniRT Ray Tracer - Technical Documentation

## Overview

MiniRT is a basic ray tracing engine implemented in C that renders 3D scenes described in simple text files. It uses the MLX library for window management and pixel display, with custom mathematics and geometry handling for the ray tracing algorithm.

## Ray Tracing Pipeline

### 1. Scene Loading and Initialization

- **Input:** Scene description file (.rt)
- **Process:** Parse the file to extract camera, lights, and objects
- **Output:** In-memory representation of the scene

The scene loading process reads a .rt file and constructs data structures representing:
- Ambient light
- Camera properties
- Light sources
- Geometric primitives (spheres, planes, cylinders)

### 2. Ray Generation

For each pixel in the output image:

- **Input:** Camera position, orientation, and field of view
- **Process:** Calculate ray direction from camera through pixel
- **Output:** Ray with origin and direction

The ray direction is calculated using:
```c
t_vec3 dir = get_ray_direction(&scene->camera, i, j, WIDTH, HEIGHT);
t_ray  ray = (t_ray){scene->camera.origin, dir};
```

### 3. Ray-Object Intersection

For each ray:

- **Input:** Ray and scene objects
- **Process:** Check for intersections with all objects, find closest
- **Output:** Hit information (point, normal, color) or background color

The trace_ray function loops through all objects in the scene:
```c
while (node)
{
    t_object *object = node->content;
    t_hit current_hit;
    int is_hit = 0;

    if (object->type == SPHERE)
        is_hit = ray_sphere(ray, &object->sphere, &current_hit);
    else if (object->type == PLANE)
        is_hit = ray_plane(ray, &object->plane, &current_hit);

    if (is_hit && current_hit.t < closest_hit.t)
        closest_hit = current_hit;

    node = node->next;
}
```

### 4. Lighting Calculation

For each intersection point:

- **Input:** Hit point, surface normal, object color, light sources
- **Process:** Calculate lighting using ambient and diffuse components
- **Output:** Final pixel color

The lighting model includes:
- **Ambient lighting:** Provides base illumination for areas not directly lit
- **Diffuse lighting:** Creates shading based on the angle between surface and light

```c
t_vec3 compute_lighting(t_point3 hit_point, t_vec3 normal, t_vec3 object_color, t_scene *scene)
{
    /* === Ambient === */
    t_vec3 ambient = vec3_mul_vec(object_color, scene->ambient.color);
    ambient = vec3_mult(ambient, scene->ambient.ratio / 255.0);

    /* === Diffuse === */
    t_vec3 to_light = vec3_normalize(vec3_sub(scene->light.position, hit_point));
    float diffuse_factor = fmaxf(vec3_dot(normal, to_light), 0.0f);
    t_vec3 diffuse = vec3_mul_vec(object_color, scene->light.color);
    diffuse = vec3_mult(diffuse, (scene->light.brightness * diffuse_factor) / 255.0);

    /* === Final Color === */
    return vec3_add(ambient, diffuse);
}
```

### 5. Image Rendering

- **Input:** Array of calculated pixel colors
- **Process:** Convert colors to displayable format and write to image buffer
- **Output:** Rendered image displayed on screen

```c
/* 3. Clamp colour and pack into 0x00RRGGBB. */
int r = (int)fminf(255, fmaxf(0, c.x));
int g = (int)fminf(255, fmaxf(0, c.y));
int b = (int)fminf(255, fmaxf(0, c.z));
int rgb = (r << 16) | (g << 8) | b;

/* 4. Write pixel into image buffer. */
my_mlx_pixel_put(&scene->image, i, j, rgb);
```

## Key Algorithms

### Ray-Sphere Intersection

Ray-sphere intersection solves the quadratic equation:

```
|P(t) - C|^2 = R^2
```

Where:
- P(t) = O + tD (ray equation)
- C is the sphere center
- R is the sphere radius

In code:
```c
float a = vec3_dot(r->direction, r->direction);
float b = 2.f * vec3_dot(oc, r->direction);
float c = vec3_dot(oc, oc) - s->radius * s->radius;

float disc = b * b - 4.f * a * c;
if (disc < 0.f)
    return 0;
float t = (-b - sqrtf(disc)) / (2.f * a);
```

### Ray-Plane Intersection

Ray-plane intersection solves:

```
(P - P0) · N = 0
```

Where:
- P is a point on the ray: P = O + tD
- P0 is a point on the plane
- N is the plane's normal vector

In code:
```c
float denom = vec3_dot(pl->normal, r->direction);
t_vec3 diff = vec3_sub(pl->point, r->origin);
float t = vec3_dot(diff, pl->normal) / denom;
```

## Vector Mathematics

The engine implements several 3D vector operations:

- **Vector Addition:** `vec3_add(v1, v2)`
- **Vector Subtraction:** `vec3_sub(v1, v2)`
- **Scalar Multiplication:** `vec3_mult(v, scalar)`
- **Component-wise Multiplication:** `vec3_mul_vec(v1, v2)`
- **Dot Product:** `vec3_dot(v1, v2)`
- **Normalization:** `vec3_normalize(v)`

## Future Enhancements

1. **Shadow Casting**
   - Cast shadow rays from hit points to light sources
   - Check if any object blocks the light path

2. **Specular Reflection**
   - Add mirror-like reflection for shiny surfaces
   - Implement Phong or Blinn-Phong specular model

3. **Refraction**
   - Implement transparent materials with refraction
   - Use Snell's law for calculating refraction angles

4. **More Primitives**
   - Add support for cones, triangles, etc.
   - Implement bounding volume hierarchies for efficiency

5. **Texture Mapping**
   - Apply image textures to object surfaces
   - Implement UV coordinate mapping

6. **Anti-aliasing**
   - Cast multiple rays per pixel
   - Average results for smoother edges

## Conclusion

MiniRT demonstrates the fundamental principles of ray tracing by implementing a pipeline from scene description to rendered image. While the current implementation focuses on basic rendering with ambient and diffuse lighting, the modular structure allows for future enhancements to improve visual quality and performance.
