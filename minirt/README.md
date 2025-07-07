# MiniRT: Ray Tracing in C

This project implements a raytracer based on the "Ray Tracing in One Weekend" book by Peter Shirley, but written in C without classes.

## Features

- **Abstract Classes in C**: Implementation of OOP-like concepts in C using structs and function pointers
- **Materials**: Lambertian (diffuse), Metal, and Dielectric (glass)
- **Primitives**: Spheres, Planes, and Cylinders
- **Camera**: Configurable position, look-at, field of view, aperture
- **Realistic Effects**: Reflection, refraction, anti-aliasing, depth of field

## Project Structure

```
├── includes/           # Header files
│   ├── minirt.h        # Main header
│   ├── vector.h        # Vector operations
│   ├── ray.h           # Ray definition
│   ├── utility.h       # Utility functions
│   ├── hittable.h      # Hittable interface
│   ├── hittable_list.h # Object collection
│   ├── material.h      # Material system
│   └── camera.h        # Camera abstraction
├── src/                # Source files
│   ├── main.c          # Main program
│   ├── vector.c        # Vector implementation
│   ├── utility.c       # Utility functions
│   ├── ray.c           # Ray operations
│   ├── camera.c        # Camera implementation
│   ├── material.c      # Material system
│   ├── hittable.c      # Primitives implementation
│   ├── hittable_list.c # World object management
│   └── render.c        # Rendering functions
└── Makefile            # Build system
```

## How It Works

### Abstract "Classes" in C

Since C doesn't have classes or inheritance, we use a combination of structs and function pointers to achieve similar functionality:

```c
// Hittable interface with function pointers
typedef struct s_hittable_methods {
    int (*hit)(const void *self, t_ray ray, double t_min, double t_max, t_hit_record *rec);
    void (*free)(void *self);
} t_hittable_methods;

// Hittable "abstract class"
typedef struct s_hittable {
    void *data;                 // Object-specific data
    t_hittable_methods methods; // Function pointers
} t_hittable;
```

### Material System

Similarly, materials use function pointers for their behavior:

```c
typedef struct s_material_methods {
    int (*scatter)(const void *material, const t_ray *ray_in, 
                   const struct s_hit_record *rec, 
                   t_color3 *attenuation, t_ray *scattered);
    void (*free)(void *material);
} t_material_methods;
```

### Scene Setup

A scene is created by adding various objects to a world list, each with its own material:

```c
t_material mat_ground = create_lambertian(vec3_create(0.5, 0.5, 0.5));
t_material mat_center = create_lambertian(vec3_create(0.7, 0.3, 0.3));

hittable_list_add(&app->world, create_sphere(
    vec3_create(0, 0, -1),
    0.5,
    mat_center
));
```

## Building and Running

```bash
# Build the project
make

# Run the raytracer
./minirt

# Or use the run script:
./run.sh
```

## References

- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
