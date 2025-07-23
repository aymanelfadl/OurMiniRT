# Hittable Interface - Architecture and Design

This document explains the object-oriented design approach used in our ray tracer to create an extensible architecture for various types of objects that can be "hit" by a ray.

## Core Abstraction

The key to our implementation is the abstraction of "hittable" objects. A hittable object is anything that can be intersected by a ray - such as spheres, planes, cylinders, etc.

### The Hittable Interface

The interface is defined through the `t_hittable` structure:

```c
typedef struct s_hittable_methods {
    int (*hit)(const void *self, t_ray ray, double t_min, double t_max, t_hit_record *rec);
    t_vec3 (*get_color)(const void *self);
    void (*free)(void *self);
} t_hittable_methods;

typedef struct s_hittable {
    void *data;
    t_hittable_methods methods;
} t_hittable;
```

This provides a uniform interface for all hittable objects with the following methods:

1. `hit`: Tests for ray intersection and calculates hit information
2. `get_color`: Returns the object's color
3. `free`: Handles cleanup of the object's allocated memory

### Factory Function

The `create_hittable` function creates a new hittable object with the specified methods:

```c
t_hittable create_hittable(void *data, 
                         int (*hit_fn)(const void *self, t_ray ray, double t_min, double t_max, t_hit_record *rec),
                         t_vec3 (*color_fn)(const void *self), 
                         void (*free_fn)(void *self)) {
    t_hittable obj;
    obj.data = data;
    obj.methods.hit = hit_fn;
    obj.methods.get_color = color_fn;
    obj.methods.free = free_fn;
    return obj;
}
```

## Implementing New Shapes

To add a new shape:

1. Define the shape's data structure:
   ```c
   typedef struct s_new_shape {
       // Shape-specific data
       t_vec3 color;
       t_material material;
   } t_new_shape;
   ```

2. Implement the hit function for the shape:
   ```c
   int new_shape_hit(const void *self, t_ray ray, double t_min, double t_max, t_hit_record *rec) {
       const t_new_shape *shape = (const t_new_shape *)self;
       // Implement intersection logic
       // ...
       return hit_found ? 1 : 0;
   }
   ```

3. Implement the color function:
   ```c
   t_vec3 get_new_shape_color(const void *self) {
       const t_new_shape *shape = (const t_new_shape *)self;
       return shape->color;
   }
   ```

4. Implement the free function:
   ```c
   void free_new_shape(void *self) {
       free(self);
   }
   ```

5. Create a factory function:
   ```c
   t_hittable make_new_shape(...parameters...) {
       t_new_shape *shape = malloc(sizeof(t_new_shape));
       // Initialize shape properties
       
       t_hittable obj = create_hittable(shape, new_shape_hit, get_new_shape_color, free_new_shape);
       return obj;
   }
   ```

## Using the Interface

With this design, we can handle all hittable objects uniformly:

```c
for (int i = 0; i < world_size; i++) {
    if (world[i].methods.hit(world[i].data, ray, 0.001, closest, &rec)) {
        // Handle hit...
        t_vec3 obj_color = world[i].methods.get_color(world[i].data);
    }
}
```

## Memory Management

The design ensures proper cleanup:

```c
// Free allocated objects
for (int i = 0; i < world_count; i++) {
    if (world[i].methods.free) {
        world[i].methods.free(world[i].data);
    }
}
```

## Benefits

1. **Polymorphism**: Different shapes can be treated identically through the hittable interface
2. **Encapsulation**: Shape-specific implementation details are hidden
3. **Extensibility**: New shapes can be added without modifying existing code
4. **Type Safety**: Each shape's data is properly typed within its own functions
5. **Memory Management**: Clean allocation and deallocation of resources

This architecture provides a solid foundation for extending the ray tracer with various types of objects and materials.
