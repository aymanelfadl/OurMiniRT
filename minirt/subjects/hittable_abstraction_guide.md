# Hittable Object Abstraction Guide

This document explains the new abstraction system implemented in our ray tracer to make it more extensible and object-oriented.

## Core Concept: Hittable Interface

Our hittable interface implements a modular approach to geometric primitives in the ray tracer. Each object type implements a common set of methods that allow the ray tracer to interact with them in a unified way.

## Structure and Interfaces

### Key Types

1. `t_hittable`: The main container type that holds:
   - `data`: A pointer to the specific object data (sphere, plane, etc.)
   - `methods`: A structure of function pointers for object-specific behavior

2. `t_hittable_methods`: The interface that all objects must implement:
   - `hit`: Tests if a ray intersects with the object
   - `get_color`: Returns the object's color
   - `free`: Cleans up resources when the object is destroyed

3. `t_object_type`: Enum to differentiate between object types (SPHERE, PLANE, etc.)

### Common Pattern

All object implementations follow this pattern:

1. Define object-specific data structure (e.g., `t_sphere`, `t_plane`)
2. Implement hit function to calculate ray intersection
3. Implement color and free utility functions
4. Create a factory function to instantiate objects

## Adding a New Primitive

To add a new primitive type to the ray tracer, follow these steps:

1. **Define the Object Structure**

```c
typedef struct s_new_primitive {
    // Geometric properties
    t_vec3 position;
    // Other properties specific to this primitive
    t_vec3 color;
    t_material material;
} t_new_primitive;
```

2. **Implement the Hit Function**

```c
int new_primitive_hit(const void *self, t_ray ray, double t_min, double t_max, t_hit_record *rec) {
    const t_new_primitive *prim = (const t_new_primitive *)self;
    
    // Implement ray-primitive intersection logic
    
    // If there's a hit, set hit record information:
    rec->t = ray_parameter;
    rec->p = hit_point;
    set_face_normal(ray, outward_normal, rec);
    return 1;  // Return 1 for hit, 0 for miss
}
```

3. **Implement Utility Functions**

```c
t_vec3 get_new_primitive_color(const void *self) {
    const t_new_primitive *prim = (const t_new_primitive *)self;
    return prim->color;
}

void free_new_primitive(void *self) {
    free(self);  // Add any additional cleanup if needed
}
```

4. **Create the Factory Function**

```c
t_hittable make_new_primitive(/* parameters */) {
    t_new_primitive *prim = malloc(sizeof(t_new_primitive));
    // Initialize primitive properties
    
    return create_hittable(prim, new_primitive_hit, get_new_primitive_color, free_new_primitive);
}
```

5. **Use the New Primitive**

```c
world[world_count++] = make_new_primitive(/* parameters */);
```

## Benefits of This Approach

1. **Extensibility**: New primitive types can be added without changing existing code
2. **Encapsulation**: Each primitive type manages its own data and behavior
3. **Memory Safety**: Each object has its own free method for proper cleanup
4. **Polymorphism**: The ray tracer can handle all objects through the same interface

## Example Implementations

The current implementation includes:
- Spheres
- Planes
- Cylinders

Each follows the pattern described above.

## Advanced Extensions

Future enhancements could include:
1. Advanced materials (reflection, refraction, etc.)
2. Texture mapping
3. Composite objects
4. Mesh-based objects

These can all be implemented using the same abstraction pattern.
