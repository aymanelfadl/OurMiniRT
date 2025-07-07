# Ray Tracing Mathematics Explained

This document explains the mathematical concepts behind the ray tracer implemented in the MiniRT project, including the key issues we fixed to get the spheres displaying properly.

## Table of Contents
1. [Basic Ray Tracing Concepts](#basic-ray-tracing-concepts)
2. [Camera and Viewport Setup](#camera-and-viewport-setup)
3. [Ray-Sphere Intersection](#ray-sphere-intersection)
4. [Normal Vectors and Shading](#normal-vectors-and-shading)
5. [The Bug Fix: Correcting Intersection Calculations](#the-bug-fix-correcting-intersection-calculations)
6. [Next Steps](#next-steps)

## Basic Ray Tracing Concepts

Ray tracing is a rendering technique that simulates the physical behavior of light to create realistic images. The basic algorithm works as follows:

1. For each pixel in the image:
   - Generate a ray from the camera through the pixel
   - Determine which objects the ray intersects
   - Calculate the color based on the object properties and lighting

In mathematical terms, a ray is represented as:
```
P(t) = O + t * D
```
Where:
- `P(t)` is a point along the ray at distance `t`
- `O` is the origin point (camera position)
- `D` is the direction vector
- `t` is a scalar parameter (distance along the ray)

## Camera and Viewport Setup

The camera setup in our implementation uses:

- Camera center at `(0, 0, 0)`
- Viewport positioned at distance `focal_length` (1.0) in front of the camera
- Field of view (FOV) of 90 degrees
- Viewport height and width calculated based on the FOV and aspect ratio

The viewport dimensions are calculated as:
```
theta = fov * π / 180
viewport_height = 2 * tan(theta/2) * focal_length
viewport_width = viewport_height * aspect_ratio
```

For each pixel, we calculate its position in 3D space:
```
pixel_position = viewport_upper_left + (i * pixel_delta_u) + (j * pixel_delta_v)
```

Then the ray direction is:
```
ray_direction = normalize(pixel_position - camera_center)
```

## Ray-Sphere Intersection

The most critical part of our implementation is detecting when a ray intersects with a sphere. This is done by solving a quadratic equation.

For a sphere with center `C` and radius `r`, a point `P` is on the sphere if:
```
|P - C|² = r²
```

Substituting our ray equation P(t) = O + t * D:
```
|O + t*D - C|² = r²
```

Expanding:
```
|O - C + t*D|² = r²
```

Using the dot product and rearranging:
```
(D·D)t² + 2(D·(O-C))t + (O-C)·(O-C) - r² = 0
```

In our code, we simplified this to:
```
a = dot(D, D)
h = dot(D, O-C)  // This is half of the b term in the quadratic formula
c = dot(O-C, O-C) - r²
discriminant = h² - a*c
```

## Normal Vectors and Shading

Once we find an intersection, we calculate the normal vector (perpendicular to the surface):
```
normal = normalize(intersection_point - sphere_center)
```

We adjust the normal direction based on whether the ray is hitting from outside (front face) or inside:
```
front_face = dot(ray_direction, outward_normal) < 0
normal = front_face ? outward_normal : -outward_normal
```

Our simple shading model maps the normal vector to a color:
```
color = 0.5 * (normal + (1, 1, 1))
```

This creates a gradient effect that helps visualize the 3D shape.

## The Bug Fix: Correcting Intersection Calculations

The key issue in the original code was in the ray-sphere intersection calculation. The quadratic formula for finding the intersection points is:
```
t = (-b ± sqrt(b² - 4ac)) / 2a
```

In our simplified version:
```
t = (-h ± sqrt(h² - ac)) / a
```

The original code incorrectly used:
```
root = (h - sqrt_d) / a
```

Which was fixed to:
```
root = (-h - sqrt_d) / a
```

The sign error caused rays to miss intersections with the spheres, even though the math indicated they should hit. This is why no spheres were visible despite the rays being correctly generated.

### Why the Sign Matters

When a ray is pointing toward a sphere:
- `h` (the half-b term) is typically negative
- For a ray originating outside the sphere, the smaller root represents where the ray enters the sphere
- Using the wrong sign flips the meaning of the roots

## Next Steps

With the basic ray tracer working, you can extend it to:

1. Add material properties (reflectivity, transparency, etc.)
2. Implement more primitives (planes, cylinders, cones)
3. Add lighting calculations (ambient, diffuse, specular)
4. Implement shadows by casting shadow rays
5. Add textures and more complex shading

The current version focuses on the fundamental algorithm and correct intersection detection, which is the foundation for all these advanced features.
